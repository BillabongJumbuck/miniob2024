//
// Created by qjm on 9/28/24.
//

#ifndef DROP_TABLE_STMT_H
#define DROP_TABLE_STMT_H


#include <string>
#include <vector>

#include "sql/stmt/stmt.h"

class Db;
class Table;
struct DropTableSqlNode;

class DropTableStmt : public Stmt
{
public:
  DropTableStmt(Table *table) : table_(table) {}
  virtual ~DropTableStmt() = default;
  StmtType type() const override { return StmtType::DROP_TABLE; }

  Table             *table() const { return table_; }

  static RC            create(Db *db, const DropTableSqlNode &drop_table, Stmt *&stmt);
private:
  Table           *table_      = nullptr;
};

#endif //DROP_TABLE_STMT_H
