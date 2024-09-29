//
// Created by qjm on 9/28/24.
//

#ifndef DROP_TABLE_EXECUTOR_H
#define DROP_TABLE_EXECUTOR_H

#include "common/rc.h"

class SQLStageEvent;

/**
 * @brief 删除表的执行器
 * @ingroup Executor
 */
class DropTableExecutor
{
public:
  DropTableExecutor()          = default;
  virtual ~DropTableExecutor() = default;

  RC execute(SQLStageEvent *sql_event);
};

#endif //DROP_TABLE_EXECUTOR_H
