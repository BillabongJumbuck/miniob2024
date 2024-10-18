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
  UpdateLogicalOperator(Table *table, const FieldMeta *field_meta, Value value);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override {return LogicalOperatorType::UPDATE;}
  Table *table() const {return table_;}
  const FieldMeta *field_meta() const {return field_meta_;}
  Value value() const {return value_;}

private:
  Table *table_ = nullptr;
  const FieldMeta *field_meta_ = nullptr;
  Value value_;
};



#endif //UPDATE_LOGICAL_OPERATOR_H
