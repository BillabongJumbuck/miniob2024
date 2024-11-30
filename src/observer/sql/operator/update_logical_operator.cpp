//
// Created by qjm on 10/14/24.
//

#include "sql/operator/update_logical_operator.h"

UpdateLogicalOperator::UpdateLogicalOperator(
    Table *table, std::vector<const FieldMeta *> &field_metas, std::vector<Expression *> &values)
{
  table_ = table;
  field_metas_ = field_metas;
  values_ = values;
}


