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

UpdateStmt::UpdateStmt(Table *table, const FieldMeta *field_meta, Value value,  FilterStmt *filter_stmt)
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

  // 检查类型是否一致
  Value value = update.value;
  if(value.attr_type() != field_meta->type()) {
    LOG_WARN("invalid argument. db=%p, attr_name=%p", db, attr_name);
    return RC::INVALID_ARGUMENT;
  }

  // 创建条件查询语句
  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(std::string(table_name), table));

  FilterStmt *filter_stmt = nullptr;
  RC          rc          = FilterStmt::create(
      db, table, &table_map, update.conditions.data(), static_cast<int>(update.conditions.size()), filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create filter statement. rc=%d:%s", rc, strrc(rc));
    return rc;
  }

  stmt = new UpdateStmt(table, field_meta, value, filter_stmt);
  return RC::SUCCESS;
}
