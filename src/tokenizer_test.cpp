#include "global.h"
#include "tokenizer.h"

int
main(void)
{
    {
        cout << "---PerLoc.input---" << endl;
        string str = textToString("../dataset/perloc/PerLoc.input");

        vector<TextToken> tokens = getTextTokens(str);

        for (auto token : tokens)
        {
            cout << token.token << ": [";
            cout << token.st << ", " << token.ed << ")" << endl;
        } 
    }

    {
        cout << "---Revenue.input---" << endl;
        string str = textToString("../dataset/Revenue/Revenue.input");

        vector<TextToken> tokens = getTextTokens(str);

        for (auto token : tokens)
        {
            cout << token.token << ": [";
            cout << token.st << ", " << token.ed << ")" << endl;
        } 
    }
    
    
    return 0;
}