
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

UnboundAggregateExpr *create_aggregate_expression(const char *aggregate_name,
                                           Expression *child,
                                           const char *sql_string,
                                           YYLTYPE *llocp)
{
  UnboundAggregateExpr *expr = new UnboundAggregateExpr(aggregate_name, child);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

%}

%define api.pure full
%define parse.error verbose
/** 启用位置标识 **/
%locations
%lex-param { yyscan_t scanner }
/** 这些定义了在yyparse函数中的参数 **/
%parse-param { const char * sql_string }
%parse-param { ParsedSqlResult * sql_result }
%parse-param { void * scanner }

//标识tokens
%token  SEMICOLON
        BY
        CREATE
        DROP
        GROUP
        TABLE
        TABLES
        INDEX
        CALC
        SELECT
        DESC
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        INT_T
        STRING_T
        DATE_T
        FLOAT_T
        VECTOR_T
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        WHERE
        AND
        SET
        ON
        LOAD
        DATA
        INFILE
        EXPLAIN
        STORAGE
        FORMAT
        EQ
        LT
        GT
        LE
        GE
        NE
        LIKE_OP
        NOT
        NULL_TYPE
        NULLABLE
        IS
        JOIN
        INNER
        L2_DISTANCE
        COSINE_DISTANCE
        INNER_PRODUCT
        IN_OP
        EXISTS_OP
        UNIQUE
        LENGTH
        ROUND
        DATE_FORMAT
        AS
        ORDER
        ASC


/** union 中定义各种数据类型，真实生成的代码也是union类型，所以不能有非POD类型的数据 **/
%union {
  ParsedSqlNode *                            sql_node;
  Expression *                               condition;
  Value *                                    value;
  enum CompOp                                comp;
  RelAttrSqlNode *                           rel_attr;
  std::vector<AttrInfoSqlNode> *             attr_infos;
  AttrInfoSqlNode *                          attr_info;
  Expression *                               expression;
  UpdateItem *                               update_item_ptr;
  std::vector<UpdateItem>*                   update_item_list_ptr;
  std::vector<std::unique_ptr<Expression>> * expression_list;
  std::vector<Value> *                       value_list;
  std::vector<RelAttrSqlNode> *              rel_attr_list;
  std::vector<InnerJoinSqlNode> *            join_list;
  std::vector<std::string> *                 relation_list;
  InnerJoinSqlNode *                         join;
  TableWithAlias *                           alias;
  OrderAttrSqlNode *                         orderby_attr;
  std::vector<OrderAttrSqlNode> *            orderby_list;
  char *                                     string;
  int                                        number;
  float                                      floats;
  bool                                       boolean;
}

%token <number> NUMBER
%token <floats> FLOAT
%token <string> ID
%token <string> SSS
//非终结符

/** type 定义了各种解析后的结果输出的是什么类型。类型对应了 union 中的定义的成员变量名称 **/
%type <number>              type
%type <expression>          condition
%type <value>               value
%type <number>              number
%type <boolean>             null_option
%type <boolean>             is_null_comp
%type <boolean>             unique_tag
%type <string>              relation
%type <comp>                comp_op
%type <rel_attr>            rel_attr
%type <attr_infos>          attr_def_list
%type <attr_info>           attr_def
%type <value_list>          value_list
%type <condition>           where
%type <string>              storage_format
%type <relation_list>       rel_list
%type <alias>               relation_with_alias
%type <join_list>           from_list
%type <join>                join_list
%type <expression>          expression
%type <expression>          aggr_func_expr
%type <expression>          sub_query_expr
%type <expression>          value_list_expr
%type <expression>          func_expr
%type <number>              func_type
%type <value_list>          value_list_ssq
%type <string>              alias
%type <expression_list>     expression_list
%type <expression_list>     group_by
%type <orderby_list>        order_by
%type <orderby_list>        order_by_items
%type <orderby_attr>        order_by_item
%type <update_item_ptr>     update_item;
%type <update_item_list_ptr> update_item_list;
%type <sql_node>            calc_stmt
%type <sql_node>            select_stmt
%type <sql_node>            insert_stmt
%type <sql_node>            update_stmt
%type <sql_node>            delete_stmt
%type <sql_node>            create_table_stmt
%type <sql_node>            drop_table_stmt
%type <sql_node>            show_tables_stmt
%type <sql_node>            desc_table_stmt
%type <sql_node>            create_index_stmt
%type <sql_node>            drop_index_stmt
%type <sql_node>            sync_stmt
%type <sql_node>            begin_stmt
%type <sql_node>            commit_stmt
%type <sql_node>            rollback_stmt
%type <sql_node>            load_data_stmt
%type <sql_node>            explain_stmt
%type <sql_node>            set_variable_stmt
%type <sql_node>            help_stmt
%type <sql_node>            exit_stmt
%type <sql_node>            command_wrapper
// commands should be a list but I use a single command instead
%type <sql_node>            commands

%left OR
%left AND
%left EQ LT GT LE GE NE LIKE_OP
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%%

commands: command_wrapper opt_semicolon  //commands or sqls. parser starts here.
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>($1);
    sql_result->add_sql_node(std::move(sql_node));
  }
  ;

command_wrapper:
    calc_stmt
  | select_stmt
  | insert_stmt
  | update_stmt
  | delete_stmt
  | create_table_stmt
  | drop_table_stmt
  | show_tables_stmt
  | desc_table_stmt
  | create_index_stmt
  | drop_index_stmt
  | sync_stmt
  | begin_stmt
  | commit_stmt
  | rollback_stmt
  | load_data_stmt
  | explain_stmt
  | set_variable_stmt
  | help_stmt
  | exit_stmt
    ;

exit_stmt:      
    EXIT {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      $$ = new ParsedSqlNode(SCF_EXIT);
    };

help_stmt:
    HELP {
      $$ = new ParsedSqlNode(SCF_HELP);
    };

sync_stmt:
    SYNC {
      $$ = new ParsedSqlNode(SCF_SYNC);
    }
    ;

begin_stmt:
    TRX_BEGIN  {
      $$ = new ParsedSqlNode(SCF_BEGIN);
    }
    ;

commit_stmt:
    TRX_COMMIT {
      $$ = new ParsedSqlNode(SCF_COMMIT);
    }
    ;

rollback_stmt:
    TRX_ROLLBACK  {
      $$ = new ParsedSqlNode(SCF_ROLLBACK);
    }
    ;

drop_table_stmt:    /*drop table 语句的语法解析树*/
    DROP TABLE ID {
      $$ = new ParsedSqlNode(SCF_DROP_TABLE);
      $$->drop_table.relation_name = $3;
      free($3);
    };

show_tables_stmt:
    SHOW TABLES {
      $$ = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
    ;

desc_table_stmt:
    DESC ID  {
      $$ = new ParsedSqlNode(SCF_DESC_TABLE);
      $$->desc_table.relation_name = $2;
      free($2);
    }
    ;

create_index_stmt:    /*create index 语句的语法解析树*/
    CREATE unique_tag INDEX ID ON ID LBRACE rel_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = $$->create_index;
      create_index.unique_tag = $2;
      create_index.index_name = $4;
      create_index.relation_name = $6;

      if ($8 != nullptr) {
        create_index.attribute_name.swap(*$8);
        delete $8;
      }

      free($4);
      free($6);
    }
    ;

unique_tag:
    /* empty */
    {
      $$ = false;
    }
    | UNIQUE
    {
      $$ = true;
    }
    ;

drop_index_stmt:      /*drop index 语句的语法解析树*/
    DROP INDEX ID ON ID
    {
      $$ = new ParsedSqlNode(SCF_DROP_INDEX);
      $$->drop_index.index_name = $3;
      $$->drop_index.relation_name = $5;
      free($3);
      free($5);
    }
    ;
create_table_stmt:    /*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format
    {
      $$ = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = $$->create_table;
      create_table.relation_name = $3;
      free($3);

      std::vector<AttrInfoSqlNode> *src_attrs = $6;

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(*$5);
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete $5;
      if ($8 != nullptr) {
        create_table.storage_format = $8;
        free($8);
      }
    }
    ;
attr_def_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA attr_def attr_def_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<AttrInfoSqlNode>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
    
attr_def:
    ID type LBRACE number RBRACE null_option
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
      $$->nullable = $6;
      free($1);
    }
    | ID type null_option
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = 4;
      $$->nullable = $3;
      free($1);
    }
    ;

null_option:
    /* empty */
    {
      $$ = false;
    }
    | NULLABLE
    {
      $$ = true;
    }
    | NULL_TYPE
    {
      $$ = true;
    }
    | NOT NULL_TYPE
    {
      $$ = false;
    }
    ;
number:
    NUMBER {$$ = $1;}
    ;
type:
    INT_T      { $$ = static_cast<int>(AttrType::INTS); }
    | STRING_T { $$ = static_cast<int>(AttrType::CHARS); }
    | DATE_T   { $$ = static_cast<int>(AttrType::DATES); }
    | FLOAT_T  { $$ = static_cast<int>(AttrType::FLOATS); }
    | VECTOR_T { $$ = static_cast<int>(AttrType::VECTORS); }
    ;
insert_stmt:        /*insert   语句的语法解析树*/
    INSERT INTO ID VALUES LBRACE value value_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_INSERT);
      $$->insertion.relation_name = $3;
      if ($7 != nullptr) {
        $$->insertion.values.swap(*$7);
        delete $7;
      }
      $$->insertion.values.emplace_back(*$6);
      std::reverse($$->insertion.values.begin(), $$->insertion.values.end());
      delete $6;
      free($3);
    }
    ;

value_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA value value_list  { 
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Value>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
value:
    NUMBER {
      $$ = new Value((int)$1);
      @$ = @1;
    }
    | '-' NUMBER %prec UMINUS {
      $$ = new Value(-(int)$2);
      @$ = @2;
    }
    | FLOAT {
      $$ = new Value((float)$1);
      @$ = @1;
    }
    | '-' FLOAT %prec UMINUS {
      $$ = new Value(-(float)$2);
      @$ = @2;
    }
    |SSS {
      char *tmp = common::substr($1,1,strlen($1)-2);
      $$ = new Value(tmp);
      free(tmp);
      free($1);
    }
    | NULL_TYPE {
      $$ = new Value();
      $$->set_null();
    }
    ;
storage_format:
    /* empty */
    {
      $$ = nullptr;
    }
    | STORAGE FORMAT EQ ID
    {
      $$ = $4;
    }
    ;
    
delete_stmt:    /*  delete 语句的语法解析树*/
    DELETE FROM ID where 
    {
      $$ = new ParsedSqlNode(SCF_DELETE);
      $$->deletion.relation_name = $3;
      if ($4 != nullptr) {
        $$->deletion.conditions = $4;
      }
      free($3);
    }
    ;
update_stmt:      /*  update 语句的语法解析树*/
    UPDATE ID SET update_item_list where
    {
      $$ = new ParsedSqlNode(SCF_UPDATE);
      $$->update.relation_name = $2;
      for(auto &item: *$4) {
        $$->update.attribute_names.push_back(item.attr_name);
        $$->update.values.push_back(item.value);
      }
      std::reverse($$->update.attribute_names.begin(), $$->update.attribute_names.end());
      std::reverse($$->update.values.begin(), $$->update.values.end());
      if ($5 != nullptr) {
        $$->update.conditions = $5;
      }
      free($2);
    }
    ;
update_item_list:
    update_item
    {
        $$ = new std::vector<UpdateItem>;
        $$->emplace_back(*$1);
        delete $1;
    }
    | update_item COMMA update_item_list
    {
        if ($3 != nullptr) {
            $$ = $3;
        } else {
            $$ = new std::vector<UpdateItem>;
        }
        $$->emplace_back(*$1);
        delete $1;
    }
    ;

update_item:
    ID EQ expression
    {
      $$ = new UpdateItem;
      $$->attr_name = $1;
      $$->value = $3;
    }
    ;
select_stmt:        /*  select 语句的语法解析树*/
    SELECT expression_list
    {
      $$ = new ParsedSqlNode(SCF_SELECT);
      if ($2 != nullptr) {
        $$->selection.expressions.swap(*$2);
        delete $2;
      }
    }
    | SELECT expression_list FROM from_list where group_by order_by
    {
      $$ = new ParsedSqlNode(SCF_SELECT);
      if ($2 != nullptr) {
        $$->selection.expressions.swap(*$2);
        delete $2;
      }

      if ($4 != nullptr) {
        $$->selection.relations.swap(*$4);
        delete $4;
      }

      if ($5 != nullptr) {
        $$->selection.conditions = $5;
      }

      if ($6 != nullptr) {
        $$->selection.group_by.swap(*$6);
        delete $6;
      }

      if ($7 != nullptr) {
        $$->selection.order_by.swap(*$7);
        delete $7;
      }
    }
    ;

calc_stmt:
    CALC expression_list
    {
      $$ = new ParsedSqlNode(SCF_CALC);
      $$->calc.expressions.swap(*$2);
      delete $2;
    }
    ;

expression_list:
    expression alias
    {
      $$ = new std::vector<std::unique_ptr<Expression>>;
      if($2 != nullptr){
        $1 -> set_alias($2);
      }
      $$->emplace_back($1);
    }
    | expression alias COMMA expression_list
    {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<std::unique_ptr<Expression>>;
      }
      if($2 != nullptr){
        $1 -> set_alias($2);
      }
      $$->emplace($$->begin(), $1);
    }
    ;

alias:
    /* empty */ {
      $$ = nullptr;
    }
    | ID {
      $$ = $1;
    }
    | AS ID {
      $$ = $2;
    }
    ;

expression:
    expression '+' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::ADD, $1, $3, sql_string, &@$);
    }
    | expression '-' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::SUB, $1, $3, sql_string, &@$);
    }
    | expression '*' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::MUL, $1, $3, sql_string, &@$);
    }
    | expression '/' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::DIV, $1, $3, sql_string, &@$);
    }
    | LBRACE expression RBRACE {
      $$ = $2;
      $$->set_name(token_name(sql_string, &@$));
    }
    | '-' expression %prec UMINUS {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, $2, nullptr, sql_string, &@$);
    }
    | value {
      $$ = new ValueExpr(*$1);
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    }
    | rel_attr {
      RelAttrSqlNode *node = $1;
      $$ = new UnboundFieldExpr(node->relation_name, node->attribute_name);
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    }
    | '*' {
      $$ = new StarExpr();
    }
    | ID DOT '*'{
      $$ = new StarExpr($1);
    }
    | func_expr {
      $$ = $1; // FuncExpr
    }
    | aggr_func_expr {
      $$ = $1; // AggrFuncExpr
    }
    | sub_query_expr {
      $$ = $1; // SubQueryExpr
    }
    | value_list_expr {
      $$ = $1; // ValueListExpr
    }
    // your code here
    | L2_DISTANCE LBRACE expression COMMA expression RBRACE {
        $$ = create_arithmetic_expression(ArithmeticExpr::Type::LD, $3, $5, sql_string, &@$);
    }
    | COSINE_DISTANCE LBRACE expression COMMA expression RBRACE {
        $$ = create_arithmetic_expression(ArithmeticExpr::Type::CD, $3, $5, sql_string, &@$);
    }
    | INNER_PRODUCT LBRACE expression COMMA expression RBRACE {
        $$ = create_arithmetic_expression(ArithmeticExpr::Type::IP, $3, $5, sql_string, &@$);
    }
    ;

aggr_func_expr:
    ID LBRACE expression RBRACE
    {
      $$ = create_aggregate_expression($1, $3, sql_string, &@$);
    }
    ;

sub_query_expr:
    LBRACE select_stmt RBRACE
    {
      $$ = new SubQueryExpr(std::move($2->selection));
      delete $2;
    }
    ;

value_list_expr:
    LBRACE value value_list_ssq RBRACE
    {
      ValueListExpr* value_list_expr = new ValueListExpr();
      if($3 != nullptr){
        value_list_expr->get_value_list().swap(*$3);
        delete $3;
      }
      value_list_expr->get_value_list().emplace_back(*$2);
      std::reverse(value_list_expr->get_value_list().begin(), value_list_expr->get_value_list().end());
      delete $2;
      $$ = value_list_expr;
    }

value_list_ssq:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA value value_list_ssq  {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Value>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;

func_expr:
    func_type LBRACE expression_list RBRACE
    {
      $$ = new FuncExpr((FuncExpr::FuncType)$1,*$3);
      $$->set_name(token_name(sql_string, &@$));
      delete $3;
    }
    ;

func_type:
    LENGTH {
      $$ = FuncExpr::FuncType::LENGTH;
    }
    | ROUND {
      $$ = FuncExpr::FuncType::ROUND;
    }
    | DATE_FORMAT {
      $$ = FuncExpr::FuncType::DATE_FORMAT;
    }
    ;

rel_attr:
    ID {
      $$ = new RelAttrSqlNode;
      $$->attribute_name = $1;
      free($1);
    }
    | ID DOT ID {
      $$ = new RelAttrSqlNode;
      $$->relation_name  = $1;
      $$->attribute_name = $3;
      free($1);
      free($3);
    }
    ;

relation:
    ID {
      $$ = $1;
    }
    ;
rel_list:
    relation {
      $$ = new std::vector<std::string>();
      $$->push_back($1);
      free($1);
    }
    | relation COMMA rel_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<std::string>;
      }

      $$->insert($$->begin(), $1);
      free($1);
    }
    ;

relation_with_alias:
    relation alias{
        $$ = new TableWithAlias;
        $$->table_name = $1;
        if($2 != nullptr){
            $$->alias = $2;
        }
        else
        {
            $$->alias = "";
        }
    };

join_list:
    relation_with_alias {
      $$ = new InnerJoinSqlNode;
      $$->tables.push_back(*$1);
    }
    | join_list INNER JOIN relation_with_alias ON condition
    {
      $$ = $1;
      $$->tables.push_back(*$4);
      $$->conditions.push_back($6);
    }
    ;
from_list:
    join_list {
      $$ = new std::vector<InnerJoinSqlNode>();
      $$->push_back(*$1);
    }
    | join_list COMMA from_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<InnerJoinSqlNode>;
      }

      $$->insert($$->begin(), *$1);
    }
    ;

where:
    /* empty */
    {
      $$ = nullptr;
    }
    | WHERE condition {
      $$ = $2;  
    }
    ;
condition:
    expression comp_op expression {
      $$ = new ComparisonExpr($2, $1, $3);
    }
    | expression is_null_comp
    {
      ValueExpr *value_expr = new ValueExpr(Value(1));
      $$ = new ComparisonExpr($2 ? IS_NULL : IS_NOT_NULL, $1, value_expr);
    }
    | condition AND condition {
      $$ = new ConjunctionExpr(ConjunctionExpr::Type::AND, $1, $3);
    }
    | condition OR condition {
      $$ = new ConjunctionExpr(ConjunctionExpr::Type::OR, $1, $3);
    }
    ;

comp_op:
      EQ { $$ = EQUAL_TO; }
    | LT { $$ = LESS_THAN; }
    | GT { $$ = GREAT_THAN; }
    | LE { $$ = LESS_EQUAL; }
    | GE { $$ = GREAT_EQUAL; }
    | NE { $$ = NOT_EQUAL; }
    | LIKE_OP { $$ = LIKE; }
    | NOT LIKE_OP { $$ = NOT_LIKE; }
    | IN_OP {$$ = IN; }
    | NOT IN_OP {$$ = NOT_IN;}
    | EXISTS_OP {$$ = EXISTS;}
    | NOT EXISTS_OP {$$ = NOT_EXISTS; }
    ;

is_null_comp:
    IS NULL_TYPE
    {
      $$ = true;
    }
    | IS NOT NULL_TYPE
    {
      $$ = false;
    }
    ;

// your code here
group_by:
    /* empty */
    {
      $$ = nullptr;
    }
    ;

order_by:
    /* empty*/
    {
      $$ = nullptr;
    }
    | ORDER BY order_by_items
    {
      $$ = $3;
    }
    ;

order_by_items:
    order_by_item {
      $$ = new std::vector<OrderAttrSqlNode>();
      $$->push_back(*$1);
    }
    | order_by_item COMMA order_by_items
    {
        if($3 != nullptr) {
            $$ = $3;
        } else {
            $$ = new std::vector<OrderAttrSqlNode>;
        }

        $$->insert($$->begin(), *$1);
    }
    ;

order_by_item:
    rel_attr
    {
        $$ = new OrderAttrSqlNode;
        $$->attr = *$1;
        $$->is_asc = true;
    }
    | rel_attr ASC
    {
        $$ = new OrderAttrSqlNode;
        $$->attr = *$1;
        $$->is_asc = true;
    }
    | rel_attr DESC
    {
        $$ = new OrderAttrSqlNode;
        $$->attr = *$1;
        $$->is_asc = false;
    }
    ;
load_data_stmt:
    LOAD DATA INFILE SSS INTO TABLE ID 
    {
      char *tmp_file_name = common::substr($4, 1, strlen($4) - 2);
      
      $$ = new ParsedSqlNode(SCF_LOAD_DATA);
      $$->load_data.relation_name = $7;
      $$->load_data.file_name = tmp_file_name;
      free($7);
      free(tmp_file_name);
    }
    ;

explain_stmt:
    EXPLAIN command_wrapper
    {
      $$ = new ParsedSqlNode(SCF_EXPLAIN);
      $$->explain.sql_node = std::unique_ptr<ParsedSqlNode>($2);
    }
    ;

set_variable_stmt:
    SET ID EQ value
    {
      $$ = new ParsedSqlNode(SCF_SET_VARIABLE);
      $$->set_variable.name  = $2;
      $$->set_variable.value = *$4;
      free($2);
      delete $4;
    }
    ;

opt_semicolon: /*empty*/
    | SEMICOLON
    ;
%%
//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
