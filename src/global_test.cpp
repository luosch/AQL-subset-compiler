#include "global.h"

int
main(void)
{
    string str = textToString("../dataset/PerLoc.aql");
    cout << str << endl;

    str = textToString("../dataset/perloc/PerLoc.input");
    cout << str << endl;

    cout << isWhite(' ') << " ";
    cout << isWhite('\n') << " ";
    cout << isWhite('\t') << " ";
    cout << isWhite('\r') << endl;

    clear(str);
    cout << (str == "") << endl;

    int number = 10;
    clear(number);
    cout << (number == 0) << endl;

    cout << lengthOfNum(10124) << endl;
    cout << lengthOfNum(0) << endl;

    cout << intToString(1203) << endl;
    cout << intToString(0) << endl;

    error("testing end");

    return 0;
}