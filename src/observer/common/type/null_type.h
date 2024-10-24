//
// Created by qjm on 10/24/24.
//

#ifndef NULL_TYPE_H
#define NULL_TYPE_H

#include "common/type/data_type.h"

/**
 * @brief NUll类型
 * @ingroup DataType
 */
class NullType : public DataType
{
public:
  NullType() : DataType(AttrType::NULLS) {}
  virtual ~NullType() = default;

  int compare(const Value &left, const Value &right) const override;

  RC to_string(const Value &val, string &result) const override;

};



#endif //NULL_TYPE_H
