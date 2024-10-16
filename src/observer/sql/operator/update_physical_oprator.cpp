//
// Created by qjm on 10/14/24.
//

#include "sql/operator/update_physical_oprator.h"

#include <utility>
#include "common/log/log.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

UpdatePhysicalOperator::UpdatePhysicalOperator(Table *table,const FieldMeta *field_meta, Value value)
{
  table_ = table;
  field_meta_ = field_meta;
  value_ = std::move(value);
}

RC UpdatePhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  std::unique_ptr<PhysicalOperator> &child = children_[0];

  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  while (OB_SUCC(rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record   &record    = row_tuple->record();
    records_.emplace_back(std::move(record));
  }

  child->close();

  // 先收集记录再Update
  // 记录的有效性由事务来保证，如果事务不保证Update的有效性，那说明此事务类型不支持并发控制，比如VacuousTrx
  for (Record &record : records_) {
    Record new_record = record;

    size_t       copy_len = field_meta_->len();
    const size_t data_len = value_.length();
    if (field_meta_->type() == AttrType::CHARS) {
      if (copy_len > data_len) {
        copy_len = data_len + 1;
      }
    }
    memcpy(new_record.data() + field_meta_->offset(), value_.data(), copy_len);

    trx->delete_record(table_, record);
    trx_->insert_record(table_, new_record);

    // if (rc != RC::SUCCESS) {
    //   LOG_WARN("failed to update record: %s", strrc(rc));
    //   return rc;
    // }
  }

  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next() { return RC::RECORD_EOF; }

RC UpdatePhysicalOperator::close() { return RC::SUCCESS; }


