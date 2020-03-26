#pragma once
#include <iostream>
#include <string>
using namespace std;

enum class token_type
{
    IDENTIFIER,


    INTEGER_CONST,
    DOUBLE_CONST,
    STRING_CONST,
    CHAR_CONST,

    // boolean constant
    TRUE,
    FALSE,




    CONST,


    // types
    UNDEFINED,

    INT,
    DOUBLE,
    BOOL,
    CHAR,
    VOID,
    AUTO,

    // cycles
    DO_WHILE,
    WHILE,
    FOR,
    // cycles addition
    BREAK,
    CONTINUE,


    SWITCH,
    CASE,
    DEFAULT,


    // conditions
    IF,
    ELSE,


    // relationship operators
    LESS, // <
    GREATER, // >
    LESS_EQUAL, // <=
    GREATER_EQUAL, // >=


    // equal operators
    EQUAL, // ==
    NOT_EQUAL, // !=


    // logical operators
    AND, // &&
    OR, // ||
    EXCLAMATION, // !

    // math operators
    PLUS, // +
    MINUS, // -
    STAR, // *
    SLASH, // /
    INC, // ++
    DEC, // --


    // brackets
    LBRA, // {
    RBRA, // }
    LPAR, // (
    RPAR, // )
    LSQR, // [
    RSQR, // ]


    // assign
    ASSIGN, // =
    ADD_ASSIGN, // +=
    SUB_ASSIGN, // -=
    MUL_ASSIGN, // *=
    DIV_ASSIGN, // /=


    // function
    FUNCTION,
    RETURN,




    // other symbols
    SEMICOLON, // ;
    COLON, // :
    COMMA, // ,
    POINT, // .
    QUESTION, // ?
    AMPERSAND, // &

    // comment
    LINE_COMMENT, // //
    BLOCK_COMMENT_START, // /*
    BLOCK_COMMENT_END, // */


    NEW,
    DELETE,


    PREPROCESSOR_DIRECTIVE,


    ACCESS_OPERATOR, // ::
};

class token
{
public:
   token_type type;
   string lexeme;

    size_t line;
    size_t pos;

public:
    explicit token(const string& lexeme, size_t line = 0, size_t pos = 0)
    {
        this->lexeme = lexeme;
        this->type = which_token_type(lexeme);

        this->line = line;
        this->pos = pos;
    }


    static token_type which_token_type(const string& lexeme)
    {
        if (lexeme == "const")
            return token_type::CONST;

        // types
        if (lexeme == "int")
            return token_type::INT;
        if (lexeme == "double")
            return token_type::DOUBLE;
        if (lexeme == "bool")
            return token_type::BOOL;
        if (lexeme == "void")
            return token_type::VOID;
        if (lexeme == "char")
            return token_type::CHAR;
        if (lexeme == "auto")
            return token_type::AUTO;

        // cycles
        if (lexeme == "for")
            return token_type::FOR;
        if (lexeme == "while")
            return token_type::WHILE;
        if (lexeme == "do")
            return token_type::DO_WHILE;
        // cycles addition
        if (lexeme == "break")
            return token_type::BREAK;
        if (lexeme == "continue")
            return token_type::CONTINUE;



        // conditions
        if (lexeme == "if")
            return token_type::IF;
        if (lexeme == "else")
            return token_type::ELSE;



        // relationship operators
        if (lexeme == ">")
            return token_type::GREATER;
        if (lexeme == "<")
            return token_type::LESS;
        if (lexeme == "<=")
            return token_type::LESS_EQUAL;
        if (lexeme == ">=")
            return token_type::GREATER_EQUAL;



        // equal operators
        if (lexeme == "==")
            return token_type::EQUAL;
        if (lexeme == "!=")
            return token_type::NOT_EQUAL;



        // logical operators
        if (lexeme == "&&")
            return token_type::AND;
        if (lexeme == "||")
            return token_type::OR;
        if (lexeme == "!")
            return token_type::EXCLAMATION;


        // math operators
        if (lexeme == "+")
            return token_type::PLUS;
        if (lexeme == "-")
            return token_type::MINUS;
        if (lexeme == "*")
            return token_type::STAR;
        if (lexeme == "/")
            return token_type::SLASH;
        if (lexeme == "++")
            return token_type::INC;
        if (lexeme == "--")
            return token_type::DEC;


        // brackets
        if (lexeme == "{")
            return token_type::LBRA;
        if (lexeme == "}")
            return token_type::RBRA;
        if (lexeme == "(")
            return token_type::LPAR;
        if (lexeme == ")")
            return token_type::RPAR;
        if (lexeme == "[")
            return token_type::LSQR;
        if (lexeme == "]")
            return token_type::RSQR;


        // assign
        if (lexeme == "=")
            return token_type::ASSIGN;
        if (lexeme == "+=")
            return token_type::ADD_ASSIGN;
        if (lexeme == "-=")
            return token_type::SUB_ASSIGN;
        if (lexeme == "*=")
            return token_type::MUL_ASSIGN;
        if (lexeme == "/=")
            return token_type::DIV_ASSIGN;



        // function
        if (lexeme == "return")
            return token_type::RETURN;



        // new
        if (lexeme == "new")
            return token_type::NEW;
        if (lexeme == "delete")
            return token_type::DELETE;



        // boolean constants
        if (lexeme == "true")
            return token_type::TRUE;
        if (lexeme == "false")
            return token_type::FALSE;



        if (lexeme == "switch")
            return token_type::SWITCH;
        if (lexeme == "case")
            return token_type::CASE;
        if (lexeme == "default")
            return token_type::DEFAULT;



        // other symbols
        if (lexeme == ";")
            return token_type::SEMICOLON;
        if (lexeme == ":")
            return token_type::COLON;
        if (lexeme == ",")
            return token_type::COMMA;
        if (lexeme == ".")
            return token_type::POINT;
        if (lexeme == "?")
            return token_type::QUESTION;
        if (lexeme == "::")
            return token_type::ACCESS_OPERATOR;
        if (lexeme == "&")
            return token_type::AMPERSAND;


        // comment
        if (lexeme == "//")
            return token_type::LINE_COMMENT;
        if (lexeme == "/*")
            return token_type::BLOCK_COMMENT_START;
        if (lexeme == "*/")
            return token_type::BLOCK_COMMENT_END;


        if (lexeme[0] == '#')
            return token_type::PREPROCESSOR_DIRECTIVE;


        if (is_integer(lexeme))
            return token_type::INTEGER_CONST;

        if (is_double(lexeme))
            return token_type::DOUBLE_CONST;

        if (is_string(lexeme))
            return token_type::STRING_CONST;

        if (is_char(lexeme))
            return token_type::CHAR_CONST;


        return token_type::IDENTIFIER;
    }

    static bool is_string(const string& current_lexeme)
    {
        return current_lexeme.front() == '"' && current_lexeme.back() == '"';
    }

    static bool is_char(const string& current_lexeme)
    {
        return current_lexeme.front() == '\'' && current_lexeme.back() == '\'';
    }

    static bool is_integer(const string& current_lexeme)
    {
        for (const auto& symbol : current_lexeme)
            if (symbol < '0' || symbol > '9')
                return false;
        return true;
    }

    static bool is_double(const string& current_lexeme)
    {
        bool point = false;

        for (const char& s : current_lexeme)
        {
            if (s == '.' && !point)
            {
                point = true;
                continue;
            }

            if (s == '.')
                return false;

            if (s < '0' || s > '9')
                return false;

        }

        return true;
    }

    void print()
    {
        cout << lexeme << " with type: \"" << token_type_to_string(type) << "\" " <<
        "\tline:" << line <<
        "\tposition:" << pos <<
        "\n";
    }


    static string token_type_to_string(token_type input_type)
    {
        switch (input_type)
        {
            case token_type::IDENTIFIER:
            {
                return "identifier";
            }
            case token_type::INTEGER_CONST:
            {
                return "integer constant";
            }
            case token_type::DOUBLE_CONST:
            {
                return "double constant";
            }
            case token_type::STRING_CONST:
            {
                return "string constant";
            }
            case token_type::CHAR_CONST:
            {
                return "char constant";
            }
            case token_type::TRUE:
            {
                return "true";
            }
            case token_type::FALSE:
            {
                return "false";
            }
            case token_type::CONST:
            {
                return "const";
            }
            case token_type::UNDEFINED:
            {
                return "undefined";
            }
            case token_type::INT:
            {
                return "int";
            }
            case token_type::DOUBLE:
            {
                return "double";
            }
            case token_type::BOOL:
            {
                return "bool";
            }
            case token_type::CHAR:
            {
                return "char";
            }
            case token_type::VOID:
            {
                return "void";
            }
            case token_type::AUTO:
            {
                return "auto";
            }
            case token_type::DO_WHILE:
            {
                return "do while";
            }
            case token_type::WHILE:
            {
                return "while";
            }
            case token_type::FOR:
            {
                return "for";
            }
            case token_type::BREAK:
            {
                return "break";
            }
            case token_type::CONTINUE:
            {
                return "continue";
            }
            case token_type::SWITCH:
            {
                return "switch";
            }
            case token_type::CASE:
            {
                return "case";
            }
            case token_type::DEFAULT:
            {
                return "default";
            }
            case token_type::IF:
            {
                return "if";
            }
            case token_type::ELSE:
            {
                return "else";
            }
            case token_type::LESS:
            {
                return "less";
            }
            case token_type::GREATER:
            {
                return "greater";
            }
            case token_type::LESS_EQUAL:
            {
                return "less and equal";
            }
            case token_type::GREATER_EQUAL:
            {
                return "greater and equal";
            }
            case token_type::EQUAL:
            {
                return "equal";
            }
            case token_type::NOT_EQUAL:
            {
                return "not equal";
            }
            case token_type::AND:
            {
                return "logic and";
            }
            case token_type::OR:
            {
                return "logic or";
            }
            case token_type::EXCLAMATION:
            {
                return "exclamation";
            }
            case token_type::PLUS:
            {
                return "plus";
            }
            case token_type::MINUS:
            {
                return "minus";
            }
            case token_type::STAR:
            {
                return "star";
            }
            case token_type::SLASH:
            {
                return "true";
            }
            case token_type::INC:
            {
                return "inc";
            }
            case token_type::DEC:
            {
                return "dec";
            }
            case token_type::LBRA:
            {
                return "lbra";
            }
            case token_type::RBRA:
            {
                return "rbra";
            }
            case token_type::LPAR:
            {
                return "lpar";
            }
            case token_type::RPAR:
            {
                return "rpar";
            }
            case token_type::LSQR:
            {
                return "lsqr";
            }
            case token_type::RSQR:
            {
                return "rsqr";
            }
            case token_type::ASSIGN:
            {
                return "assign";
            }
            case token_type::ADD_ASSIGN:
            {
                return "add assign";
            }
            case token_type::SUB_ASSIGN:
            {
                return "sun assign";
            }
            case token_type::MUL_ASSIGN:
            {
                return "mul assign";
            }
            case token_type::DIV_ASSIGN:
            {
                return "true";
            }
            case token_type::FUNCTION:
            {
                return "function";
            }
            case token_type::RETURN:
            {
                return "return";
            }
            case token_type::SEMICOLON:
            {
                return "semicolon";
            }
            case token_type::COLON:
            {
                return "colon";
            }
            case token_type::COMMA:
            {
                return "comma";
            }
            case token_type::POINT:
            {
                return "point";
            }
            case token_type::QUESTION:
            {
                return "question";
            }
            case token_type::AMPERSAND:
            {
                return "ampersand";
            }
            case token_type::LINE_COMMENT:
            {
                return "line comment";
            }
            case token_type::BLOCK_COMMENT_START:
            {
                return "block comment start";
            }
            case token_type::BLOCK_COMMENT_END:
            {
                return "block comment end";
            }
            case token_type::NEW:
            {
                return "new";
            }
            case token_type::DELETE:
            {
                return "delete";
            }
            case token_type::PREPROCESSOR_DIRECTIVE:
            {
                return "preprocessor directive";
            }
            case token_type::ACCESS_OPERATOR:
            {
                return "access operator";
            }

            default:
                return "";
        }
    }



};