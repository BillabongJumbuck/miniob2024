//
// Created by qjm on 12/6/24.
//

#ifndef ORDER_BY_PHYSICAL_OPERATOR_H
#define ORDER_BY_PHYSICAL_OPERATOR_H

#include "sql/operator/physical_operator.h"

/**
 * @brief Order By 物理算子
 * @ingroup PhysicalOperator
 */
class OrderByPhysicalOperator : public PhysicalOperator
{
public:
  OrderByPhysicalOperator() = default;

  OrderByPhysicalOperator(std::vector<int>& is_asc, std::vector<std::unique_ptr<Expression>>& order_by_expressions);

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::ORDER_BY;
  }

  virtual ~OrderByPhysicalOperator() = default;

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

private:
  RC heap_sort();
  RC PercDown(int index);
  int compare(Tuple *tuple1, Tuple *tuple2);

private:
  std::vector<int> is_asc_;  // 0: desc, 1: asc
  std::vector<std::unique_ptr<Expression>> order_by_expressions_; // 待排序的表达式
  int pos_ = -1; // 当前tuple的索引
  std::vector<Tuple*> tuples_; // 所有收集到的tuple
};



#endif //ORDER_BY_PHYSICAL_OPERATOR_H
