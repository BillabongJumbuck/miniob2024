//
// Created by qjm on 10/14/24.
//

#ifndef UPDATE_LOGICAL_OPERATOR_H
#define UPDATE_LOGICAL_OPERATOR_H

#include "sql/operator/logical_operator.h"

/**
 * @brief 逻辑算子，用于执行update语句
 * @ingroup LogicalOperator
 */

class UpdateLogicalOperator : public LogicalOperator {
public:
  UpdateLogicalOperator(Table *table, std::vector<const FieldMeta*> &field_metas, std::vector<Expression*> &values);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override {return LogicalOperatorType::UPDATE;}
  Table *table() const {return table_;}
  std::vector<const FieldMeta*> &field_metas() {return field_metas_;}
  std::vector<Expression*> &values() {return values_;}

private:
  Table *table_ = nullptr;
  std::vector<const FieldMeta*> field_metas_;
  std::vector<Expression*> values_;
};



#endif //UPDATE_LOGICAL_OPERATOR_H
