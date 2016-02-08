#ifndef LEXER_H
#define LEXER_H

#include "head.h"

enum AQLType
{
    CREATE,             // "create"
    VIEW,               // "view"
    AS,                 // "as"
    OUTPUT,             // "output"
    SELECT,             // "select"
    FROM,               // "from"
    EXTRACT,            // "extract"
    REGEX,              // "regex"
    ON,                 // "on"
    RETURN,             // "return"
    GROUP,              // "group"
    AND,                // "and"
    TOKEN,              // "Token"
    PATTERN,            // "pattern"
    
    DOT,                // "."
    COMMA,              // ","
    SEMICOLON,          // ";"
    LEFT_ROUND_BRACKET, // "("
    RIGHT_ROUND_BRACKET,// ")"
    LEFT_ANGLE_BRACKET, // "<"
    RIGHT_ANGLE_BRACKET,// ">"
    LEFT_CURLY_BRACKET, // "{"
    RIGHT_CURLY_BRACKET, // "}"

    REG,                // "/.../"
    NUM,                // "1234567890"
    ID,                 // other string as ID
};

class AQLToken
{
  public:
    enum AQLType type;
    string str;
    int num;

    AQLToken() {}
    AQLToken(enum AQLType type) :
        type(type) {}
    AQLToken(enum AQLType type, string str) :
        type(type), str(str) {}
    AQLToken(enum AQLType type, int num) :
        type(type), num(num) {}
};

// extract tokens in AQL text
vector<AQLToken>
getAQLTokens(string AQLText);

// type to string
string
AQLTypeToString(const enum AQLType &type);

#endif

