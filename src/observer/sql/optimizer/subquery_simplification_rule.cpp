//
// Created by qjm on 11/12/24.
//

#include "subquery_simplification_rule.h"
#include "common/log/log.h"
#include "sql/expr/expression.h"

RC SubQuerySimplificationRule::rewrite(std::unique_ptr<Expression> &expr, bool &change_made)
{
  // RC rc = RC::SUCCESS;
  change_made = false;

  if (expr->type() == ExprType::SUBQUERY) {
    SubQueryExpr *subquery_expr = dynamic_cast<SubQueryExpr *>(expr.get());

    subquery_expr->LogicalPlanGenerate();

  }

  return RC::UNIMPLEMENTED;
}
