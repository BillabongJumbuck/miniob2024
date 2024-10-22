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

#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include <unordered_map>
#include <vector>
#include <storage/db/db.h>

class Db;
class Table;
class FieldMeta;

/**
 * @brief Filter/谓词/过滤语句
 * @ingroup Statement
 */
class FilterStmt
{
public:
  FilterStmt() = default;
  ~FilterStmt() = default;

public:
  ConjunctionExpr *conjunction() const
  {
    return conjunction_;
  }
  Table *table() const
  {
    return default_table_;
  }
  Db *db() const
  {
    return db_;
  }

public:
  RC create(ConjunctionExpr *conjunction_expr, Table *default_table, Db *db);
  RC create(ComparisonExpr *comparison_expr, Table *default_table, Db *db);

private:
  ConjunctionExpr *conjunction_ = nullptr;
  Table *default_table_ = nullptr;
  Db *db_ = nullptr;
};
