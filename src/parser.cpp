#include "parser.h"

vector<AQLView> views;
map<string, int> viewsIndex;

string textInString;
vector<TextToken> textTokens;

string inputFile;

extern vector< vector<int> >
findall(const char *regex, const char *content);

// clear the views and its index
void init()
{
	while (!views.empty())
		views.pop_back();
	viewsIndex.clear();
}

// add a new view into views
void addView(AQLView view)
{
    // add indexs for the columns' names
	view.makeIndex();

	viewsIndex[view.name] = views.size();
	views.push_back(view);
}

// load text as a view
void loadText(const string &text)
{
	// save the text in the parser
	textInString = text;

	textTokens = getTextTokens(text);

	AQLColumn textColumn("text");
	int size = textTokens.size(), i;
	for (i = 0; i < size; i++) {
		vector<TextToken> item;
		item.push_back(textTokens[i]);

		textColumn.items.push_back(item);
	}
    
    vector<AQLColumn> columns;
    columns.push_back(textColumn);
	AQLView textView = AQLView("Document", columns);

	addView(textView);
}

void parser(vector<AQLToken> tokens,
            const string &text,
            string aqlFile_name)
{
	init();

	loadText(text);
	
	inputFile = aqlFile_name;

	printf("Processing %s\n", inputFile.c_str());

	int size = tokens.size(), i;
	for (i = 0; i < size; i++)
	{
		vector<AQLToken> aql_tokens;

		// split the AQL statements
		for (; tokens[i].type != SEMICOLON; i++)
		{
		    aql_tokens.push_back(tokens[i]);
		}
		aql_tokens.push_back(tokens[i]);

		aql_stmt(aql_tokens);
	}
}

// aql_stmt -> create_stmt; | output_stmt;
void aql_stmt(vector<AQLToken> &tokens)
{
	if     (tokens[0].type == CREATE)   create_stmt(tokens);
	else /* tokens[0].type == OUTPUT */ output_stmt(tokens);
}

// create view ID as ...
void create_stmt(vector<AQLToken> &tokens)
{
	string view_name = tokens[2].str;

	vector<AQLToken> view_tokens;
	int size = tokens.size(), i;
	for (i = 4; i < size; i++)
		view_tokens.push_back(tokens[i]);

	AQLView view = view_stmt(view_tokens);
	view.name = view_name;

	addView(view);
}

// view_stmt -> select_stmt | extract_stmt
AQLView view_stmt(vector<AQLToken> &tokens)
{
	if     (tokens[0].type == SELECT)   return select_stmt(tokens);
	else /* tokens[0].type == EXTRACT */return extract_stmt(tokens);
}

// select select_list from from_list
AQLView select_stmt(vector<AQLToken> &tokens)
{
	int i;

	// save the tokens of select_list for later selection
	vector<AQLToken> select_list_tokens;
	for (i = 1; tokens[i].type != FROM; i++)
		select_list_tokens.push_back(tokens[i]);

	// get from items from from list,
	// but here there should be only ONE item
	vector<AQLToken> from_list_tokens;
	for (i++; tokens[i].type != SEMICOLON; i++)
		from_list_tokens.push_back(tokens[i]);
		
	map<string, string> from_items = from_list(from_list_tokens);

	return select_list(select_list_tokens, from_items);
}

// from_list -> from_item | from_list,from_item
// from_item -> ID ID
map<string, string> from_list(vector<AQLToken> &tokens)
{
	map<string, string> abb;

	int size = tokens.size(), i;
	for (i = 0; i < size; i++) {
		if (tokens[i].type == COMMA
		 || tokens[i].type == SEMICOLON) continue;

		abb[tokens[i+1].str] = tokens[i].str;
		i++;
	}

	return abb;
}

// select_list -> select_item | select_list,select_item
// select_item -> ID.ID | ID.ID as ID
AQLView select_list(vector<AQLToken> &tokens,
                    map<string, string> &abb)
{
	AQLView view;

	int size = tokens.size(), i;
	for (i = 0; i < size; i++)
	{
		if (tokens[i].type == COMMA) continue;
		string viewID = tokens[i].str,
			   columnID = tokens[i+2].str;
		i += 2;

		AQLColumn column = views[viewsIndex[abb[viewID]]].
						   columns[views[viewsIndex[abb[viewID]]].columnsIndex[columnID]];

		// select_item -> ID.ID as ID
		if (tokens[i+1].type == AS)
		{
			column.name = tokens[i+2].str;
			i += 2;
		}

		// add this column into the new view
		view.columns.push_back(column);
	}

	return view;
}

// extract_stmt -> extract extract_spec from from_list
AQLView extract_stmt(vector<AQLToken> &tokens)
{
	int i;

	// save the tokens of extract_spec for later extraction
	vector<AQLToken> extract_spec_tokens;
	for (i = 1; tokens[i].type != FROM; i++)
		extract_spec_tokens.push_back(tokens[i]);

	// get from items from from list
	vector<AQLToken> from_list_tokens;
	for (i++; tokens[i].type != SEMICOLON; i++)
		from_list_tokens.push_back(tokens[i]);
	map<string, string> from_items = from_list(from_list_tokens);

	return extract_spec(extract_spec_tokens, from_items);
}

// extract_spec -> regex_spec | pattern_spec
AQLView extract_spec(vector<AQLToken> &tokens,
                     map<string, string> &abb)
{
	if 	   (tokens[0].type == REGEX)     return regex_spec(tokens, abb);
	else /* tokens[0].type == PATTERN */ return pattern_spec(tokens, abb);
}

// regex_spec -> regex REG on column name_spec
// column -> ID.ID
// name_spec -> as ID | return group_spec
AQLView regex_spec(vector<AQLToken> &tokens,
                   map<string, string> &abb)
{
	// because "extract regex" only use on the Document.text",
	// we don't need to care the from list or the column,
	// just caring the Tokens after RETURN
	string regex = tokens[1].str;

	map<int, string> group_name;

	// name_spec -> as ID
	if (tokens[6].type == AS)
	{
		group_name[0] = tokens[7].str;
	}
	// name_spec -> return group_spec
	else
	{
		int size = tokens.size(), i;

		// get group_spec tokens from regex_spec
		vector<AQLToken> group_spec_tokens;
		for (i = 7; i < size; i++)
		{
			group_spec_tokens.push_back(tokens[i]);
		}

		/* map<int, string> */
		group_name = group_spec(group_spec_tokens);
	}

	// promise that the regex are supposed to match WHOLE TextToken(s)
	vector<vector<int> > pos_items = findall(regex.c_str(), textInString.c_str());

	AQLView view;
	int size1 = pos_items.size(), size2 = pos_items[0].size(), i, j;
	for (j = 0; j < size2; j += 2)
	{
		// whether the group is returned
		if (group_name.find(j>>1) == group_name.end()) continue;

		AQLColumn column(group_name[j>>1]);
		for (i = 0; i < size1; i++)
		{
			vector<TextToken> item;
			string tmp_str = textInString.substr(pos_items[i][j], pos_items[i][j+1]-pos_items[i][j]);
			item = getTextTokens(tmp_str);

			int size_item = item.size(), k;
			for (k = 0; k < size_item; k++)
			{
				item[k].st += pos_items[i][j];
				item[k].ed += pos_items[i][j];
			}

			column.items.push_back(item);
		}

		view.columns.push_back(column);
	}

	return view;
}

// group_spec -> single_group | group_spec and single_group
// sinble_group -> group NUM as ID
map<int, string> group_spec(vector<AQLToken> &tokens)
{
	map<int, string> group_name;

	int size = tokens.size(), i;
	for (i = 0; i < size; i += 5)
	{
		group_name[tokens[i+1].num] = tokens[i+3].str;
	}

	return group_name;
}

// pattern_spec -> pattern_expr name_spec
// name_spec -> as ID | return group_spec
AQLView pattern_spec(vector<AQLToken> &tokens,
                     map<string, string> &abb)
{    
	int size = tokens.size(), i;

	vector<AQLToken> pattern_expr_tokens;
	for (i = 1; i < size; i++)
	{
		if (tokens[i].type == AS
		 || tokens[i].type == RETURN) break;

		pattern_expr_tokens.push_back(tokens[i]);
	}

	map<int, string> group_name;

	// name_spec -> as ID
	if (tokens[i].type == AS)
	{
		group_name[0] = tokens[i+1].str;
	}
	// name_spec -> return group_spec
	else
	{
		vector<AQLToken> group_spec_tokens;
		for (i++; i < size; i++)
		{
			group_spec_tokens.push_back(tokens[i]);
		}

		group_name = group_spec(group_spec_tokens);
	}
	return pattern_expr(pattern_expr_tokens, group_name, abb);
}

// pattern_expr -> pattern_pkg | pattern_expr patter_pkg
// pattern_pkg -> atom | atom{NUM,NUM} | pattern_group
// atom -> <column> | <Token> | REG
// pattern_group -> (pattern_expr)
AQLView pattern_expr(vector<AQLToken> &tokens,
                     map<int, string> &group_name,
                     map<string, string> &abb)
{
	vector<int> cur_group_num;

	vector<AQLAtom> atoms;

	int cnt = 0;
	cur_group_num.push_back(cnt++);

	int size = tokens.size(), i;
	for (i = 0; i < size; i++)
	{
		// "("
		if (tokens[i].type == LEFT_ROUND_BRACKET)
		{
			cur_group_num.push_back(cnt++);
		}
		
		// ")"
		else if (tokens[i].type == RIGHT_ROUND_BRACKET)
		{
			cur_group_num.pop_back();
		}
		
		// "<"
		else if (tokens[i].type == LEFT_ANGLE_BRACKET)
		{
			// <Token>
			if (tokens[i+1].type == TOKEN)
			{
				// <Token>{min,max}
				if (i+3 < size && tokens[i+3].type == LEFT_CURLY_BRACKET)
				{
					int minn = tokens[i+4].num,
						maxx = tokens[i+6].num;
					atoms.push_back(AQLAtom(Token,
                                            minn, maxx,
                                            cur_group_num));

					i += 7;
				}
				// <Token>
				else
				{
					atoms.push_back(AQLAtom(Token, cur_group_num));

					i += 2;
				}
			}

			// <column> -> <ID.ID>
			else
			{
				string viewID   = tokens[i+1].str,
					   columnID = tokens[i+3].str;
				// <ID.ID>{min,max}
				if (i+5 < size && tokens[i+5].type == LEFT_CURLY_BRACKET)
				{
					int minn     = tokens[i+6].num,
						maxx     = tokens[i+8].num;
					atoms.push_back(AQLAtom(Column,
                                            abb[viewID], columnID,
                                            minn, maxx,
                                            cur_group_num));

					i += 9;
				}
				// <ID.ID>
				else
				{
					atoms.push_back(AQLAtom(Column,
                                            abb[viewID], columnID,
                                            cur_group_num));

					i += 4;
				}
			}
		}
		// /REG/
		else
		{
			// /REG/{min,max}
			if (i+1 < size && tokens[i+1].type == LEFT_CURLY_BRACKET)
			{
				int minn = tokens[i+1].num,
					maxx = tokens[i+3].num;
				atoms.push_back(AQLAtom(Reg, tokens[i].str,
                                        minn, maxx,
                                        cur_group_num));

				i += 5;
			}	
			// /REG/
			else
			{
				atoms.push_back(AQLAtom(Reg, tokens[i].str,
                                        cur_group_num));
			}
		}
	}

	return matchPattern(atoms, group_name, cnt);
}

// output_stmt -> output view ID
//				| output view ID as ID
void output_stmt(const vector<AQLToken> &tokens)
{
	string view_name = tokens[3].type == AS ?
					   tokens[4].str : tokens[2].str;
	printf("View: %s\n", view_name.c_str());

	if (viewsIndex.find(tokens[2].str) == viewsIndex.end())
	{
		puts("Empty set\n");
		return;
	}

	AQLView view = views[viewsIndex[tokens[2].str]];
	if (view.columns.empty())
	{
		puts("Empty set\n");
		return;
	}

	int size_col = view.columns.size(),
		size_row = view.columns[0].items.size(),
		size_items,
		i, j, k,
		item_st, item_ed;

	vector<int> widths_of_columns;

	for (j = 0; j < size_col; j++)
	{
		widths_of_columns.push_back(view.columns[j].name.length());

		for (i = 0; i < size_row; i++)
		{
			size_items = view.columns[j].items[i].size();

			item_st = view.columns[j].items[i][0].st;
			item_ed = view.columns[j].items[i][size_items-1].ed;

			int len = lengthOfNum(item_st) + lengthOfNum(item_ed) + 4 + item_ed - item_st;

			if (len > widths_of_columns[j])
				widths_of_columns[j] = len;
		}
	}

	printLine(widths_of_columns, size_col);
	printName(widths_of_columns, size_col, view);
	printLine(widths_of_columns, size_col);
	printItem(widths_of_columns, size_col, view);
	printLine(widths_of_columns, size_col);

	printf("%d %s in set\n\n", size_row, size_row ? "rows" : "row");
}

void printLine(const vector<int> &widths, const int &size)
{
	int i, j;
	for (j = 0; j < size; j++)
	{
		printf("+-");
		for (i = 0; i <= widths[j]; i++)
			putchar('-');
	}
	puts("+");
}

void printName(const vector<int> &widths, const int &size, const AQLView &view) {
	int i, j;
	for (j = 0; j < size; j++)
	{
		printf("| %s ", view.columns[j].name.c_str());
		for (i = widths[j]-view.columns[j].name.length(); i > 0; i--)
			putchar(' ');
	}
	puts("|");
}

void printItem(const vector<int> &widths, const int &size_col, const AQLView &view) {
	int i, j, k,
		size_row = view.columns[0].items.size(),
		size_items,
		item_st, item_ed;
		
	string tmp;
	for (i = 0; i < size_row; i++)
	{
		for (j = 0; j < size_col; j++)
		{
			tmp = "";
			size_items = view.columns[j].items[i].size();
			item_st = view.columns[j].items[i][0].st;
			item_ed = view.columns[j].items[i][size_items-1].ed;
			tmp += textInString.substr(item_st, item_ed - item_st);

			tmp += ":(";
			tmp += intToString(item_st);
			tmp += ",";
			tmp += intToString(item_ed);
			tmp += ")";

			printf("| %s", tmp.c_str());
			for (k = widths[j]-tmp.length(); k >= 0; k--)
				putchar(' ');
		}
		puts("|");
	}
}
