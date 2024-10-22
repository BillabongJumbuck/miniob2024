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

#include "sql/stmt/filter_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

RC FilterStmt::create(ConjunctionExpr *conjunction_expr, Table* defau_table, Db *db)
{
  RC rc = RC::SUCCESS;

  conjunction_ = conjunction_expr;
  default_table_ = defau_table;
  db_ = db;

  return rc;
}

RC FilterStmt::create(ComparisonExpr *comparison_expr, Table* defau_table, Db *db)
{
  RC rc = RC::SUCCESS;
  Value tempvalue1(1);
  Value tempvalue2(2);
  ComparisonExpr *temp = new ComparisonExpr(CompOp::EQUAL_TO, new ValueExpr(tempvalue1), new ValueExpr(tempvalue2));
  ConjunctionExpr *conjunction_expr = new ConjunctionExpr(ConjunctionExpr::Type::OR, comparison_expr, temp);
  conjunction_ = conjunction_expr;
  default_table_ = defau_table;
  db_ = db;

  return rc;
}

