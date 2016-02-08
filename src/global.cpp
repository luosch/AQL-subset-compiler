#include "global.h"

string
textToString(string file) {
    ifstream in(file.c_str());
    
    string str = "", s;
    while (getline(in, s)) {
        // add '\n' to have a multiple line string
        str += s + "\n";
    }

    in.close();
    return str;
}

// judge if it is a blank character
bool
isWhite(const char &c)
{
    return c == ' ' \
        || c == '\n' \
        || c == '\t' \
        || c == '\r';
}

// show error message
void
error(string msg)
{
    printf("Error: %s\n", msg.c_str());
    exit(0);
}

void clear(string &s) { s = ""; }
void clear(int &a)    { a = 0;  }

int
lengthOfNum(int num)
{
    if (num == 0)
    {
        return 1;
    }
    else if (num < 0)
    {
        num = -num;
    }

    int cnt = 0;

    while (num)
    {
        cnt++;
        num /= 10;
    }

    return cnt;
}

// int to string
string
intToString(int num)
{
    string str = "";
    if (num == 0)
    {
        return string("0");
    }
    else if (num < 0)
    {
        str += '-';
        num = -num;
    }
    while (num)
    {
        str = char(num % 10 + '0') + str;
        num /= 10;
    }
    return str;
}
