/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/07/05.
//

#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/expr/arithmetic_operator.hpp"

#include <storage/db/db.h>

#include <gtest/internal/gtest-internal.h>

#include <utility>
#include <sql/optimizer/logical_plan_generator.h>
#include <sql/stmt/select_stmt.h>
#include <cmath>


using namespace std;

RC FieldExpr::get_value(const Tuple &tuple, Value &value) const
{
  return tuple.find_cell(TupleCellSpec(table_name(), field_name()), value);
}

bool FieldExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != ExprType::FIELD) {
    return false;
  }
  const auto &other_field_expr = static_cast<const FieldExpr &>(other);
  return table_name() == other_field_expr.table_name() && field_name() == other_field_expr.field_name();
}

// TODO: 在进行表达式计算时，`chunk` 包含了所有列，因此可以通过 `field_id` 获取到对应列。
// 后续可以优化成在 `FieldExpr` 中存储 `chunk` 中某列的位置信息。
RC FieldExpr::get_column(Chunk &chunk, Column &column)
{
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
  } else {
    column.reference(chunk.column(field().meta()->field_id()));
  }
  return RC::SUCCESS;
}

bool ValueExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != ExprType::VALUE) {
    return false;
  }
  const auto &other_value_expr = static_cast<const ValueExpr &>(other);
  return value_.compare(other_value_expr.get_value()) == 0;
}

RC ValueExpr::get_value(const Tuple &tuple, Value &value) const
{
  value = value_;
  return RC::SUCCESS;
}

RC ValueExpr::get_column(Chunk &chunk, Column &column)
{
  column.init(value_);
  return RC::SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
CastExpr::CastExpr(unique_ptr<Expression> child, AttrType cast_type) : child_(std::move(child)), cast_type_(cast_type)
{}

CastExpr::~CastExpr() {}

RC CastExpr::cast(const Value &value, Value &cast_value) const
{
  RC rc = RC::SUCCESS;
  if (this->value_type() == value.attr_type()) {
    cast_value = value;
    return rc;
  }
  rc = Value::cast_to(value, cast_type_, cast_value);
  return rc;
}

RC CastExpr::get_value(const Tuple &tuple, Value &result) const
{
  Value value;
  RC rc = child_->get_value(tuple, value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, result);
}

RC CastExpr::try_get_value(Value &result) const
{
  Value value;
  RC rc = child_->try_get_value(value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, result);
}

////////////////////////////////////////////////////////////////////////////////

ComparisonExpr::ComparisonExpr(CompOp comp, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : comp_(comp), left_(std::move(left)), right_(std::move(right))
{}

ComparisonExpr::ComparisonExpr(CompOp comp, Expression* left, Expression* right)
    : comp_(comp), left_(left), right_(right)
{}

ComparisonExpr::~ComparisonExpr() {}

RC ComparisonExpr::compare_value(const Value &left, const Value &right, bool &result) const
{
  RC  rc         = RC::SUCCESS;
  result         = false;
  switch (comp_) {
    case EQUAL_TO: {
      if(left.is_null() || right.is_null()) {
        result = false;
      }else {
        result = (0 == left.compare(right));
      }
    } break;
    case LESS_EQUAL: {
      if(left.is_null() || right.is_null()) {
        result = false;
      }else {
        result = (left.compare(right) <= 0);
      }
    } break;
    case NOT_EQUAL: {
      if(left.is_null() || right.is_null()) {
        result = false;
      }else {
        result = (left.compare(right) != 0);
      }
    } break;
    case LESS_THAN: {
      if (left.is_null() || right.is_null()){
        result = false;
      }else {
        result = (left.compare(right) < 0);
      }
    } break;
    case GREAT_EQUAL: {
      if(left.is_null() || right.is_null()) {
        result = false;
      }else {
        result = (left.compare(right) >= 0);
      }
    } break;
    case GREAT_THAN: {
      if(left.is_null() || right.is_null()) {
        result = false;
      }else {
        result = (left.compare(right) > 0);
      }
    } break;
    case LIKE: {
      if(left.attr_type() != AttrType::CHARS){
        LOG_ERROR("like only support CHARS type. attr_type=%s", left.attr_type());
        return RC::INVALID_ARGUMENT;
      }
      
      const char* pattern = right.data();
      const char* str = left.data();
      
      result = match_pattern(pattern, str);
    } break;
    case NOT_LIKE: {
      if(left.attr_type() != AttrType::CHARS){
        LOG_ERROR("like only support CHARS type. attr_type=%s", left.attr_type());
        return RC::INVALID_ARGUMENT;
      }

      const char* pattern = right.data();
      const char* str = left.data();

      result = !(match_pattern(pattern, str));
    } break;
    case IS_NULL: {
      result = left.is_null();
    } break;
    case IS_NOT_NULL: {
      result = !left.is_null();
    } break;
    default: {
      LOG_WARN("unsupported comparison. %d", comp_);
      rc = RC::INTERNAL;
    } break;
  }

  return rc;
}

bool ComparisonExpr::match_pattern(const char* pattern, const char* str) const {
    while (*str && *pattern != '%') {
        if (*pattern != '_' && *pattern != *str) return false;
        ++pattern;
        ++str;
    }

    if (*pattern == '%') {
        while (*(++pattern) == '%') ; // 跳过连续的 '%'
        if (!*pattern) return true; // 如果 pattern 结束了，说明匹配成功

        // 递归检查剩余部分
        while (*str) {
            if (match_pattern(pattern, str)) return true;
            ++str;
        }
    }

    return *pattern == '\0' && *str == '\0';
}

RC ComparisonExpr::try_get_value(Value &cell) const
{
  if (left_->type() == ExprType::VALUE && right_->type() == ExprType::VALUE) {
    ValueExpr *  left_value_expr  = static_cast<ValueExpr *>(left_.get());
    ValueExpr *  right_value_expr = static_cast<ValueExpr *>(right_.get());
    const Value &left_cell        = left_value_expr->get_value();
    const Value &right_cell       = right_value_expr->get_value();

    bool value = false;
    RC   rc    = compare_value(left_cell, right_cell, value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to compare tuple cells. rc=%s", strrc(rc));
    } else {
      cell.set_boolean(value);
    }
    return rc;
  }

  return RC::INVALID_ARGUMENT;
}

RC ComparisonExpr::get_value(const Tuple &tuple, Value &value) const
{
  Value left_value;
  Value right_value;

  RC rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_value(tuple, right_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }

  bool bool_value = false;
  if(left_->type() == ExprType::SUBQUERY) {
    rc = compare_subquery_left(right_value, bool_value);
  }else if(right_->type() == ExprType::SUBQUERY) {
    rc = compare_subquery_right(left_value, bool_value);
  }else if(left_ -> type() == ExprType::VALUELIST) {
    rc = compare_valuelist(true, right_value, bool_value);
  }else if(right_-> type() == ExprType::VALUELIST) {
      rc = compare_valuelist(false, left_value, bool_value);
  }
  else {
    rc = compare_value(left_value, right_value, bool_value);
  }
  if (rc == RC::SUCCESS) {
    value.set_boolean(bool_value);
  }
  return rc;
}

RC ComparisonExpr::compare_subquery_left(const Value &right, bool &value) const {
  SubQueryExpr* subquery_expr = dynamic_cast<SubQueryExpr*>(left_.get());
  std::vector<Value> result_vector = subquery_expr->get_result_vector();
  value = false;

  switch (comp_) {
    case IN:
      for (auto iter = result_vector.begin(); iter != result_vector.end(); iter++) {
        if (right.compare(*iter) == 0) {
          value = true;
          break;
        }
      }
    return RC::SUCCESS;

    case NOT_IN:
      value = true;
      for (auto iter = result_vector.begin(); iter != result_vector.end(); iter++) {
        if (right.compare(*iter) == 0) {
          value = false;
          break;
      }
    }
    return RC::SUCCESS;

    case EXISTS:
      value = result_vector.size() > 0;
    return RC::SUCCESS;

    case NOT_EXISTS:
      value = result_vector.size() == 0;
    return RC::SUCCESS;

    case EQUAL_TO:
    case LESS_EQUAL:
    case NOT_EQUAL:
    case LESS_THAN:
    case GREAT_EQUAL:
    case GREAT_THAN:
      if (result_vector.size() > 1) {
        LOG_WARN("subquery result size is more than one");
        return RC::INTERNAL;
      }else if(result_vector.size() == 0) {
        value = false;
        return RC::SUCCESS;
      }else {
        return compare_value(result_vector[0], right, value);
      }

    default:
      LOG_WARN("unsupported comparison. %d", comp_);
    return RC::INTERNAL;
  }
}

RC ComparisonExpr::compare_subquery_right(const Value &left, bool &value) const {
  SubQueryExpr* subquery_expr = dynamic_cast<SubQueryExpr*>(right_.get());
  std::vector<Value> result_vector = subquery_expr->get_result_vector();
  value = false;

  switch (comp_) {
    case IN:
      for (auto iter = result_vector.begin(); iter != result_vector.end(); iter++) {
        if (left.compare(*iter) == 0) {
          value = true;
          break;
        }
      }
    return RC::SUCCESS;

    case NOT_IN:
      value = true;
    for (auto iter = result_vector.begin(); iter != result_vector.end(); iter++) {
      if (left.compare(*iter) == 0) {
        value = false;
        break;
      }
    }
    return RC::SUCCESS;

    case EXISTS:
      value = result_vector.size() > 0;
    return RC::SUCCESS;

    case NOT_EXISTS:
      value = result_vector.size() == 0;
    return RC::SUCCESS;

    case EQUAL_TO:
    case LESS_EQUAL:
    case NOT_EQUAL:
    case LESS_THAN:
    case GREAT_EQUAL:
    case GREAT_THAN:
      if (result_vector.size() > 1) {
        LOG_WARN("subquery result size is more than one");
        return RC::ERROR_IN_SSQ;
      } else if(result_vector.size() == 0) {
        value = false;
        return RC::SUCCESS;
      } else {
        return compare_value(left, result_vector[0], value);
      }

    default: LOG_WARN("unsupported comparison. %d", comp_);
    return RC::INTERNAL;
  }
}

RC ComparisonExpr::compare_valuelist(bool is_left,const Value &a, bool &value) const{
  ValueListExpr* expr = nullptr;
  if(is_left) {
    expr = dynamic_cast<ValueListExpr*>(left_.get());
  }else {
    expr = dynamic_cast<ValueListExpr*>(right_.get());
  }
  std::vector<Value> values_list = expr->get_value_list();
  value = false;

  switch (comp_) {
    case IN:
      for (auto & iter : values_list) {
      if (a.compare(iter) == 0) {
        value = true;
        break;
      }
    }
    return RC::SUCCESS;

    case NOT_IN:
      value = true;
      for (auto & iter : values_list) {
          if (a.compare(iter) == 0) {
            value = false;
            break;
        }
      }
      return RC::SUCCESS;
    default:
      LOG_WARN("unsupported comparison. %d", comp_);
      return RC::INTERNAL;
  }
}


RC ComparisonExpr::eval(Chunk &chunk, std::vector<uint8_t> &select)
{
  RC     rc = RC::SUCCESS;
  Column left_column;
  Column right_column;

  rc = left_->get_column(chunk, left_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_column(chunk, right_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }
  if (left_column.attr_type() != right_column.attr_type()) {
    LOG_WARN("cannot compare columns with different types");
    return RC::INTERNAL;
  }
  if (left_column.attr_type() == AttrType::INTS) {
    rc = compare_column<int>(left_column, right_column, select);
  } else if (left_column.attr_type() == AttrType::FLOATS) {
    rc = compare_column<float>(left_column, right_column, select);
  } else if (left_column.attr_type() == AttrType::CHARS && comp_ == CompOp::LIKE) {
    // like 比较
    for (int i = 0; i < left_column.count(); i++) {
      Value left_value  = left_column.get_value(i);
      Value right_value = right_column.get_value(i);
      select[i] &= (match_pattern(left_value.data(), right_value.data()) ? 1 : 0);
    }
  } else if(left_column.attr_type() == AttrType::CHARS && comp_ == CompOp::NOT_LIKE) {
    // not like 比较
    for (int i = 0; i < left_column.count(); i++) {
      Value left_value  = left_column.get_value(i);
      Value right_value = right_column.get_value(i);
      select[i] &= (match_pattern(left_value.data(), right_value.data()) ? 0 : 1);
    }
  } else if(comp_ ==CompOp::IS_NULL) {
    for (int i = 0; i < left_column.count(); i++) {
      Value left_value  = left_column.get_value(i);
      select[i] &= (left_value.is_null() ? 0 : 1);
    }
  }else if(comp_ ==CompOp::IS_NOT_NULL) {
      for (int i = 0; i < left_column.count(); i++) {
      Value left_value  = left_column.get_value(i);
      select[i] &= (!left_value.is_null() ? 1 : 0);
    }
  }
  else {
    // TODO: support string compare
    LOG_WARN("unsupported data type %d", left_column.attr_type());
    return RC::INTERNAL;
  }
  return rc;
}

template <typename T>
RC ComparisonExpr::compare_column(const Column &left, const Column &right, std::vector<uint8_t> &result) const
{
  RC rc = RC::SUCCESS;

  bool left_const  = left.column_type() == Column::Type::CONSTANT_COLUMN;
  bool right_const = right.column_type() == Column::Type::CONSTANT_COLUMN;
  if (left_const && right_const) {
    compare_result<T, true, true>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  } else if (left_const && !right_const) {
    compare_result<T, true, false>((T *)left.data(), (T *)right.data(), right.count(), result, comp_);
  } else if (!left_const && right_const) {
    compare_result<T, false, true>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  } else {
    compare_result<T, false, false>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  }
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
ConjunctionExpr::ConjunctionExpr(Type type, Expression *left, Expression *right):
    conjunction_type_(type) , left_(left), right_(right){}

RC ConjunctionExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  rc = left_->get_value(tuple, left_value);
  if (rc == RC::INVALID_ARGUMENT) {
    LOG_DEBUG("divide by zero!");
    left_value.set_boolean(false);
    // rc = RC::SUCCESS;
  }else if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value by left child expression. rc=%s", strrc(rc));
    return rc;
  }

  if(conjunction_type_ == Type::AND) {
    if(left_value.get_boolean() == true) {
      Value right_value;
      rc = right_->get_value(tuple, right_value);
      if (rc == RC::INVALID_ARGUMENT) {
        LOG_DEBUG("divide by zero!");
        right_value.set_boolean(false);
        // rc =   RC::SUCCESS;
      }else if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get value by right child expression. rc=%s", strrc(rc));
        return rc;
      }
      value.set_boolean(right_value.get_boolean());
    }else {
      value.set_boolean(false);
    }
  }else if(conjunction_type_ == Type::OR) {
    if(left_value.get_boolean() == false) {
      Value right_value;
      rc = right_->get_value(tuple, right_value);
      if (rc == RC::INVALID_ARGUMENT) {
        LOG_DEBUG("divide by zero!");
        right_value.set_boolean(false);
        // rc = RC::SUCCESS;
      }else if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get value by right child expression. rc=%s", strrc(rc));
        return rc;
      }
    value.set_boolean(right_value.get_boolean());
    }else {
      value.set_boolean(true);
    }
  }else {
    LOG_ERROR("unsupported conjunction type %d", conjunction_type_);
    return  RC::INTERNAL;
  }

  return rc;
}

////////////////////////////////////////////////////////////////////////////////

ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, Expression *left, Expression *right)
    : arithmetic_type_(type), left_(left), right_(right)
{}
ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : arithmetic_type_(type), left_(std::move(left)), right_(std::move(right))
{}

bool ArithmeticExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (type() != other.type()) {
    return false;
  }
  auto &other_arith_expr = static_cast<const ArithmeticExpr &>(other);
  return arithmetic_type_ == other_arith_expr.arithmetic_type() && left_->equal(*other_arith_expr.left_) &&
         right_->equal(*other_arith_expr.right_);
}
AttrType ArithmeticExpr::value_type() const
{
  if (!right_) {
    return left_->value_type();
  }

  if (left_->value_type() == AttrType::INTS && right_->value_type() == AttrType::INTS &&
      arithmetic_type_ != Type::DIV) {
    return AttrType::INTS;
  }

  if (left_->value_type() == AttrType::VECTORS || right_->value_type() == AttrType::VECTORS) {
    return AttrType::VECTORS;
  }

  return AttrType::FLOATS;
}

RC ArithmeticExpr::calc_value(const Value &left_value, const Value &right_value, Value &value) const
{
  RC rc = RC::SUCCESS;

  const AttrType target_type = value_type();
  value.set_type(target_type);

  switch (arithmetic_type_) {
    case Type::ADD: {
      Value::add(left_value, right_value, value);
    } break;

    case Type::SUB: {
      Value::subtract(left_value, right_value, value);
    } break;

    case Type::MUL: {
      Value::multiply(left_value, right_value, value);
    } break;

    case Type::DIV: {
      if ( (right_value.attr_type() == AttrType::INTS && right_value.get_int() == 0)
        || (right_value.attr_type() == AttrType::FLOATS && right_value.get_float() == 0)) {
        LOG_DEBUG("divide by zero");
        return RC::INVALID_ARGUMENT;
      }
      Value::divide(left_value, right_value, value);
    } break;

    case Type::NEGATIVE: {
      Value::negative(left_value, value);
    } break;

    case Type::LD: {
      value.set_type(AttrType::FLOATS);
      Value::l2_distance(left_value, right_value, value);
    }break;

    case Type::CD: {
      value.set_type(AttrType::FLOATS);
      Value::cosine_distance(left_value, right_value, value);
    }break;

    case Type::IP: {
      value.set_type(AttrType::FLOATS);
      Value::inner_product(left_value, right_value, value);
    }break;

    default: {
      rc = RC::INTERNAL;
      LOG_WARN("unsupported arithmetic type. %d", arithmetic_type_);
    } break;
  }
  return rc;
}

template <bool LEFT_CONSTANT, bool RIGHT_CONSTANT>
RC ArithmeticExpr::execute_calc(
    const Column &left, const Column &right, Column &result, Type type, AttrType attr_type) const
{
  RC rc = RC::SUCCESS;
  switch (type) {
    case Type::ADD: {
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, AddOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, AddOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
    } break;
    case Type::SUB:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, SubtractOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, SubtractOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::MUL:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, MultiplyOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, MultiplyOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::DIV:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, DivideOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, DivideOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::NEGATIVE:
      if (attr_type == AttrType::INTS) {
        unary_operator<LEFT_CONSTANT, int, NegateOperator>((int *)left.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        unary_operator<LEFT_CONSTANT, float, NegateOperator>(
            (float *)left.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    default: rc = RC::UNIMPLEMENTED; break;
  }
  if (rc == RC::SUCCESS) {
    result.set_count(result.capacity());
  }
  return rc;
}

RC ArithmeticExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  if(right_ != nullptr) {
    rc = right_->get_value(tuple, right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }else {
    right_value.set_value(Value(0));
  }

  return calc_value(left_value, right_value, value);
}

RC ArithmeticExpr::get_column(Chunk &chunk, Column &column)
{
  RC rc = RC::SUCCESS;
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
    return rc;
  }
  Column left_column;
  Column right_column;

  rc = left_->get_column(chunk, left_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get column of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_column(chunk, right_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get column of right expression. rc=%s", strrc(rc));
    return rc;
  }
  return calc_column(left_column, right_column, column);
}

RC ArithmeticExpr::calc_column(const Column &left_column, const Column &right_column, Column &column) const
{
  RC rc = RC::SUCCESS;

  const AttrType target_type = value_type();
  column.init(target_type, left_column.attr_len(), std::max(left_column.count(), right_column.count()));
  bool left_const  = left_column.column_type() == Column::Type::CONSTANT_COLUMN;
  bool right_const = right_column.column_type() == Column::Type::CONSTANT_COLUMN;
  if (left_const && right_const) {
    column.set_column_type(Column::Type::CONSTANT_COLUMN);
    rc = execute_calc<true, true>(left_column, right_column, column, arithmetic_type_, target_type);
  } else if (left_const && !right_const) {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<true, false>(left_column, right_column, column, arithmetic_type_, target_type);
  } else if (!left_const && right_const) {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<false, true>(left_column, right_column, column, arithmetic_type_, target_type);
  } else {
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<false, false>(left_column, right_column, column, arithmetic_type_, target_type);
  }
  return rc;
}

RC ArithmeticExpr::try_get_value(Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->try_get_value(left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  if (right_) {
    rc = right_->try_get_value(right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  return calc_value(left_value, right_value, value);
}

////////////////////////////////////////////////////////////////////////////////

UnboundAggregateExpr::UnboundAggregateExpr(const char *aggregate_name, Expression *child)
    : aggregate_name_(aggregate_name), child_(child)
{}

////////////////////////////////////////////////////////////////////////////////
AggregateExpr::AggregateExpr(Type type, Expression *child) : aggregate_type_(type), child_(child) {}

AggregateExpr::AggregateExpr(Type type, unique_ptr<Expression> child) : aggregate_type_(type), child_(std::move(child))
{}

RC AggregateExpr::get_column(Chunk &chunk, Column &column)
{
  RC rc = RC::SUCCESS;
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
  } else {
    rc = RC::INTERNAL;
  }
  return rc;
}

bool AggregateExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != type()) {
    return false;
  }
  const AggregateExpr &other_aggr_expr = static_cast<const AggregateExpr &>(other);
  return aggregate_type_ == other_aggr_expr.aggregate_type() && child_->equal(*other_aggr_expr.child());
}

unique_ptr<Aggregator> AggregateExpr::create_aggregator() const
{
  unique_ptr<Aggregator> aggregator;
  switch (aggregate_type_) {
    case Type::SUM: {
      aggregator = make_unique<SumAggregator>();
    }break;
    case Type::COUNT: {
      aggregator = make_unique<CountAggregator>();
    }break;
    case Type::MAX: {
        aggregator = make_unique<MaxAggregator>();
    }break;
    case Type::MIN: {
        aggregator = make_unique<MinAggregator>();
    }break;
    case Type::AVG: {
      aggregator = make_unique<AvgAggregator>();
    }break;
    default: {
      ASSERT(false, "unsupported aggregate type");
    }break;
  }
  return aggregator;
}

RC AggregateExpr::get_value(const Tuple &tuple, Value &value) const
{
  return tuple.find_cell(TupleCellSpec(name()), value);
}

RC AggregateExpr::type_from_string(const char *type_str, AggregateExpr::Type &type)
{
  RC rc = RC::SUCCESS;
  if (0 == strcasecmp(type_str, "count")) {
    type = Type::COUNT;
  } else if (0 == strcasecmp(type_str, "sum")) {
    type = Type::SUM;
  } else if (0 == strcasecmp(type_str, "avg")) {
    type = Type::AVG;
  } else if (0 == strcasecmp(type_str, "max")) {
    type = Type::MAX;
  } else if (0 == strcasecmp(type_str, "min")) {
    type = Type::MIN;
  } else {
    rc = RC::INVALID_ARGUMENT;
  }
  return rc;
}

SubQueryExpr::SubQueryExpr(SelectSqlNode select_sql_node)
  : select_sql_node_(std::move(select_sql_node))
{
  this->sub_query_result = std::vector<Value>();
}

RC SubQueryExpr::get_value(const Tuple &tuple, Value &value) const {
  if(this->sub_query_result.size() == 0) {
    value.set_null();
  }else {
    value.set_value(this->sub_query_result[0]);
  }
  return RC::SUCCESS;
}

AttrType SubQueryExpr::value_type() const{
  auto stmt = dynamic_cast<SelectStmt*>(this->select_stmt_);
  auto& query = stmt->query_expressions();
  if(query.size() != 1) {
    return AttrType::UNDEFINED;
  }else {
    return query[0]->value_type();
  }
}

RC SubQueryExpr::Create_stmt(Db *db){
  return SelectStmt::create(db, this->select_sql_node_, this->select_stmt_);
}

RC SubQueryExpr::LogicalPlanGenerate(){
  RC rc = LogicalPlanGenerator::create(this->select_stmt_, this->logical_plan_);
  // 判断RC
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to create logical plan in sub query");
    return rc;
  }
  return RC::SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
ValueListExpr::ValueListExpr()
{
  this->value_list_ = std::vector<Value>();
}

RC ValueListExpr::get_value(const Tuple &tuple, Value &value) const {
  if(this->value_list_.size() == 0) {
    value.set_null();
  }else {
    value.set_value(this->value_list_[0]);
  }
  return RC::SUCCESS;
}

AttrType ValueListExpr::value_type() const
{
  if (this->value_list_.size() == 0) {
    return AttrType::UNDEFINED;
  }else{
    return this->value_list_[0].attr_type();
  }
}

FuncExpr::FuncExpr(FuncType func_type, std::vector<std::unique_ptr<Expression>> &child){
  func_type_ = func_type;
  child_ = std::move(child);
}

AttrType FuncExpr::value_type() const{
  switch (func_type_) {
    case FuncExpr::LENGTH:
      return AttrType::INTS;
    case FuncExpr::ROUND: return AttrType::FLOATS;
    case FuncType::DATE_FORMAT:
      return AttrType::CHARS;
    default:
      return AttrType::UNDEFINED;
  }
}

RC FuncExpr::get_value(const Tuple &tuple, Value &value) const{
  RC rc = RC::SUCCESS;
  switch (func_type_) {
  case LENGTH: {
    rc = get_length_value(tuple, value);
    break;
  }
  case ROUND: {
    rc = get_round_value(tuple, value);
    break;
  }
  case DATE_FORMAT: {
    rc = get_date_format_value(tuple, value);
    break;
  }
  default: break;
  }
  return rc;
}

RC FuncExpr::get_length_value(const Tuple &tuple, Value &value) const{
  RC rc = RC::SUCCESS;
  auto &child = child_.front();
  Value value_temp;
  rc = child->get_value(tuple, value_temp);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of child expression. rc=%s", strrc(rc));
    return rc;
  }

  if(value_temp.attr_type() != AttrType::CHARS) {
    LOG_WARN("invalid type of child expression. type=%d", value_temp.attr_type());
    return RC::INVALID_ARGUMENT;
  }
  value = Value(static_cast<int>(strlen(value_temp.data())));
  return RC::SUCCESS;
}

RC FuncExpr::get_round_value(const Tuple &tuple, Value &value) const{
  RC rc = RC::SUCCESS;
  switch (child_.size()) {
  case 1:
    {
      Value value_temp;
      rc = child_.front()->get_value(tuple, value_temp);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get value of child expression. rc=%s", strrc(rc));
        return rc;
      }
      if(value_temp.attr_type() != AttrType::FLOATS) {
        LOG_WARN("invalid type of child expression. type=%d", value_temp.attr_type());
      }
      double val = value_temp.get_float();
      value = Value(static_cast<float>(std::round(val)));
      return RC::SUCCESS;
    }break;
  case 2:
    {
      Value value_temp;
      rc = child_[0]->get_value(tuple, value_temp);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get value of child expression. rc=%s", strrc(rc));
        return rc;
      }
      if(value_temp.attr_type() != AttrType::FLOATS) {
        LOG_WARN("invalid type of child expression. type=%d", value_temp.attr_type());
      }
      double val = value_temp.get_float();

      Value value_temp2;
      rc = child_[1]->get_value(tuple, value_temp2);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get value of child expression. rc=%s", strrc(rc));
        return rc;
      }
      if(value_temp2.attr_type() != AttrType::INTS) {
        LOG_WARN("invalid type of child expression. type=%d", value_temp2.attr_type());
      }
      int precision = value_temp2.get_int();
      value = Value(static_cast<float>(std::round(val * std::pow(10, precision)) / std::pow(10, precision)));
      return RC::SUCCESS;
    }break;
    default: break;
  }
  return RC::UNIMPLEMENTED;
}


RC FuncExpr::get_date_format_value(const Tuple &tuple, Value &value) const{
  RC rc = RC::SUCCESS;
  Value value_temp;
  rc = child_.front()->get_value(tuple, value_temp);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of child expression. rc=%s", strrc(rc));
    return rc;
  }
  if(value_temp.attr_type() != AttrType::DATES) {
    if(value_temp.attr_type() == AttrType::CHARS) {
      // 尝试转换为date
      const char* temp = value_temp.data();
      // 判断是否为日期格式
      if(temp[4] != '-' || temp[7] != '-') {
        return RC::INVALID_ARGUMENT;
      }
      int y, m ,d;
      // 提取年份
      y = (temp[0] - '0') * 1000 + (temp[1] - '0') * 100 + (temp[2] - '0') * 10 + (temp[3] - '0');

      // 提取月份
      m = (temp[5] - '0') * 10 + (temp[6] - '0');

      // 提取日期
      d = (temp[8] - '0') * 10 + (temp[9] - '0');
      value_temp.set_date(y,m,d);
    }else {
      LOG_WARN("invalid type of child expression. type=%d", value_temp.attr_type());
      return RC::INVALID_ARGUMENT;
    }
  }

  Value format_value;
  rc = child_[1]->get_value(tuple, format_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of child expression. rc=%s", strrc(rc));
    return rc;
  }

  if(format_value.attr_type() != AttrType::CHARS) {
    LOG_WARN("invalid type of child expression. type=%d", format_value.attr_type());
    return RC::INVALID_ARGUMENT;
  }
  const char* format = format_value.data();
  int y, m , d;
  int date = value_temp.get_int();
  y = date / 10000;
  m = (date - y * 10000) / 100;
  d = date - y * 10000 - m * 100;
  stringstream ss;
  for(int i = 0; i < format_value.length(); i++) {
    if(format[i] == '%') {
      continue;
    }else if(format[i] == 'Y' || format[i] == 'y') {
      ss << std::setw(4) << std::setfill('0') << y;
    }else if(format[i] == 'M' || format[i] == 'm') {
      ss << std::setw(2) << std::setfill('0') << m;
    }else if(format[i] == 'D' || format[i] == 'd') {
      ss << std::setw(2) << std::setfill('0') << d;
    }else {
      ss << format[i];
    }
  }
  value = Value(ss.str().c_str());
  return RC::SUCCESS;
}



