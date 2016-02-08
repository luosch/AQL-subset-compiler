#include "global.h"
#include "tokenizer.h"

// extract TextTokens from string text
vector<TextToken>
getTextTokens(const string &text)
{
    vector<TextToken> tokens;
    
    string tmp_str = "";
    
    int len = text.length(), i, st = -1;
        
    for (i = 0; i < len; i++)
    {
        // ignore blank character
        if (isWhite(text[i])) continue;
        
        // digits
        else if (isdigit(text[i]))
        {
            if (st == -1) st = i;
            tmp_str += text[i];
            
            if (!isdigit(text[i+1]) || isWhite(text[i+1]))
            {
                tokens.push_back(TextToken(tmp_str, st));
                clear(tmp_str);
                st = -1;
            }
        }
        
        // alpha
        else if (isalpha(text[i]))
        {
            if (st == -1) st = i;
            tmp_str += text[i];
            
            if (!isalpha(text[i+1]) || isWhite(text[i+1]))
            {
                tokens.push_back(TextToken(tmp_str, st));
                clear(tmp_str);
                st = -1;
            } 
        }
        
        // other charactors
        else
        {
            tmp_str += text[i];
            tokens.push_back(TextToken(tmp_str, i));
            clear(tmp_str);
        }
    }
    
    return tokens;
}

