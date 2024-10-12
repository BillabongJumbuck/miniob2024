//
// Created by qjm on 10/12/24.
//

#ifndef DATE_TYPE_H
#define DATE_TYPE_H

#include "common/type/data_type.h"

class DateType : public DataType
{
public:
  DateType() : DataType(AttrType::DATES) {}
  virtual ~DateType() = default;

  int compare(const Value &left, const Value &right) const override;

  RC to_string(const Value &val, string &result) const override;

};
#endif //DATE_TYPE_H
