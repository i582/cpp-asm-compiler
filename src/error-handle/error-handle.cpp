#include "error-handle.h"
#include "../lexer/lexer.h"

void error_handle::raise(error_handle_type error_type, const string& message, lexer* lex)
{
    string error_begin;

    switch (error_type)
    {
        case error_handle_type::LEXER:
        {
            error_begin = "Lexical error! ";
            break;
        }
        case error_handle_type::PARSER:
        {
            error_begin = "Syntax error! ";
            break;
        }
        case error_handle_type::SEMANTIC:
        {
            error_begin = "Semantic error! ";
            break;
        }
        case error_handle_type::CODE_GENERATION:
        {
            error_begin = "Code generation error! ";
            break;
        }
    }

    string error_message = error_begin + message;

    cout << error_message << endl;

    if (lex != nullptr)
    {
        lex->print_current_token_line();
    }

    throw logic_error(error_message);
}