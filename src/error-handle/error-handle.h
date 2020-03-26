#pragma once

#include <string>
#include <iostream>

using namespace std;

class lexer;

enum class error_handle_type
{
    LEXER,
    PARSER,
    SEMANTIC,
    CODE_GENERATION
};

class error_handle
{
public:
    static void raise(error_handle_type error_type, const string& message, lexer* lex = nullptr);

};


