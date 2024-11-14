//
// Created by qjm on 10/14/24.
//

#include "sql/operator/update_logical_operator.h"

 UpdateLogicalOperator::UpdateLogicalOperator(Table *table, const FieldMeta *field_meta, Expression* value)
 {
   table_ = table;
   field_meta_ = field_meta;
   value_ = value;
 }

