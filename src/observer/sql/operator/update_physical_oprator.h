//
// Created by qjm on 10/14/24.
//

#ifndef UPDATE_PHYSICAL_OPRATOR_H
#define UPDATE_PHYSICAL_OPRATOR_H

#include "sql/operator/physical_operator.h"

class Trx;
class UpdateStmt;

/**
 * @brief 物理算子, Update
 * @ingroup PhysicalOperator
 */
class UpdatePhysicalOperator : public PhysicalOperator
{
public:
  UpdatePhysicalOperator(Table *table, const FieldMeta *field_meta, Expression* value);

  virtual ~UpdatePhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::UPDATE; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override { return nullptr; }

private:
  Table               *table_      = nullptr;
  const FieldMeta     *field_meta_ = nullptr;
  Expression*          value_                ;
  Trx                 *trx_        = nullptr;
  std::vector<Record> records_              ;
};



#endif //UPDATE_PHYSICAL_OPRATOR_H
