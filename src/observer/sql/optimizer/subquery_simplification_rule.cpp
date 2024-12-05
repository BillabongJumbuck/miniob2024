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
    if(!subquery_expr->has_result_vector() && !subquery_expr->get_rewiter_failure()) {
      subquery_expr->LogicalPlanGenerate();
      std::unique_ptr<PhysicalOperator> subquery_physical_operator = nullptr;
      PhysicalPlanGenerator::create(*subquery_expr->logical_plan(), subquery_physical_operator);
      LOG_INFO("subquery_physical_operator type: %d", subquery_physical_operator->type());
      Tuple *tuple         = nullptr;
      subquery_physical_operator->open(nullptr);
      while (true) {
        rc = subquery_physical_operator->next();
        if (RC::SUCCESS != rc) {
          if(rc == RC::RECORD_EOF) {
            LOG_INFO("subquery rewite succeed!");
            subquery_expr->set_has_result();
            change_made = true;
          }else {
            LOG_INFO("subquery rewite failed!");
            subquery_expr->set_rewrite_failure();
          }
          subquery_physical_operator->close();
          break;
        }
        tuple = subquery_physical_operator->current_tuple();
        Value value;
        tuple->cell_at(0, value);
        subquery_expr->add_result(value);
      }
    }
  }

  return RC::SUCCESS;
}
