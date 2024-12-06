//
// Created by qjm on 12/6/24.
//

#ifndef ORDER_BY_LOGICAL_OPER_H
#define ORDER_BY_LOGICAL_OPER_H

#include "sql/operator/logical_operator.h"

struct OrderByItem;

class OrderByLogicalOperator : public LogicalOperator
{
public:
  OrderByLogicalOperator(std::vector<OrderByItem> &&order_by_items);

  virtual ~OrderByLogicalOperator() = default;

  LogicalOperatorType type() const override {return LogicalOperatorType::ORDER_BY;}

  auto &is_asc(){return is_asc_;}
  auto &order_by_expressions(){return order_by_expressions_;}
private:
  std::vector<int> is_asc_;
  std::vector<std::unique_ptr<Expression>> order_by_expressions_;
};



#endif //ORDER_BY_LOGICAL_OPER_H
