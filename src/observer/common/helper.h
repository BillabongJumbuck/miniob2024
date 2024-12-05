//
// Created by qjm on 12/5/24.
//

#ifndef HELPER_H
#define HELPER_H

#include "sql/operator/physical_operator.h"
#include "sql/operator/logical_operator.h"

inline std::unique_ptr<PhysicalOperator> po_in_helper;

inline std::unique_ptr<LogicalOperator> lo_in_helper;

inline std::unordered_map<std::string, Table*> extra_table_map;

inline bool use_extra_table_map = false;

#endif //HELPER_H
