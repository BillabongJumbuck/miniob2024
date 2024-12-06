//
// Created by qjm on 11/12/24.
//

#include "subquery_simplification_rule.h"

#include "physical_plan_generator.h"
#include "logical_plan_generator.h"
#include "common/log/log.h"
#include "sql/expr/expression.h"
#include "common/helper.h"

RC SubQuerySimplificationRule::rewrite(std::unique_ptr<Expression> &expr, bool &change_made)
{
  RC rc = RC::SUCCESS;
  change_made = false;

  if (expr->type() == ExprType::SUBQUERY) {
    SubQueryExpr *subquery_expr = dynamic_cast<SubQueryExpr *>(expr.get());
    if(!subquery_expr->has_result_vector() && !subquery_expr->get_rewiter_failure()) {
      rc = LogicalPlanGenerator::create(subquery_expr->select_stmt(), lo_in_helper);
      if(rc != RC::SUCCESS) {
        LOG_WARN("failed to create logical plan for subquery");
        return rc;
      }
      if(use_extra_table_map) {
        LOG_INFO("use extra_ table _ map");
        subquery_expr->set_rewrite_failure();
        return RC::SUCCESS;
      }
      PhysicalPlanGenerator::create(*lo_in_helper, po_in_helper);
      LOG_INFO("subquery_physical_operator type: %d", po_in_helper->type());
      Tuple *tuple         = nullptr;
      po_in_helper->open(nullptr);
      while (true) {
        rc = po_in_helper->next();
        if (RC::SUCCESS != rc) {
          if(rc == RC::RECORD_EOF) {
            LOG_INFO("subquery rewite succeed!");
            subquery_expr->set_has_result();
            change_made = true;
          }else {
            LOG_INFO("subquery rewite failed!");
            subquery_expr->set_rewrite_failure();
          }
          po_in_helper->close();
          break;
        }
        tuple = po_in_helper->current_tuple();
        Value value;
        tuple->cell_at(0, value);
        subquery_expr->add_result(value);
      }
    }
  }

  return RC::SUCCESS;
}
