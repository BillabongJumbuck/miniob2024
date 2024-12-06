//
// Created by qjm on 12/6/24.
//

#include <algorithm>
#include "order_by_physical_operator.h"

OrderByPhysicalOperator::OrderByPhysicalOperator(
    std::vector<int> &is_asc, std::vector<std::unique_ptr<Expression>> &order_by_expressions)
{
  for (int i = 0; i < is_asc.size(); i++) {
    is_asc_.push_back(is_asc[i]);
    order_by_expressions_.push_back(std::move(order_by_expressions[i]));
  }
}
RC OrderByPhysicalOperator::open(Trx *trx)
{
  ASSERT(children_.size() == 1, "order by operator only support one child, but got %d", children_.size());

  PhysicalOperator &child = *children_[0];
  RC                rc    = child.open(trx);
  if (OB_FAIL(rc)) {
    LOG_INFO("failed to open child operator. rc=%s", strrc(rc));
    return rc;
  }

  while (OB_SUCC(rc = child.next())) {
    Tuple *child_tuple = child.current_tuple();
    if (nullptr == child_tuple) {
      LOG_WARN("failed to get tuple from child operator. rc=%s", strrc(rc));
      return RC::INTERNAL;
    }

    if (OB_FAIL(rc)) {
      LOG_WARN("failed to get next tuple. rc=%s", strrc(rc));
      return rc;
    }

    tuples_.push_back(child_tuple);
  }

  if (RC::RECORD_EOF == rc) {
    rc = RC::SUCCESS;
  }

  if (OB_SUCC(rc)) {
    std::sort(tuples_.begin(), tuples_.end(), [this](Tuple* a, Tuple* b) {
      return this->compare(a, b) < 0;
    });
  }

  return RC::SUCCESS;
}

RC OrderByPhysicalOperator::next()
{
  pos_++;
  if (pos_ < tuples_.size()) {
    return RC::SUCCESS;
  } else {
    return RC::RECORD_EOF;
  }
}

RC OrderByPhysicalOperator::close()
{
  pos_ = -1;
  return children_[0]->close();
}

Tuple *OrderByPhysicalOperator::current_tuple() { return tuples_[pos_]; }

RC OrderByPhysicalOperator::heap_sort()
{
  RC rc = RC::SUCCESS;
  int i;
  print();
  for(i = tuples_.size()/2; i>=0; i--) {
    rc = PercDown(i);
    if(rc != RC::SUCCESS) {
      LOG_INFO("failed to heap sort. rc=%s", strrc(rc));
      return rc;
    }
    print();
  }

  for(i = tuples_.size()-1; i>0; i--) {
    Tuple *tmp = tuples_[i];
    tuples_[i] = tuples_[0];
    tuples_[0] = tmp;
    rc = PercDown(0);
    if(rc != RC::SUCCESS) {
      LOG_INFO("failed to heap sort. rc=%s", strrc(rc));
      return rc;
    }
    print();
  }
  return rc;
}

RC OrderByPhysicalOperator::PercDown(int i)
{
  RC rc = RC::SUCCESS;
  int Parent, Child;
  Tuple *tmp = tuples_[i];
  for(Parent = i; (Parent * 2 + 1) < tuples_.size(); Parent = Child) {
    Child = Parent * 2 + 1;
    if(Child != tuples_.size() - 1 && compare(tuples_[Child], tuples_[Child+1]) < 0) {
      Child++;
    }
    if(compare(tmp, tuples_[Child]) >= 0) break;
    else tuples_[Parent] = tuples_[Child];
  }
  tuples_[Parent] = tmp;
  return rc;
}

int OrderByPhysicalOperator::compare(Tuple *a, Tuple *b)
{
  Value a_value, b_value;
  for (int i = 0; i < order_by_expressions_.size(); i++) {
    auto &expr = order_by_expressions_[i];
    expr->get_value(*a, a_value);
    expr->get_value(*b, b_value);

    int res = a_value.compare(b_value);
    if (res == 0) {
      continue;
    }

    if (is_asc_[i]) {
      // Ascending order: return true if a < b
      if (res < 0)
        return -1;  // a < b
      else
        return 1;  // a > b
    } else {
      // Descending order: return true if a > b
      if (res > 0)
        return -1;  // a > b
      else
        return 1;  // a < b
    }
  }
  return 0;
}

void OrderByPhysicalOperator::print()
{
  for(auto &tuple : tuples_) {
    cout << tuple->to_string() << endl;
  }
  cout << endl;
}

