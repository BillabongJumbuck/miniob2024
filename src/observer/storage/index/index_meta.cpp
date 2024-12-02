/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai.wyl on 2021/5/18.
//

#include "storage/index/index_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "storage/field/field_meta.h"
#include "storage/table/table_meta.h"
#include "json/json.h"
const static Json::StaticString UNIQUE_TAG("unique");
const static Json::StaticString FIELD_NAME("name");
const static Json::StaticString FIELD_FIELD_NAME("field_name");

RC IndexMeta::init(bool is_unique, const char *name, const std::vector<const FieldMeta*> &fields)
{
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }

  unique_tag = is_unique;
  name_  = name;
  for (const FieldMeta *field : fields) {
    field_.emplace_back(field->name());
  }
  return RC::SUCCESS;
}

void IndexMeta::to_json(Json::Value &json_value) const
{
  json_value[UNIQUE_TAG]       = unique_tag;
  json_value[FIELD_NAME]       = name_;

  Json::Value fields;
  for (int i = 0; i < field_.size(); i++) {
    fields[i] = field_[i];
  }
  json_value[FIELD_FIELD_NAME] = std::move(fields);
}

RC IndexMeta::from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index)
{
  const Json::Value &unique_value = json_value[UNIQUE_TAG];
  const Json::Value &name_value  = json_value[FIELD_NAME];
  const Json::Value &field_value = json_value[FIELD_FIELD_NAME];

  if (!unique_value.isBool()) {
    LOG_ERROR("Unique tag of index [%s] is not a boolean. json value=%s",
        name_value.asCString(), unique_value.toStyledString().c_str());
    return RC::INTERNAL;
  }
  if (!name_value.isString()) {
    LOG_ERROR("Index name is not a string. json value=%s", name_value.toStyledString().c_str());
    return RC::INTERNAL;
  }

  std::vector<const FieldMeta*> fields;
  for (int i = 0; i < field_value.size(); i++) {
    const FieldMeta *field = table.field(field_value[i].asCString());
    if (nullptr == field) {
      LOG_ERROR("Deserialize index [%s]: no such field: %s", name_value.asCString(), field_value.asCString());
      return RC::SCHEMA_FIELD_MISSING;
    }
    fields.emplace_back(field);
  }

  return index.init(unique_value.asBool(),name_value.asCString(), fields);
}

const bool IndexMeta::is_unique() const { return unique_tag; }

const char *IndexMeta::name() const { return name_.c_str(); }

const std::vector<std::string> &IndexMeta::field() const { return field_; }

// void IndexMeta::desc(ostream &os) const { os << "index name=" << name_ << ", field=" << field_; }