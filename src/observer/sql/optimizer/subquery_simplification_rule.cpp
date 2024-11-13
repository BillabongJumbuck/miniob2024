//
// Created by qjm on 11/12/24.
//

#include "subquery_simplification_rule.h"

#include "physical_plan_generator.h"
#include "common/log/log.h"
#include "sql/expr/expression.h"

RC SubQuerySimplificationRule::rewrite(std::unique_ptr<Expression> &expr, bool &change_made)
{
  RC rc = RC::SUCCESS;
  change_made = false;

  if (expr->type() == ExprType::SUBQUERY) {
    SubQueryExpr *subquery_expr = dynamic_cast<SubQueryExpr *>(expr.get());
    if(!subquery_expr->has_result_vector()) {
      subquery_expr->LogicalPlanGenerate();
      std::unique_ptr<PhysicalOperator> subquery_physical_operator = nullptr;
      PhysicalPlanGenerator::create(*subquery_expr->logical_plan(), subquery_physical_operator);
      LOG_INFO("subquery_physical_operator type: %d", subquery_physical_operator->type());
      Tuple *tuple         = nullptr;
      subquery_physical_operator->open(nullptr);
      while (RC::SUCCESS == (rc = subquery_physical_operator->next())) {
        tuple = subquery_physical_operator->current_tuple();
        Value value;
        tuple->cell_at(0, value);
        subquery_expr->add_result(value);
      }
      subquery_physical_operator->close();
      subquery_expr->set_has_result();
      change_made = true;
    }
  }

  return RC::SUCCESS;
}
