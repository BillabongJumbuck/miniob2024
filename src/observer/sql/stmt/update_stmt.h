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

#pragma once

#include "common/rc.h"
#include "sql/stmt/stmt.h"
#include "sql/parser/parse_defs.h"

class Table;
class FilterStmt;
class FieldMeta;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt
{
public:
  UpdateStmt(Table *table, std::vector<const FieldMeta*> &field_metas, std::vector<Expression*> &values,  FilterStmt *filter_stmt);
  ~UpdateStmt() override;

  Table *table() const { return table_; }
  std::vector<const FieldMeta*> & field_metas() { return field_metas_; }
  std::vector<Expression*> & values() { return values_; }
  FilterStmt *filter_stmt() const { return filter_stmt_; }

  StmtType type() const override { return StmtType::UPDATE; }

  static RC create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt);

private:
  Table *table_ = nullptr;
  std::vector<const FieldMeta*> field_metas_;
  std::vector<Expression*> values_;
  FilterStmt *filter_stmt_ = nullptr;
};
