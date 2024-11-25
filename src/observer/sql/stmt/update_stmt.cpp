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

UpdateStmt::UpdateStmt(Table *table, const FieldMeta *field_meta, Expression* value,  FilterStmt *filter_stmt)
{
  table_ = table;
  field_meta_ = field_meta;
  value_ = value;
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
  const char *attr_name = update.attribute_name.c_str();
  if(common::is_blank(attr_name)) {
    LOG_WARN("invalid argument. db=%p, attr_name=%p", db, attr_name);
    return RC::INVALID_ARGUMENT;
  }

  // 检查属性名是否存在
  const FieldMeta *field_meta = table->table_meta().field(attr_name);
  if(field_meta == nullptr) {
    LOG_WARN("update table error :no such attribute. db=%s, attr_name=%s", db->name(), attr_name);
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  if(update.value -> type() == ExprType::SUBQUERY) {
    SubQueryExpr *subquery_expr = dynamic_cast<SubQueryExpr *>(update.value);
    RC rc = subquery_expr->Create_stmt(db);
    if(OB_FAIL(rc)) {
      LOG_WARN("can not create stmt in subquery expr in update");
      return rc;
    }
  }

  Value value;
  // 检查类型是否一致
  if(update.value -> type() == ExprType::VALUE) {
    value = dynamic_cast<ValueExpr *>(update.value)->get_value();
    // patch:转换日期类型
    if(field_meta->type() == AttrType::DATES) {
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
    if( !value.is_null() &&  value.attr_type() != field_meta->type()) {
      LOG_WARN("invalid argument. db=%p, attr_name=%p", db, attr_name);
      return RC::INVALID_ARGUMENT;
    }
  }

  // // 检查是否将null插入非null属性
  // if(value.is_null() && !field_meta->is_nullable()) {
  //   LOG_WARN("invalid argument. db=%p, attr_name=%p", db, attr_name);
  //   return RC::INVALID_ARGUMENT;
  // }

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

  if(update.value -> type() == ExprType::SUBQUERY)
    stmt = new UpdateStmt(table, field_meta, update.value, filter_stmt);
  else if(update.value -> type() == ExprType::VALUE)
    stmt = new UpdateStmt(table, field_meta, new ValueExpr(value), filter_stmt);
  return RC::SUCCESS;
}
