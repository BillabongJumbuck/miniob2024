//
// Created by qjm on 11/12/24.
//

#ifndef SUBQUERY_SIMPLIFICATION_RULE_H
#define SUBQUERY_SIMPLIFICATION_RULE_H

#pragma once

#include "common/rc.h"
#include "sql/optimizer/rewrite_rule.h"

class LogicalOperator;


class SubQuerySimplificationRule : public ExpressionRewriteRule
{
public:
  SubQuerySimplificationRule() = default;
  virtual ~SubQuerySimplificationRule() = default;

  RC rewrite(std::unique_ptr<Expression> &expr, bool &change_made) override;

};


#endif //SUBQUERY_SIMPLIFICATION_RULE_H
