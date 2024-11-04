#include <iomanip>

#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/vector_type.h"
#include "common/value.h"
#include <cmath>

int VectorType::compare(const Value &left, const Value &right) const {
  Value real_left = left;
  Value real_right = right;
  if (left.attr_type_!=AttrType::VECTORS) {
    Value::cast_to(left,AttrType::VECTORS, real_left);
  }
  if (right.attr_type_!=AttrType::VECTORS) {
    Value::cast_to(right,AttrType::VECTORS, real_right);
  }
  // 获取向量数据
  const size_t lnum_elements = real_left.length_;
  const size_t rnum_elements = real_right.length_;

  //检查向量长度是否相等
  if (lnum_elements != rnum_elements) {
    LOG_WARN("Vector sizes do not match: left size = %zu, right size = %zu",
             lnum_elements, rnum_elements);
    return -2;
  }

  // 使用指针构造出向量
  const float* lvec = reinterpret_cast<float*>(real_left.value_.pointer_value_);
  const float* rvec = reinterpret_cast<float*>(real_right.value_.pointer_value_);

  // 逐个元素进行比较
  for (size_t i = 0; i < lnum_elements; ++i) {
    if (lvec[i] < rvec[i]) {
      return -1; // left 小于 right
    } else if (lvec[i] > rvec[i]) {
      return 1; // left 大于 right
    }
  }

  return 0; // 两个向量相等
}


RC VectorType::add(const Value &left, const Value &right, Value &result) const {
    Value real_left = left;
    Value real_right = right;
    if (left.attr_type_!=AttrType::VECTORS) {
      Value::cast_to(left,AttrType::VECTORS, real_left);
    }
    if (right.attr_type_!=AttrType::VECTORS) {
      Value::cast_to(right,AttrType::VECTORS, real_right);
    }
    // 获取向量数据
    const size_t lnum_elements = real_left.length_;
    const size_t rnum_elements = real_right.length_;

    // 检查向量长度是否相等
    if (lnum_elements != rnum_elements) {
        LOG_WARN("Vector sizes do not match: left size = %zu, right size = %zu",
                 lnum_elements, rnum_elements);
        return RC::INVALID_ARGUMENT;
    }

    // 使用指针构造出向量
    const float* lvec = reinterpret_cast<float*>(real_left.value_.pointer_value_);
    const float* rvec = reinterpret_cast<float*>(real_right.value_.pointer_value_);

    std::vector<float> result_vector(lnum_elements);

    // 逐元素相加
    for (size_t i = 0; i < lnum_elements; ++i) {
      result_vector[i] = lvec[i] + rvec[i];
    }

    // 设置结果值
    result.set_vector(result_vector.data(), lnum_elements);

    return RC::SUCCESS;
}

// 减法：逐元素相减
RC VectorType::subtract(const Value &left, const Value &right, Value &result) const {
  Value real_left = left;
  Value real_right = right;
  if (left.attr_type_!=AttrType::VECTORS) {
    Value::cast_to(left,AttrType::VECTORS, real_left);
  }
  if (right.attr_type_!=AttrType::VECTORS) {
    Value::cast_to(right,AttrType::VECTORS, real_right);
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

    std::vector<float> result_vector(lnum_elements);

    for (size_t i = 0; i < lnum_elements; ++i) {
        result_vector[i] = lvec[i] - rvec[i];
    }

    result.set_vector(result_vector.data(), lnum_elements);

    return RC::SUCCESS;
}

// 乘法：逐元素相乘
RC VectorType::multiply(const Value &left, const Value &right, Value &result) const {
  Value real_left = left;
  Value real_right = right;
  if (left.attr_type_!=AttrType::VECTORS) {
    Value::cast_to(left,AttrType::VECTORS, real_left);
  }
  if (right.attr_type_!=AttrType::VECTORS) {
    Value::cast_to(right,AttrType::VECTORS, real_right);
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

    std::vector<float> result_vector(lnum_elements);

    for (size_t i = 0; i < lnum_elements; ++i) {
        result_vector[i] = lvec[i] * rvec[i];
    }

    result.set_vector(result_vector.data(), lnum_elements);

    return RC::SUCCESS;
}

RC VectorType::to_string(const Value &val, string &result) const {
    float* vec = (float*)val.value_.pointer_value_;
    int num_elements = val.length_ ;// 获取元素数量
    vector<float> val_vector(vec, vec + num_elements);
    // 使用字符串流构建结果字符串
    std::ostringstream oss;
    oss << "[";
    for (int i = 0; i < num_elements; ++i) {
        oss << std::round(val_vector[i] * 100.0) / 100.0;
        if (i < num_elements - 1) {
            oss << ","; // 添加逗号分隔符
        }
    }
    oss << "]";

    // 将结果字符串赋值给 result
    result = oss.str();
    return RC::SUCCESS; // 返回成功
}



int VectorType::cast_cost(AttrType type)
{
  if (type == AttrType::CHARS) {
    return 1;
  }
  return INT32_MAX;
}

