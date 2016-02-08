#include "global.h"
#include "lexer.h"

int
main(void)
{
    {
        cout << "---PerLoc.aql---" << endl;
        string str = textToString("../dataset/PerLoc.aql");

        for (auto token : getAQLTokens(str))
        {
            cout << AQLTypeToString(token.type) << endl;
        } 
    }

    {
        cout << "---Revenue.aql---" << endl;
        string str = textToString("../dataset/Revenue.aql");

        for (auto token : getAQLTokens(str))
        {
            cout << AQLTypeToString(token.type) << endl;
        }
    }
    
    return 0;
}