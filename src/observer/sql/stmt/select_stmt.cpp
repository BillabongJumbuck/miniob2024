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
// Created by Wangyunlai on 2022/6/6.
//

#include "sql/stmt/select_stmt.h"
#include "sql/expr/expression.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "sql/parser/expression_binder.h"

using namespace std;
using namespace common;

SelectStmt::~SelectStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

RC SelectStmt::create(Db *db, SelectSqlNode &select_sql, Stmt *&stmt)
{
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  BinderContext binder_context;

  // collect tables in `from` statement
  vector<Table *>                tables;
  unordered_map<string, Table *> table_map;
  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    for(size_t j=0; j < select_sql.relations[i].tables.size();j++) {
      const char *table_name = select_sql.relations[i].tables[j].table_name.c_str();
      if(table_name == nullptr){
        LOG_WARN("invalid argument. table name is null. index=%d", j);
        return RC::INVALID_ARGUMENT;
      }
      Table *table = db->find_table(table_name);
      if (nullptr == table) {
        LOG_WARN("no such table. db=%s, table_name=%s", db->name(), select_sql.relations[i].tables[j].table_name.c_str());
        return RC::SCHEMA_TABLE_NOT_EXIST;
      }

      // 查找是否已经存在
      if (table_map.find(table->name()) == table_map.end()) {
        binder_context.add_table(table);
        tables.push_back(table);
        table_map.insert({table_name, table});
        if(!select_sql.relations[i].tables[j].alias.empty()) {
          const char *alias = select_sql.relations[i].tables[j].alias.c_str();
          binder_context.add_table(table, alias);
          table_map.insert({alias, table});
        }
      }
    }
  }

  // collect query fields in `select` statement
  vector<unique_ptr<Expression>> bound_expressions;
  ExpressionBinder expression_binder(binder_context);

  for (unique_ptr<Expression> &expression : select_sql.expressions) {
    RC rc = expression_binder.bind_expression(expression, bound_expressions);
    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }

  vector<unique_ptr<Expression>> group_by_expressions;
  for (unique_ptr<Expression> &expression : select_sql.group_by) {
    RC rc = expression_binder.bind_expression(expression, group_by_expressions);
    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }
  ConjunctionExpr *on_conditions = new ConjunctionExpr(ConjunctionExpr::Type::AND, new ValueExpr(Value(true)), new ValueExpr(Value(true)));
  for(auto vec : select_sql.relations) {
    if(!vec.conditions.empty()){
      for(auto cond : vec.conditions) {
        Expression* temp = on_conditions;
        on_conditions = new ConjunctionExpr(ConjunctionExpr::Type::AND, cond, temp);
      }
    }
  }

  // create filter statement in `where` statement
  FilterStmt *filter_stmt = new FilterStmt();
  if(select_sql.conditions != nullptr) {
    Expression* temp = on_conditions;
    on_conditions = new ConjunctionExpr(ConjunctionExpr::Type::AND, select_sql.conditions, temp);
  }

  filter_stmt->create(on_conditions, default_table, db);


  // everything alright
  SelectStmt *select_stmt = new SelectStmt();

  select_stmt->tables_.swap(tables);
  select_stmt->query_expressions_.swap(bound_expressions);
  select_stmt->filter_stmt_ = filter_stmt;
  select_stmt->table_map_ = table_map;
  select_stmt->group_by_.swap(group_by_expressions);
  stmt                      = select_stmt;
  return RC::SUCCESS;
}

bool SelectStmt::add_table_map(std::unordered_map<std::string, Table *> &table_map){
  bool res = false;
  // 添加表映射，先检查是否重复
  for(auto table : table_map) {
    if(table_map_.find(table.first) == table_map_.end()) {
      table_map_.insert({table.first, table.second});
      res = true;
    }
  }
  return res;
}

