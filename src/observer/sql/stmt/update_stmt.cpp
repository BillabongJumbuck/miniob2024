/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/5/22.
//

#include "common/log/log.h"
#include "common/types.h"
#include "common/lang/string.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/update_stmt.h"
#include "storage/db/db.h"

#include <storage/table/table.h>

UpdateStmt::UpdateStmt(Table *table, std::vector<const FieldMeta*> &field_metas, std::vector<Expression*> &values,  FilterStmt *filter_stmt)
{
  table_ = table;
  field_metas_ = field_metas;
  values_ = values;
  filter_stmt_ = filter_stmt;
}

UpdateStmt::~UpdateStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
  const char *table_name = update.relation_name.c_str();
  if (nullptr == db || nullptr == table_name) {
    LOG_WARN("invalid argument. db=%p, table_name=%p",
        db, table_name);
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  // 检查属性名是否为空
  std::vector<const FieldMeta*> field_metas;
  for(auto &attr_name : update.attribute_names) {
    auto attr_name_cstring = attr_name.c_str();
    if(common::is_blank(attr_name_cstring)) {
      LOG_WARN("invalid argument. db=%p, attr_name=%p", db, attr_name);
      return RC::INVALID_ARGUMENT;
    }

    // 检查属性名是否存在
    const FieldMeta *field_meta = table->table_meta().field(attr_name_cstring);
    if(field_meta == nullptr) {
      LOG_WARN("update table error :no such attribute. db=%s, attr_name=%s", db->name(), attr_name);
      return RC::SCHEMA_FIELD_NOT_EXIST;
    }

    field_metas.push_back(field_meta);
  }


  std::vector<Expression*> values;
  for(size_t i = 0; i < update.values.size(); i++) {
    Expression *value_expr = update.values[i];
    // 处理子查询
    if(value_expr -> type() == ExprType::SUBQUERY) {
      SubQueryExpr *subquery_expr = dynamic_cast<SubQueryExpr *>(value_expr);
      RC rc = subquery_expr->Create_stmt(db);
      if(OB_FAIL(rc)) {
        LOG_WARN("can not create stmt in subquery expr in update");
        return rc;
      }
      values.push_back(value_expr);
      continue;
    }
    // 类型检查
    Value value;
    // 检查类型是否一致
    if(value_expr -> type() == ExprType::VALUE) {
      value = dynamic_cast<ValueExpr *>(value_expr)->get_value();
      // patch:转换日期类型
      if(field_metas[i]->type() == AttrType::DATES) {
        // 从 'yyyy-mm-dd' 找出 y, m ,d 三个变量
        const char* temp = value.data();
        int y, m ,d;
        // 提取年份
        y = (temp[0] - '0') * 1000 + (temp[1] - '0') * 100 + (temp[2] - '0') * 10 + (temp[3] - '0');

        // 提取月份
        m = (temp[5] - '0') * 10 + (temp[6] - '0');

        // 提取日期
        d = (temp[8] - '0') * 10 + (temp[9] - '0');
        value.set_date(y,m,d);
      }
      if( !value.is_null() &&  value.attr_type() != field_metas[i]->type()) {
        LOG_WARN("invalid argument. db=%p, attr_name=%p", db, field_metas[i]->name());
        return RC::INVALID_ARGUMENT;
      }

      // 检查是否将null插入非null属性
      if(value.is_null() && !field_metas[i]->is_nullable()) {
        LOG_WARN("invalid argument. db=%p, attr_name=%p", db, field_metas[i]->name());
        return RC::INVALID_ARGUMENT;
      }
      values.push_back(new ValueExpr(value));
    }
  }

  // create filter statement in `where` statement
  FilterStmt *filter_stmt;
  if(update.conditions == nullptr) {
    filter_stmt = nullptr;
  }else {
    filter_stmt = new FilterStmt();
    if(update.conditions->type() == ExprType::COMPARISON) {
      RC rc = filter_stmt->create(dynamic_cast<ComparisonExpr *>(update.conditions), table, db);
      if (rc != RC::SUCCESS) {
        LOG_WARN("cannot construct filter stmt");
        return rc;
      }
    }else if ( update.conditions->type() == ExprType::CONJUNCTION){
      RC rc = filter_stmt->create(dynamic_cast<ConjunctionExpr *>(update.conditions), table, db);
      if (rc != RC::SUCCESS) {
        LOG_WARN("cannot construct filter stmt");
        return rc;
      }
    }else {
      LOG_WARN("invalid condition type");
      return RC::INTERNAL;
    }
  }

  stmt = new UpdateStmt(table, field_metas, values, filter_stmt);
  return RC::SUCCESS;
}
