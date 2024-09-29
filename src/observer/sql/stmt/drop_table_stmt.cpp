//
// Created by qjm on 9/28/24.
//

#include "common/log/log.h"
#include "common/types.h"
#include "common/lang/string.h"
#include "sql/stmt/drop_table_stmt.h"
#include "event/sql_debug.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

RC DropTableStmt::create(Db *db, const DropTableSqlNode &drop_table, Stmt *&stmt)
{
  stmt = nullptr;

  const char *table_name = drop_table.relation_name.c_str(); // 要删除的表的名字
  if (common::is_blank(table_name)) {
    LOG_WARN("invalid argument. db=%p, table_name=%p", db, table_name);
    return RC::INVALID_ARGUMENT;
  }

  // 检查表是否存在
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("drop table error :no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  stmt = new DropTableStmt(table);
  return RC::SUCCESS;
}
