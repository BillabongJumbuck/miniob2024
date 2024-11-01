/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/lang/comparator.h"
#include "common/log/log.h"
#include "common/type/char_type.h"
#include "common/time/datetime.h"

#include "date_type.h"
#include "vector_type.h"
#include "common/value.h"

int CharType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::CHARS && right.attr_type() == AttrType::CHARS, "invalid type");
  return common::compare_string(
      (void *)left.value_.pointer_value_, left.length_, (void *)right.value_.pointer_value_, right.length_);
}

RC CharType::set_value_from_str(Value &val, const string &data) const
{
  val.set_string(data.c_str());
  return RC::SUCCESS;
}

RC CharType::cast_to(const Value &val, AttrType type, Value &result) const
{
  switch (type) {
    case AttrType::DATES: {
      result.attr_type_ = AttrType::DATES;
      int y,m,d;
      if(sscanf(val.value_.pointer_value_, "%d-%d-%d", &y, &m, &d) != 3) {
        LOG_WARN("invalid date format: %s", val.value_.pointer_value_);
        return RC::INVALID_ARGUMENT;
      }
      bool check_ret = common::check_date(y, m, d);
      if(!check_ret) {
        LOG_WARN("invalid date format: %s", val.value_.pointer_value_);
        return RC::INVALID_ARGUMENT;
      }
      result.set_date(y, m, d);
    }break;

    case AttrType::VECTORS: {
      result.attr_type_ = AttrType::VECTORS;
      std::string vector_str(val.value_.pointer_value_);
      std::vector<float> vector_values;
      // 去掉开头和结尾的方括号
      if (vector_str.front() == '[') {
          vector_str.erase(0, 1);
      }
      if (vector_str.back() == ']') {
          vector_str.pop_back();
      }

      // 分割字符串并解析浮点数
      std::stringstream ss(vector_str);
      std::string token;
      while (std::getline(ss, token, ',')) {
          try {
              float value = std::stof(token); // 转换为浮点数
              vector_values.push_back(value); // 存入向量
          } catch (const std::exception &ex) {
              LOG_WARN("invalid float format: %s", token.c_str());
              return RC::INVALID_ARGUMENT;
          }
      }

      // 检查向量维度是否超过最大限制
      // if (vector_values.size() > 16000) { // 假设最大支持 16000 维
      //     LOG_WARN("vector exceeds maximum dimension: %lu", vector_values.size());
      //     return RC::INVALID_ARGUMENT;
      // }
      result.set_vector(vector_values.data(), vector_values.size());
    } break;

    default: return RC::UNIMPLEMENTED;
  }
  return RC::SUCCESS;
}

int CharType::cast_cost(AttrType type)
{
  if (type == AttrType::CHARS) {
    return 0;
  }
  if (type == AttrType::DATES) {
    return 1;
  }
  return INT32_MAX;
}

RC CharType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << val.value_.pointer_value_;
  result = ss.str();
  return RC::SUCCESS;
}