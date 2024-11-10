//
// Created by qjm on 10/24/24.
//

#include "null_type.h"

#include <common/value.h>

RC NullType::to_string(const Value &val, string &result) const
{
  result = "NULL";
  return RC::SUCCESS;
}

int NullType::compare(const Value &left_val, const Value &right_val) const
{
  return left_val.is_null() && right_val.is_null() ? 0 : -1;
}
