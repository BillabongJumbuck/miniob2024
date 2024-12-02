//
// Created by qjm on 10/14/24.
//

#include "sql/operator/update_physical_oprator.h"

#include <utility>
#include "common/log/log.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include "sql/optimizer/physical_plan_generator.h"

 UpdatePhysicalOperator::UpdatePhysicalOperator(
    Table *table, std::vector<const FieldMeta *> &field_metas, std::vector<Expression *> &values)
{
  table_ = table;
  field_metas_ = field_metas;
  values_ = values;
}


RC UpdatePhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  std::unique_ptr<PhysicalOperator> &child = children_[0];

  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  while (OB_SUCC(rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record   &record    = row_tuple->record();
    records_.emplace_back(std::move(record));
  }

  child->close();

  if(records_.empty()) {
    LOG_WARN("no record to update!");
    return RC::SUCCESS;
  }

  if(field_metas_.size() != values_.size()) {
    LOG_WARN("field_metas size is not equal to values size!");
    return RC::INVALID_ARGUMENT;
  }

  std::vector<Value> values;
  for(size_t i = 0; i< field_metas_.size(); i++) {
    Value value;
    // 求出value的实际值
    if(values_[i] -> type() == ExprType::VALUE) {
      ValueExpr *expr = dynamic_cast<ValueExpr *>(values_[i]);
      value = expr->get_value();
    }else if(values_[i] -> type() == ExprType::SUBQUERY) {
      SubQueryExpr *subquery_expr = dynamic_cast<SubQueryExpr *>(values_[i]);
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

      if(subquery_expr->get_result_vector().size() > 1) {
        LOG_WARN("subquery result size is not 1!");
        return RC::INVALID_ARGUMENT;
      }else if(subquery_expr->get_result_vector().empty()) {
        if(!field_metas_[i]->is_nullable()) {
          LOG_WARN("subquery result is empty but field is not nullable!");
          return RC::SUCCESS;
        }
        value.set_null();
      }else  {
        value = subquery_expr->get_result_vector()[0];
      }
    }else {
      LOG_WARN("value type is not VALUE or SUBQUERY!");
      return RC::INVALID_ARGUMENT;
    }

    // 检查是否将null插入非null属性
    if(value.is_null() && !field_metas_[i]->is_nullable()) {
      LOG_WARN("insert null to a not nullable field!");
      return RC::INVALID_ARGUMENT;
    }

    values.push_back(value);
  }


  // 先收集记录再Update
  // 记录的有效性由事务来保证，如果事务不保证Update的有效性，那说明此事务类型不支持并发控制，比如VacuousTrx
  for (Record &record : records_) {
    rc = trx -> update_record(table_, record, values, field_metas_);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to update record: %s", strrc(rc));
      return rc;
    }
  }

  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next() { return RC::RECORD_EOF; }

RC UpdatePhysicalOperator::close() { return RC::SUCCESS; }


