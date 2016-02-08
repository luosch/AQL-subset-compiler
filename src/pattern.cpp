#include "pattern.h"

extern string textInString;
extern vector<TextToken> textTokens;
int size_textTokens;
int cnt;

vector<vector<int> >
findall(const char *regex, const char *content);

extern vector<AQLView> views;
extern map<string, int> viewsIndex;

AQLView matchPattern(vector<AQLAtom> atoms,
                     map<int, string> group_name,
                     int tmp_cnt)
{   
    
    AQLView view;
    cnt = tmp_cnt;
    while (tmp_cnt--) view.columns.push_back(AQLColumn());

    size_textTokens = textTokens.size();

    int size = textTokens.size(), i;
    for (i = 0; i < size; i++)
    {
    	vector<TextToken> tmpTokens;
    	dfs(atoms, 0, atoms.size(), group_name, tmpTokens, view, i);
    }
    // choose the columns in the group needed returned
    AQLView view_cut;
    AQLColumn tmp_column;
    for (map<int, string>::iterator it = group_name.begin();
    	 it != group_name.end(); it++)
    {
    	tmp_column = AQLColumn(it->second, view.columns[it->first].items);
    	view_cut.columns.push_back(tmp_column);
    }
    return view_cut;
}

void dfs(const vector<AQLAtom> &atoms, int kk, const int &size,
		 const map<int, string> &group_name,
         vector<TextToken> &tmpTokens, AQLView &view, int pos)
{
	int tmp_size = tmpTokens.size();
    
    if (pos >= size_textTokens && kk < size) return;
    
	int i, j, k;
    
    // all atoms matched
	if (kk == size)
    {
		vector<TextToken> *item;
		item = new vector<TextToken>[cnt+1];

		int size_tmpTokens = tmpTokens.size();
		int size_group_num;
		for (i = 0; i < size_tmpTokens; i++)
        {
		    size_group_num = tmpTokens[i].group_num.size();
			for (k = 0; k < size_group_num; k++)
            {
				item[tmpTokens[i].group_num[k]].push_back(tmpTokens[i]);
			}
		}

		for (i = 0; i < cnt; i++)
        {
			view.columns[i].items.push_back(item[i]);
		}
		delete []item;
	} else {
		// Reg
		if (atoms[kk].type == Reg)
        {
			for (i = atoms[kk].minn; i <= atoms[kk].maxx; i++)
            {
				if (pos+i-1 >= size_textTokens) return;

				vector<vector<int> > tmp = findall(atoms[kk].reg.c_str(), textTokens[pos+i-1].token.c_str());
				// the regex NOT matched the TextToken
				if (tmp.empty()
				 || tmp[0][0] != 0
				 || tmp[0][1] != textTokens[pos+i-1].token.length()) return;

				for (j = 0; j < i; j++)
                {
					TextToken tmp_textToken = textTokens[pos+j];
					tmp_textToken.group_num = atoms[kk].group_num;

					tmpTokens.push_back(tmp_textToken);
				}

				dfs(atoms, kk+1, size, group_name, tmpTokens, view, pos+i);

				while (tmpTokens.size() > tmp_size) tmpTokens.pop_back();
			}

			while (tmpTokens.size() > tmp_size) tmpTokens.pop_back();
		}
		// Column
		else if (atoms[kk].type == Column)
        {
			AQLView &tmp_view = views[viewsIndex[atoms[kk].viewID]];
			AQLColumn &tmp_column = tmp_view.columns[tmp_view.columnsIndex[atoms[k].columnID]];

			int tmp_pos,
				size_items = tmp_column.items.size(),
				size_item;

			for (j = 0; j < size_items; j++)
            {
				size_item = tmp_column.items[j].size();
				
				for (i = atoms[kk].minn; i <= atoms[kk].maxx; i++)
                {
					if (pos+size_item*i-1 >= size_textTokens) return;

					tmp_pos = pos;
					for (k = 0; k < size_item; k++)
                    {
						if (tmp_column.items[j][k].st != textTokens[tmp_pos++].st) break;
					}

					if (k < size_item) break;
					else
                    {
						tmp_pos = pos;
						for (k = 0; k < i; k++)
                        {
							for (int r = 0; r < size_item; r++)
                            {
								TextToken tmp_textToken = textTokens[tmp_pos++];
								tmp_textToken.group_num = atoms[kk].group_num;

								tmpTokens.push_back(tmp_textToken);
							}
						}
					}

					dfs(atoms, kk+1, size, group_name, tmpTokens, view, pos+i*size_item);

					while (tmpTokens.size() > tmp_size) tmpTokens.pop_back();
				}

				while (tmpTokens.size() > tmp_size) tmpTokens.pop_back();
			}
		}
		// Token
		else {
			if (pos+atoms[kk].minn-1 >= size_textTokens) return;

			for (i = atoms[kk].minn; i <= atoms[kk].maxx; i++)
            {
				if (pos+i-1 >= size_textTokens) return;

				for (j = 0; j < i; j++)
                {
					TextToken tmp_textToken = textTokens[pos+j];
					tmp_textToken.group_num = atoms[kk].group_num;

					tmpTokens.push_back(tmp_textToken);
				}

				dfs(atoms, kk+1, size, group_name, tmpTokens, view, pos+i);

				while (tmpTokens.size() > tmp_size) tmpTokens.pop_back();
			}

			while (tmpTokens.size() > tmp_size) tmpTokens.pop_back();
		}
	}
}

