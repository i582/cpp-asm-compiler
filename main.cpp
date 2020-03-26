#include "src/lexer/lexer.h"

int main()
{
    try
    {
        lexer lex("test.cpp");
        lex.split();
        lex.merge();
        lex.print();
    }
    catch (logic_error& error)
    {
        cout << error.what() << endl;
    }


    return 0;
}
