//
// Created by qjm on 12/6/24.
//

#include "order_by_logical_operator.h"

#include <sql/stmt/select_stmt.h>

OrderByLogicalOperator::OrderByLogicalOperator(std::vector<OrderByItem> &&order_by_items)
 {
   order_by_expressions_ = std::vector<std::unique_ptr<Expression>>();
   is_asc_ = std::vector<int>();
   for(auto &item: order_by_items) {
     order_by_expressions_.push_back(std::move(item.expr));
     is_asc_.push_back(item.is_asc ? 1 : 0);
   }
 }
