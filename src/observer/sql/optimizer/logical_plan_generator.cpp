/* Copyright (c) 2023 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/08/16.
//

#include "sql/optimizer/logical_plan_generator.h"

#include <common/log/log.h>

#include "sql/expr/expression.h"

#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/group_by_logical_operator.h"

#include "sql/stmt/calc_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/explain_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"
#include "sql/stmt/update_stmt.h"

#include "sql/expr/expression_iterator.h"
#include "sql/operator/update_logical_operator.h"

using namespace std;
using namespace common;

RC LogicalPlanGenerator::create(Stmt *stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  RC rc = RC::SUCCESS;
  switch (stmt->type()) {
    case StmtType::CALC: {
      CalcStmt *calc_stmt = static_cast<CalcStmt *>(stmt);

      rc = create_plan(calc_stmt, logical_operator);
    } break;

    case StmtType::SELECT: {
      SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);

      rc = create_plan(select_stmt, logical_operator);
    } break;

    case StmtType::INSERT: {
      InsertStmt *insert_stmt = static_cast<InsertStmt *>(stmt);

      rc = create_plan(insert_stmt, logical_operator);
    } break;

    case StmtType::DELETE: {
      DeleteStmt *delete_stmt = static_cast<DeleteStmt *>(stmt);

      rc = create_plan(delete_stmt, logical_operator);
    } break;

    case StmtType::EXPLAIN: {
      ExplainStmt *explain_stmt = static_cast<ExplainStmt *>(stmt);

      rc = create_plan(explain_stmt, logical_operator);
    } break;
    case StmtType::UPDATE: {
      UpdateStmt *update_stmt = static_cast<UpdateStmt *>(stmt);

      rc = create_plan(update_stmt, logical_operator);
    } break;
    default: {
      rc = RC::UNIMPLEMENTED;
    }
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(CalcStmt *calc_stmt, std::unique_ptr<LogicalOperator> &logical_operator)
{
  logical_operator.reset(new CalcLogicalOperator(std::move(calc_stmt->expressions())));
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  unique_ptr<LogicalOperator> *last_oper = nullptr;

  unique_ptr<LogicalOperator> table_oper(nullptr);
  last_oper = &table_oper;

  const std::vector<Table *> &tables = select_stmt->tables();
  for (Table *table : tables) {

    unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_ONLY));
    if (table_oper == nullptr) {
      table_oper = std::move(table_get_oper);
    } else {
      JoinLogicalOperator *join_oper = new JoinLogicalOperator;
      join_oper->add_child(std::move(table_oper));
      join_oper->add_child(std::move(table_get_oper));
      table_oper = unique_ptr<LogicalOperator>(join_oper);
    }
  }

  unique_ptr<LogicalOperator> predicate_oper;

  RC rc = create_plan(select_stmt->filter_stmt(), predicate_oper, select_stmt->table_map());
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to create predicate logical plan. rc=%s", strrc(rc));
    return rc;
  }

  if (predicate_oper) {
    if (*last_oper) {
      predicate_oper->add_child(std::move(*last_oper));
    }

    last_oper = &predicate_oper;
  }

  unique_ptr<LogicalOperator> group_by_oper;
  rc = create_group_by_plan(select_stmt, group_by_oper);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to create group by logical plan. rc=%s", strrc(rc));
    return rc;
  }

  if (group_by_oper) {
    if (*last_oper) {
      group_by_oper->add_child(std::move(*last_oper));
    }

    last_oper = &group_by_oper;
  }

  auto project_oper = make_unique<ProjectLogicalOperator>(std::move(select_stmt->query_expressions()));
  if (*last_oper) {
    project_oper->add_child(std::move(*last_oper));
  }

  logical_operator = std::move(project_oper);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(FilterStmt *filter_stmt, unique_ptr<LogicalOperator> &logical_operator, std::unordered_map<std::string, Table *> &table_map)
{
  RC                                  rc = RC::SUCCESS;

  unique_ptr<PredicateLogicalOperator> predicate_oper;
  if(filter_stmt != nullptr) {
    ConjunctionExpr* conjunction_expr = filter_stmt->conjunction();
    rc = traversal(conjunction_expr, filter_stmt->table(), filter_stmt->db(), table_map);
    if (OB_FAIL(rc)) {
      LOG_WARN("failed to create predicate logical plan. rc=%s", strrc(rc));
      return rc;
    }
    predicate_oper = unique_ptr<PredicateLogicalOperator>(new PredicateLogicalOperator(std::unique_ptr<ConjunctionExpr>(conjunction_expr )));
  }else {
    predicate_oper = nullptr;
  }

  logical_operator = std::move(predicate_oper);
  return rc;
}

RC LogicalPlanGenerator::traversal(ConjunctionExpr *expr, Table *default_table, Db *db, std::unordered_map<std::string, Table *> &table_map)
{
  RC rc = RC::SUCCESS;
  // 获取左右表达式
  Expression *left = expr->left().get();
  Expression *right = expr->right().get();

  if(left->type() == ExprType::CONJUNCTION) {
    rc = traversal(dynamic_cast<ConjunctionExpr*>(left), default_table, db, table_map);
    if(OB_FAIL(rc)) {
      LOG_WARN("failed to traversal left expression. rc=%s", strrc(rc));
      return rc;
    }
  }else if(left->type() == ExprType::COMPARISON ) {
     rc = comparison_process(dynamic_cast<ComparisonExpr*>(left), default_table, db, table_map);
     if(OB_FAIL(rc)) {
      LOG_WARN("failed to traversal left expression. rc=%s", strrc(rc));
      return rc;
    }
  }
  else if(left->type() == ExprType::VALUE) {

  }else {
    LOG_ERROR("unexpected expression type: %s", left->type());
    rc = RC::INTERNAL;
  }

  if(right->type() == ExprType::CONJUNCTION) {
    rc = traversal(dynamic_cast<ConjunctionExpr*>(right), default_table, db, table_map);
    if(OB_FAIL(rc)) {
      LOG_WARN("failed to traversal right expression. rc=%s", strrc(rc));
      return rc;
    }
  }else if(right->type() == ExprType::COMPARISON) {
    rc = comparison_process(dynamic_cast<ComparisonExpr*>(right), default_table, db, table_map);
    if(OB_FAIL(rc)) {
      LOG_WARN("failed to traversal right expression. rc=%s", strrc(rc));
      return rc;
    }
  }else if(left->type() == ExprType::VALUE) {

  }else {
    LOG_ERROR("unexpected expression type: %s", right->type());
    rc = RC::INTERNAL;
  }
  return rc;
}

RC LogicalPlanGenerator::comparison_process(ComparisonExpr *expr, Table *default_table, Db *db, std::unordered_map<std::string, Table *> &table_map)
{
  RC rc = RC::SUCCESS;


  // Expression *left_child = expr->left().get();
  std::unique_ptr<Expression> left_child = std::move(expr->left());
  // 如果是unbound_field，则需要替换为field_expr
  if(left_child->type() == ExprType::UNBOUND_FIELD) {
    UnboundFieldExpr* unbound_expr = dynamic_cast<UnboundFieldExpr*>(left_child.get());
    Table *table;
    if(unbound_expr->table_name() == nullptr || unbound_expr->table_name()[0] == '\0') {
      table = default_table;
    }else {
      table = table_map[unbound_expr->table_name()];
    }
    const FieldMeta *field_meta = table->table_meta().field(unbound_expr->field_name());
    // field不存在，返回错误
    if(field_meta == nullptr) {
      LOG_WARN("field not found: %s", unbound_expr->field_name());
      return RC::SCHEMA_FIELD_MISSING;
    }
    left_child = std::make_unique<FieldExpr>(table, field_meta);
  }else if (left_child-> type() == ExprType::ARITHMETIC) {
    rc = arithmetic_process(dynamic_cast<ArithmeticExpr*>(left_child.get()), default_table, db, table_map);
    // 判断rc
    if(OB_FAIL(rc)) {
      LOG_WARN("failed to process arithmetic expression. rc=%s", strrc(rc));
      return rc;
    }
  }else if(left_child-> type() == ExprType::SUBQUERY) {
    auto subquery_expr = dynamic_cast<SubQueryExpr*>(left_child.get());
    rc = subquery_expr->Create_stmt(db);
    // 判断rc
    if(OB_FAIL(rc)) {
      LOG_WARN("failed to process subquery expression. rc=%s", strrc(rc));
      return rc;
    }
  }else if(left_child-> type() == ExprType::FUNCTION) {
    rc = func_expr_process(dynamic_cast<FuncExpr*>(left_child.get()), default_table, db, table_map);
    // 判断rc
    if(OB_FAIL(rc)) {
      LOG_WARN("failed to process fucntion expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  // Expression *right_child = expr->right().get();
  std::unique_ptr<Expression> right_child = std::move(expr->right());
  if(right_child->type() == ExprType::UNBOUND_FIELD) {
    UnboundFieldExpr* unbound_expr = dynamic_cast<UnboundFieldExpr*>(right_child.get());
    Table *table;
    if(unbound_expr->table_name() == nullptr || unbound_expr->table_name()[0] == '\0') {
      table = default_table;
    }else {
      table = table_map[unbound_expr->table_name()];
    }
    const FieldMeta *field_meta = table->table_meta().field(unbound_expr->field_name());
    // field不存在，返回错误
    if(field_meta == nullptr) {
      LOG_WARN("field not found: %s", unbound_expr->field_name());
      return RC::SCHEMA_FIELD_MISSING;
    }
    right_child = std::make_unique<FieldExpr>(table, field_meta);
  }else if (right_child-> type() == ExprType::ARITHMETIC) {
    rc = arithmetic_process(dynamic_cast<ArithmeticExpr*>(right_child.get()), default_table, db, table_map);
    // 判断rc
    if(OB_FAIL(rc)) {
      LOG_WARN("failed to process arithmetic expression. rc=%s", strrc(rc));
      return rc;
    }
  }else if(right_child-> type() == ExprType::SUBQUERY) {
    auto subquery_expr = dynamic_cast<SubQueryExpr*>(right_child.get());
    rc = subquery_expr->Create_stmt(db);
    // 判断rc
    if(OB_FAIL(rc)) {
      LOG_WARN("failed to process subquery expression. rc=%s", strrc(rc));
      return rc;
    }
  }else if(right_child-> type() == ExprType::FUNCTION) {
    rc = func_expr_process(dynamic_cast<FuncExpr*>(right_child.get()), default_table, db, table_map);
    // 判断rc
    if(OB_FAIL(rc)) {
      LOG_WARN("failed to process fucntion expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  // 尝试隐式转换
  // 左右含有null值，则无法隐式转换
  // 运算符为IS_NULL 或 IS_NOT_NULL , 则不进行隐式转换
  if((left_child->type() == ExprType::VALUE && left_child->value_type() == AttrType::NULLS) ||
      (right_child->type() == ExprType::VALUE && right_child->value_type() == AttrType::NULLS) ||
      (expr->comp() == CompOp::IS_NULL || expr->comp() == CompOp::IS_NOT_NULL)
      ) {
    LOG_INFO("implicit cast not support null value");
  }
  else if (left_child->value_type() != right_child->value_type()) {
    auto left_to_right_cost = implicit_cast_cost(left_child->value_type(), right_child->value_type());
    auto right_to_left_cost = implicit_cast_cost(right_child->value_type(), left_child->value_type());
    if (left_to_right_cost <= right_to_left_cost && left_to_right_cost != INT32_MAX) {
      ExprType left_type = left_child->type();
      std::unique_ptr<CastExpr> cast_expr =std::make_unique<CastExpr>(std::move(left_child), right_child->value_type());
      if (left_type == ExprType::VALUE) {
        Value left_val;
        if (OB_FAIL(rc = cast_expr->try_get_value(left_val)))
        {
          LOG_WARN("failed to get value from left child", strrc(rc));
          return rc;
        }
        left_child = std::make_unique<ValueExpr>(left_val);
      } else {
        left_child = std::move(cast_expr);
      }
    } else if (right_to_left_cost < left_to_right_cost && right_to_left_cost != INT32_MAX) {
      ExprType right_type = right_child->type();
      std::unique_ptr<CastExpr> cast_expr = std::make_unique<CastExpr>(std::move(right_child), left_child->value_type());
      if (right_type == ExprType::VALUE) {
        Value right_val;
        if (OB_FAIL(rc = cast_expr->try_get_value(right_val)))
        {
          LOG_WARN("failed to get value from right child", strrc(rc));
          return rc;
        }
        right_child = std::make_unique<ValueExpr>(right_val);
      } else {
        right_child = std::move(cast_expr);
      }

    } else {
      rc = RC::UNSUPPORTED;
      LOG_WARN("unsupported cast from %s to %s", attr_type_to_string(left_child->value_type()), attr_type_to_string(right_child->value_type()));
      return rc;
    }
  }

  expr->left() = std::move(left_child);
  expr->right() = std::move(right_child);

  return rc;
}

RC LogicalPlanGenerator::arithmetic_process(ArithmeticExpr *expr, Table *default_table, Db *db, std::unordered_map<std::string, Table *> &table_map)
{
  RC rc = RC::SUCCESS;

  // Expression *left_child = expr->left().get();
  std::unique_ptr<Expression> left_child = std::move(expr->left());
  // 如果是unbound_field，则需要替换为field_expr
  if(left_child->type() == ExprType::UNBOUND_FIELD) {
    UnboundFieldExpr* unbound_expr = dynamic_cast<UnboundFieldExpr*>(left_child.get());
    Table *table;
    if(unbound_expr->table_name() == nullptr || unbound_expr->table_name()[0] == '\0') {
      table = default_table;
    }else {
      table = table_map[unbound_expr->table_name()];
    }
    const FieldMeta *field_meta = table->table_meta().field(unbound_expr->field_name());
    // field不存在，返回错误
    if(field_meta == nullptr) {
      LOG_WARN("field not found: %s", unbound_expr->field_name());
      return RC::SCHEMA_FIELD_MISSING;
    }
    left_child = std::make_unique<FieldExpr>(table, field_meta);
  }else if (left_child-> type() == ExprType::ARITHMETIC) {
    rc = arithmetic_process(dynamic_cast<ArithmeticExpr*>(left_child.get()), default_table, db, table_map);
    // 判断rc
    if(OB_FAIL(rc)) {
      LOG_WARN("failed to process arithmetic expression. rc=%s", strrc(rc));
      return rc;
    }
  }else if(left_child-> type() == ExprType::FUNCTION) {
    rc = func_expr_process(dynamic_cast<FuncExpr*>(left_child.get()), default_table, db, table_map);
    // 判断rc
    if(OB_FAIL(rc)) {
      LOG_WARN("failed to process fucntion expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  // 处理右子表达式
  std::unique_ptr<Expression> right_child = nullptr;
  if (expr->right() != nullptr) {
    right_child = std::move(expr->right());
    if(right_child->type() == ExprType::UNBOUND_FIELD) {
      UnboundFieldExpr* unbound_expr = dynamic_cast<UnboundFieldExpr*>(right_child.get());
      Table *table;
      if(unbound_expr->table_name() == nullptr || unbound_expr->table_name()[0] == '\0') {
        table = default_table;
      }else {
        table = table_map[unbound_expr->table_name()];
      }
      const FieldMeta *field_meta = table->table_meta().field(unbound_expr->field_name());
      // field不存在，返回错误
      if(field_meta == nullptr) {
        LOG_WARN("field not found: %s", unbound_expr->field_name());
        return RC::SCHEMA_FIELD_MISSING;
      }
      right_child = std::make_unique<FieldExpr>(table, field_meta);
    }else if (right_child-> type() == ExprType::ARITHMETIC) {
      rc = arithmetic_process(dynamic_cast<ArithmeticExpr*>(right_child.get()), default_table, db, table_map);
      // 判断rc
      if(OB_FAIL(rc)) {
        LOG_WARN("failed to process arithmetic expression. rc=%s", strrc(rc));
        return rc;
      }
    }else if(right_child-> type() == ExprType::FUNCTION) {
      rc = func_expr_process(dynamic_cast<FuncExpr*>(right_child.get()), default_table, db, table_map);
      // 判断rc
      if(OB_FAIL(rc)) {
        LOG_WARN("failed to process fucntion expression. rc=%s", strrc(rc));
        return rc;
      }
    }
  }

  // 如果右子表达式不为空且类型不同，则进行隐式类型转换
  if (right_child != nullptr && left_child->value_type() != right_child->value_type()) {
    auto left_to_right_cost = implicit_cast_cost(left_child->value_type(), right_child->value_type());
    auto right_to_left_cost = implicit_cast_cost(right_child->value_type(), left_child->value_type());
    if (left_to_right_cost <= right_to_left_cost && left_to_right_cost != INT32_MAX) {
      ExprType left_type = left_child->type();
      std::unique_ptr<CastExpr> cast_expr =std::make_unique<CastExpr>(std::move(left_child), right_child->value_type());
      if (left_type == ExprType::VALUE) {
        Value left_val;
        if (OB_FAIL(rc = cast_expr->try_get_value(left_val)))
        {
          LOG_WARN("failed to get value from left child", strrc(rc));
          return rc;
        }
        left_child = std::make_unique<ValueExpr>(left_val);
      } else {
        left_child = std::move(cast_expr);
      }
    } else if (right_to_left_cost < left_to_right_cost && right_to_left_cost != INT32_MAX) {
      ExprType right_type = right_child->type();
      std::unique_ptr<CastExpr> cast_expr = std::make_unique<CastExpr>(std::move(right_child), left_child->value_type());
      if (right_type == ExprType::VALUE) {
        Value right_val;
        if (OB_FAIL(rc = cast_expr->try_get_value(right_val)))
        {
          LOG_WARN("failed to get value from right child", strrc(rc));
          return rc;
        }
        right_child = std::make_unique<ValueExpr>(right_val);
      } else {
        right_child = std::move(cast_expr);
      }

    } else {
      rc = RC::UNSUPPORTED;
      LOG_WARN("unsupported cast from %s to %s", attr_type_to_string(left_child->value_type()), attr_type_to_string(right_child->value_type()));
      return rc;
    }
  }

  // 更新左右子表达式
  expr->left() = std::move(left_child);
  expr->right() = std::move(right_child);

  return rc;
}

RC LogicalPlanGenerator::func_expr_process(FuncExpr *expr, Table *default_table, Db *db, std::unordered_map<std::string, Table *> &table_map)
{
  RC rc = RC::SUCCESS;

  for (int i = 0; i < expr->get_child().size(); i++) {
    std::unique_ptr<Expression> child = std::move(expr->get_child()[i]);
    if (child->type() == ExprType::UNBOUND_FIELD) {
      UnboundFieldExpr* unbound_expr = dynamic_cast<UnboundFieldExpr*>(child.get());
      Table *table;
      if(unbound_expr->table_name() == nullptr || unbound_expr->table_name()[0] == '\0') {
        table = default_table;
      }else {
        table = table_map[unbound_expr->table_name()];
      }
      const FieldMeta *field_meta = table->table_meta().field(unbound_expr->field_name());
      // field不存在，返回错误
      if(field_meta == nullptr) {
        LOG_WARN("field not found: %s", unbound_expr->field_name());
        return RC::SCHEMA_FIELD_MISSING;
      }
      expr->get_child()[i] = std::make_unique<FieldExpr>(table, field_meta);
    }
  }
  return rc;
}


int LogicalPlanGenerator::implicit_cast_cost(AttrType from, AttrType to)
{
  if (from == to) {
    return 0;
  }
  return DataType::type_instance(from)->cast_cost(to);
}

RC LogicalPlanGenerator::create_plan(InsertStmt *insert_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table        *table = insert_stmt->table();
  vector<Value> values(insert_stmt->values(), insert_stmt->values() + insert_stmt->value_amount());

  InsertLogicalOperator *insert_operator = new InsertLogicalOperator(table, values);
  logical_operator.reset(insert_operator);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(DeleteStmt *delete_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table                      *table       = delete_stmt->table();
  FilterStmt                 *filter_stmt = delete_stmt->filter_stmt();
  unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_WRITE));

  unique_ptr<LogicalOperator> predicate_oper;

  std::unordered_map<std::string, Table *> table_map = {};
  RC rc = create_plan(filter_stmt, predicate_oper, table_map);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  unique_ptr<LogicalOperator> delete_oper(new DeleteLogicalOperator(table));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    delete_oper->add_child(std::move(predicate_oper));
  } else {
    delete_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(delete_oper);
  return rc;
}

RC LogicalPlanGenerator::create_plan(ExplainStmt *explain_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  unique_ptr<LogicalOperator> child_oper;

  Stmt *child_stmt = explain_stmt->child();

  RC rc = create(child_stmt, child_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create explain's child operator. rc=%s", strrc(rc));
    return rc;
  }

  logical_operator = unique_ptr<LogicalOperator>(new ExplainLogicalOperator);
  logical_operator->add_child(std::move(child_oper));
  return rc;
}

RC LogicalPlanGenerator::create_group_by_plan(SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  vector<unique_ptr<Expression>> &group_by_expressions = select_stmt->group_by();
  vector<Expression *> aggregate_expressions;
  vector<unique_ptr<Expression>> &query_expressions = select_stmt->query_expressions();
  function<RC(std::unique_ptr<Expression>&)> collector = [&](unique_ptr<Expression> &expr) -> RC {
    RC rc = RC::SUCCESS;
    if(expr == nullptr) {
      LOG_INFO("expr is nullptr when  collecto");
      return  rc;
    }
    if (expr->type() == ExprType::AGGREGATION) {
      expr->set_pos(aggregate_expressions.size() + group_by_expressions.size());
      aggregate_expressions.push_back(expr.get());
    }
    rc = ExpressionIterator::iterate_child_expr(*expr, collector);
    return rc;
  };

  function<RC(std::unique_ptr<Expression>&)> bind_group_by_expr = [&](unique_ptr<Expression> &expr) -> RC {
    RC rc = RC::SUCCESS;
    if(expr == nullptr) {
      LOG_INFO("expr is nullptr when bind grouby expr");
      return  rc;
    }
    for (size_t i = 0; i < group_by_expressions.size(); i++) {
      auto &group_by = group_by_expressions[i];
      if (expr->type() == ExprType::AGGREGATION) {
        break;
      } else if (expr->equal(*group_by)) {
        expr->set_pos(i);
        continue;
      } else {
        rc = ExpressionIterator::iterate_child_expr(*expr, bind_group_by_expr);
      }
    }
    return rc;
  };

 bool found_unbound_column = false;
  function<RC(std::unique_ptr<Expression>&)> find_unbound_column = [&](unique_ptr<Expression> &expr) -> RC {
    RC rc = RC::SUCCESS;
    if(expr == nullptr) {
      LOG_INFO("expr is nullptr when find_unbound_column");
      return  rc;
    }
    if (expr->type() == ExprType::AGGREGATION) {
      // do nothing
    } else if (expr->pos() != -1) {
      // do nothing
    } else if (expr->type() == ExprType::FIELD) {
      found_unbound_column = true;
    }else {
      rc = ExpressionIterator::iterate_child_expr(*expr, find_unbound_column);
    }
    return rc;
  };
  

  for (unique_ptr<Expression> &expression : query_expressions) {
    bind_group_by_expr(expression);
  }

  for (unique_ptr<Expression> &expression : query_expressions) {
    find_unbound_column(expression);
  }

  // collect all aggregate expressions
  for (unique_ptr<Expression> &expression : query_expressions) {
    collector(expression);
  }

  if (group_by_expressions.empty() && aggregate_expressions.empty()) {
    // 既没有group by也没有聚合函数，不需要group by
    return RC::SUCCESS;
  }

  if (found_unbound_column) {
    LOG_WARN("column must appear in the GROUP BY clause or must be part of an aggregate function");
    return RC::INVALID_ARGUMENT;
  }

  // 如果只需要聚合，但是没有group by 语句，需要生成一个空的group by 语句

  auto group_by_oper = make_unique<GroupByLogicalOperator>(std::move(group_by_expressions),
                                                           std::move(aggregate_expressions));
  logical_operator = std::move(group_by_oper);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(UpdateStmt *update_stmt, std::unique_ptr<LogicalOperator> &logical_operator)
{
  Table *table = update_stmt->table();
  std::vector<const FieldMeta*> field_metas = update_stmt->field_metas();
  std::vector<Expression*> values = update_stmt->values();
  FilterStmt                 *filter_stmt = update_stmt->filter_stmt();
  unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_WRITE));

  unique_ptr<LogicalOperator> predicate_oper;

  std::unordered_map<std::string, Table *> table_map = {};
  RC rc = create_plan(filter_stmt, predicate_oper, table_map);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  unique_ptr<LogicalOperator> update_opr(new UpdateLogicalOperator(table, field_metas, values));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    update_opr->add_child(std::move(predicate_oper));
  } else {
    update_opr->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(update_opr);
  return rc;
}