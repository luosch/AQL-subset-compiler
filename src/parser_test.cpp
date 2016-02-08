#include "global.h"

#include "tokenizer.h"
#include "lexer.h"
#include "pattern.h"
#include "parser.h"

int
main(void)
{
    {
        cout << "---PerLoc.aql---" << endl;
        string input_text = textToString("../dataset/perloc/PerLoc.input");
        string aql_text = textToString("../dataset/PerLoc.aql");
        vector<AQLToken> aqlTokens = getAQLTokens(aql_text);
        parser(aqlTokens, input_text, "PerLoc.aql");
    }

    {
        cout << "---Revenue.aql---" << endl;
        string input_text = textToString("../dataset/revenue/Revenue.input");
        string aql_text = textToString("../dataset/Revenue.aql");
        vector<AQLToken> aqlTokens = getAQLTokens(aql_text);
        parser(aqlTokens, input_text, "Revenue.aql");
    }
    
    return 0;
}