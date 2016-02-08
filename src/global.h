#ifndef GLOBAL_H
#define GLOBAL_H

#include "head.h"

// open file and store the content to a string
string
textToString(string file);

// judge if it is a blank character
bool
isWhite(const char &c);

// show error message
void
error(string msg);

// string = "" 
void
clear(string &s);

// int = 0
void
clear(int &a);

int
lengthOfNum(int a);

// int to string
string
intToString(int a);

#endif
