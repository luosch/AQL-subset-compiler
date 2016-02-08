#include "head.h"
#include "global.h"

#include "tokenizer.h"
#include "lexer.h"
#include "parser.h"

#include <dirent.h>

int
main(int argc, char const *argv[])
{
    if (argc == 3)
    {
        string aql_file = string(argv[1]);
        string aql_text = textToString(aql_file);
        vector<AQLToken> aqlTokens = getAQLTokens(aql_text);
        string input_source = string(argv[2]);
        string input_text = textToString(input_source);

        if (input_text.length() <= 0) // input source is a directory
        {
            if (input_source[input_source.length() - 1] != '/')
            {
                input_source += '/';
            }
            DIR *dirp = opendir(input_source.c_str());
            struct dirent *dp;
            if (dirp == NULL)
            {
                printf("%s\n", "ERROR: no such directory");
                return 0;
            }
            while ((dp = readdir(dirp)) != NULL)
            {
                string file_name = string(dp->d_name);
                if (file_name.find(".input") != string::npos \
                    || file_name.find(".txt") != string::npos)
                {
                    string file_path = input_source + file_name;
                    string input_text = textToString(file_path);
                    parser(aqlTokens, input_text, file_path);
                }
            }
            closedir(dirp);
        }
        else // input source is a file
        {
            if (input_source.find(".input") != string::npos \
                || input_source.find(".txt") != string::npos)
            {
                string input_text = textToString(input_source);
                parser(aqlTokens, input_text, input_source);
            }
        }
    }
    else
    {
        printf("%s\n", "ERROR: must have exactly two parameters");
    }
    return 0;
}
