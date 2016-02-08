#ifndef PATTERN_H
#define PATTERN_H

#include "head.h"
#include "parser.h"
#include "tokenizer.h"

enum AtomType
{
	Reg, Column, Token
};

class AQLAtom
{
  public:
	enum AtomType type;

    string reg;
    string viewID, columnID;

    vector<int> group_num;

    int minn, maxx;

    AQLAtom() {}
    AQLAtom(enum AtomType type,
    		string reg,
            const vector<int> &group_num)
    {
    	// type == Reg
    	this->type = type;
    	this->reg  = reg;
        this->minn = 1;
        this->maxx = 1;
        this->group_num = group_num;
    }
    AQLAtom(enum AtomType type,
    		string reg,
    		int minn,
    		int maxx,
            const vector<int> &group_num)
    {
    	// type == Reg
    	this->type = type;
    	this->reg  = reg;
    	this->minn = minn;
    	this->maxx = maxx;
        this->group_num = group_num;
    }
    AQLAtom(enum AtomType type,
    		string viewID,
    		string columnID,
            const vector<int> &group_num)
    {
    	// type == Column
    	this->type     = type;
    	this->viewID   = viewID;
    	this->columnID = columnID;
        this->minn     = 1;
        this->maxx     = 1;
        this->group_num = group_num;
    }
    AQLAtom(enum AtomType type,
    		string viewID,
    		string columnID,
    		int minn,
    		int maxx,
            const vector<int> &group_num)
    {
    	// type == Column
    	this->type     = type;
    	this->viewID   = viewID;
    	this->columnID = columnID;
    	this->minn     = minn;
    	this->maxx     = maxx;
        this->group_num = group_num;
    }
    AQLAtom(enum AtomType type,
            const vector<int> &group_num)
    {
    	// type == Token
    	this->type = type;
        this->minn = 1;
        this->maxx = 1;
        this->group_num = group_num;
    }
    AQLAtom(enum AtomType type,
    		int minn,
    		int maxx,
            const vector<int> &group_num)
    {
    	// type == Token
    	this->type = type;
    	this->minn = minn;
    	this->maxx = maxx;
        this->group_num = group_num;
    }
};

class AQLView;

AQLView matchPattern(vector<AQLAtom> atoms,
                     map<int, string> group_name,
                     int cnt);

void dfs(const vector<AQLAtom> &atoms, int k, const int &size,
         const map<int, string> &group_name,
         vector<TextToken> &tmpTokens, AQLView &view,
         int pos);

#endif

