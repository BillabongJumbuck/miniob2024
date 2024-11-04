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
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/float_type.h"
#include "common/value.h"
#include "common/lang/limits.h"
#include "common/value.h"
#include <cmath>

int FloatType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::FLOATS, "left type is not integer");
  ASSERT(right.attr_type() == AttrType::INTS || right.attr_type() == AttrType::FLOATS, "right type is not numeric");
  float left_val  = left.get_float();
  float right_val = right.get_float();
  return common::compare_float((void *)&left_val, (void *)&right_val);
}

RC FloatType::add(const Value &left, const Value &right, Value &result) const
{
  result.set_float(left.get_float() + right.get_float());
  return RC::SUCCESS;
}
RC FloatType::subtract(const Value &left, const Value &right, Value &result) const
{
  result.set_float(left.get_float() - right.get_float());
  return RC::SUCCESS;
}
RC FloatType::multiply(const Value &left, const Value &right, Value &result) const
{
  result.set_float(left.get_float() * right.get_float());
  return RC::SUCCESS;
}

RC FloatType::divide(const Value &left, const Value &right, Value &result) const
{
  if (right.get_float() > -EPSILON && right.get_float() < EPSILON) {
    // NOTE:
    // 设置为浮点数最大值是不正确的。通常的做法是设置为NULL，但是当前的miniob没有NULL概念，所以这里设置为浮点数最大值。
    result.set_float(numeric_limits<float>::max());
  } else {
    result.set_float(left.get_float() / right.get_float());
  }
  return RC::SUCCESS;
}

RC FloatType::negative(const Value &val, Value &result) const
{
  result.set_float(-val.get_float());
  return RC::SUCCESS;
}

RC FloatType::set_value_from_str(Value &val, const string &data) const
{
  RC                rc = RC::SUCCESS;
  stringstream deserialize_stream;
  deserialize_stream.clear();
  deserialize_stream.str(data);

  float float_value;
  deserialize_stream >> float_value;
  if (!deserialize_stream || !deserialize_stream.eof()) {
    rc = RC::SCHEMA_FIELD_TYPE_MISMATCH;
  } else {
    val.set_float(float_value);
  }
  return rc;
}

RC FloatType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << common::double_to_str(val.value_.float_value_);
  result = ss.str();
  return RC::SUCCESS;
}


RC FloatType::l2_distance(const Value &left, const Value &right, Value &result) const {
  Value real_left = left;
  Value real_right = right;

  if (left.attr_type_ != AttrType::VECTORS) {
    Value::cast_to(left, AttrType::VECTORS, real_left);
  }
  if (right.attr_type_ != AttrType::VECTORS) {
    Value::cast_to(right, AttrType::VECTORS, real_right);
  }

  const size_t lnum_elements = real_left.length_;
  const size_t rnum_elements = real_right.length_;

  if (lnum_elements != rnum_elements) {
    LOG_WARN("Vector sizes do not match: left size = %zu, right size = %zu",
             lnum_elements, rnum_elements);
    return RC::INVALID_ARGUMENT;
  }

  const float* lvec = reinterpret_cast<float*>(real_left.value_.pointer_value_);
  const float* rvec = reinterpret_cast<float*>(real_right.value_.pointer_value_);

  float sum = 0.0f;
  for (size_t i = 0; i < lnum_elements; ++i) {
    float diff = lvec[i] - rvec[i];
    sum += diff * diff;
  }

  float distance = std::sqrt(sum);
  // 保留两位小数
  distance = std::round(distance * 100.0f) / 100.0f;
  result.set_float(distance);  // 假设 result 支持 set_float

  return RC::SUCCESS;
}


RC FloatType::cosine_distance(const Value &left, const Value &right, Value &result) const {
  Value real_left = left;
  Value real_right = right;

  if (left.attr_type_ != AttrType::VECTORS) {
    Value::cast_to(left, AttrType::VECTORS, real_left);
  }
  if (right.attr_type_ != AttrType::VECTORS) {
    Value::cast_to(right, AttrType::VECTORS, real_right);
  }

  const size_t lnum_elements = real_left.length_;
  const size_t rnum_elements = real_right.length_;

  if (lnum_elements != rnum_elements) {
    LOG_WARN("Vector sizes do not match: left size = %zu, right size = %zu",
             lnum_elements, rnum_elements);
    return RC::INVALID_ARGUMENT;
  }

  const float* lvec = reinterpret_cast<float*>(real_left.value_.pointer_value_);
  const float* rvec = reinterpret_cast<float*>(real_right.value_.pointer_value_);

  float dot_product = 0.0f, norm_left = 0.0f, norm_right = 0.0f;

  for (size_t i = 0; i < lnum_elements; ++i) {
    dot_product += lvec[i] * rvec[i];
    norm_left += lvec[i] * lvec[i];
    norm_right += rvec[i] * rvec[i];
  }

  float cosine_similarity = dot_product / (std::sqrt(norm_left) * std::sqrt(norm_right));
  float cosine_distance = 1.0f - cosine_similarity;

  // 保留两位小数
  cosine_distance = std::round(cosine_distance * 100.0f) / 100.0f;

  result.set_float(cosine_distance);  // 假设 result 支持 set_float

  return RC::SUCCESS;
}


RC FloatType::inner_product(const Value &left, const Value &right, Value &result) const {
  Value real_left = left;
  Value real_right = right;

  if (left.attr_type_ != AttrType::VECTORS) {
    Value::cast_to(left, AttrType::VECTORS, real_left);
  }
  if (right.attr_type_ != AttrType::VECTORS) {
    Value::cast_to(right, AttrType::VECTORS, real_right);
  }

  const size_t lnum_elements = real_left.length_;
  const size_t rnum_elements = real_right.length_;

  if (lnum_elements != rnum_elements) {
    LOG_WARN("Vector sizes do not match: left size = %zu, right size = %zu",
             lnum_elements, rnum_elements);
    return RC::INVALID_ARGUMENT;
  }

  const float* lvec = reinterpret_cast<float*>(real_left.value_.pointer_value_);
  const float* rvec = reinterpret_cast<float*>(real_right.value_.pointer_value_);

  float dot_product = 0.0f;
  for (size_t i = 0; i < lnum_elements; ++i) {
    dot_product += lvec[i] * rvec[i];
  }

  // 保留两位小数
  dot_product = std::round(dot_product * 100.0f) / 100.0f;

  result.set_float(dot_product);  // 假设 result 支持 set_float

  return RC::SUCCESS;
}
