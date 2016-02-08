#ifndef PARSER_H
#define PARSER_H

#include "head.h"
#include "tokenizer.h"
#include "global.h"
#include "lexer.h"
#include "pattern.h"

class AQLColumn
{
  public:
	string name;
	vector<vector<TextToken> > items;

	AQLColumn() {}
	AQLColumn(string name) :
		name(name) {}
	AQLColumn(string name, vector<vector<TextToken> > items) :
		name(name), items(items) {}
};

class AQLView
{
  public:
	string name;
	vector<AQLColumn> columns;
	map<string, int> columnsIndex;

	AQLView() {}
	AQLView(string name) :
		name(name) {}
	AQLView(string name, vector<AQLColumn> columns) :
		name(name), columns(columns) {}

	void makeIndex() {
		int size = columns.size(), i;
		for (i = 0; i < size; i++)
			columnsIndex[columns[i].name] = i;
	}
};

// clear the views and its index
void init();

// add a new view into views
void addView(AQLView view);

// load text as a view
void loadText(const string &text);

void parser(vector<AQLToken> tokens,
            const string &text,
            string aqlFile_name);

// aql_stmt -> create_stmt; | output_stmt;
void aql_stmt(vector<AQLToken> &tokens);

// create view ID as ...
void create_stmt(vector<AQLToken> &tokens);

// view_stmt -> select_stmt | extract_stmt
AQLView
view_stmt(vector<AQLToken> &tokens);

// select select_list from from_list
AQLView
select_stmt(vector<AQLToken> &tokens);

// from_list -> from_item | from_list,from_item
// from_item -> ID ID
map<string, string>
from_list(vector<AQLToken> &tokens);

// select_list -> select_item | select_list,select_item
// select_item -> ID.ID | ID.ID as ID
AQLView
select_list(vector<AQLToken> &tokens,
            map<string, string> &abb);

// extract_stmt -> extract extract_spec from from_list
AQLView
extract_stmt(vector<AQLToken> &tokens);

// extract_spec -> regex_spec | pattern_spec
AQLView
extract_spec(vector<AQLToken> &tokens,
             map<string, string> &abb);

// regex_spec -> regex REG on column name_spec
// column -> ID.ID
// name_spec -> as ID | return group_spec
AQLView
regex_spec(vector<AQLToken> &tokens,
           map<string, string> &abb);

// group_spec -> single_group | group_spec and single_group
// sinble_group -> group NUM as ID
map<int, string>
group_spec(vector<AQLToken> &tokens);

// pattern_spec -> pattern_expr name_spec
// name_spec -> as ID | return group_spec
AQLView
pattern_spec(vector<AQLToken> &tokens,
             map<string, string> &abb);

// pattern_expr -> pattern_pkg | pattern_expr patter_pkg
// pattern_pkg -> atom | atom{NUM,NUM} | pattern_group
// atom -> <column> | <Token> | REG
// pattern_group -> (pattern_expr)
AQLView
pattern_expr(vector<AQLToken> &tokens,
             map<int, string> &group_name,
             map<string, string> &abb);

// output_stmt -> output view ID
//				| output view ID as ID
void output_stmt(const vector<AQLToken> &tokens);

void printLine(const vector<int> &widths,
               const int &size);

void printName(const vector<int> &widths,
               const int &size,
               const AQLView &view);

void printItem(const vector<int> &widths,
               const int &size_col,
               const AQLView &view);

#endif

