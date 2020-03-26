#pragma once
#include <fstream>
#include <vector>
#include "../error-handle/error-handle.h"
#include "token/token.h"

enum class lexer_state
{
    DEFAULT,
    IN_STRING,
    IN_SYMBOL,
    POSSIBLE_LINE_COMMENT,
    IN_LINE_COMMENT
};


class lexer
{
public:
    string file_path;
    string code;
    vector<token> tokens;

    lexer_state state;

    size_t current_token_index;

public:
    explicit lexer(const string& file_path)
    {
        this->file_path = file_path;
        this->current_token_index = 0;
        this->state = lexer_state::DEFAULT;

        open();
    }

    void open()
    {
        ifstream in(file_path, ios::binary);

        if (!in.is_open())
            error_handle::raise(error_handle_type::LEXER, "File not found!");


        int size = in.seekg(0, ios::end).tellg();
        in.seekg(0);

        code.resize(size);
        in.read(&code[0], size);
    }

    void next_token()
    {
        if (current_token_index < tokens.size() - 2)
        {
            ++current_token_index;
        }
    }

    token current_token()
    {
        return tokens[current_token_index];
    }

    token_type current_token_type()
    {
        return current_token().type;
    }

    void split()
    {
        current_token_index = -1;

        size_t current_line = 1;
        size_t current_pos = 0;

        string current_lexeme;

        for (auto& symbol : code)
        {
            if (symbol == '\n')
            {
                ++current_line;
                current_pos = 0;
            }

            ++current_pos;

            if (state == lexer_state::DEFAULT || state == lexer_state::POSSIBLE_LINE_COMMENT)
            {
                if (symbol == '/')
                {
                    if (state == lexer_state::POSSIBLE_LINE_COMMENT)
                    {
                        state = lexer_state::IN_LINE_COMMENT;
                    }
                    else
                    {
                        state = lexer_state::POSSIBLE_LINE_COMMENT;
                        current_lexeme += symbol;
                        continue;
                    }
                }
                else
                {
                    state = lexer_state::DEFAULT;
                }
            }

            if (state == lexer_state::IN_LINE_COMMENT)
            {
                if (symbol == '\n')
                {
                    current_lexeme.clear();
                    state = lexer_state::DEFAULT;
                }
                continue;
            }


            if (symbol == '"')
            {
                if (state == lexer_state::DEFAULT)
                {
                    state = lexer_state::IN_STRING;
                    current_lexeme += symbol;

                    continue;
                }
                else if (state == lexer_state::IN_STRING)
                {
                    state = lexer_state::DEFAULT;
                    current_lexeme += symbol;

                    token new_token(current_lexeme, current_line, current_pos);
                    tokens.push_back(new_token);
                    current_lexeme.clear();

                    ++current_token_index;

                    continue;
                }
            }

            if (state == lexer_state::IN_STRING)
            {
                current_lexeme += symbol;
                continue;
            }



            if (symbol == '\'')
            {
                if (state == lexer_state::DEFAULT)
                {
                    state = lexer_state::IN_SYMBOL;
                    current_lexeme += symbol;

                    continue;
                }
                else if (state == lexer_state::IN_SYMBOL)
                {
                    state = lexer_state::DEFAULT;
                    current_lexeme += symbol;

                    token new_token(current_lexeme, current_line, current_pos);
                    tokens.push_back(new_token);
                    current_lexeme.clear();

                    ++current_token_index;

                    continue;
                }
            }

            if (state == lexer_state::IN_SYMBOL)
            {
                current_lexeme += symbol;
                continue;
            }





            if (is_separate_symbol(symbol))
            {
                if (!current_lexeme.empty())
                {
                    token new_token(current_lexeme, current_line, current_pos);
                    tokens.push_back(new_token);
                    current_lexeme.clear();

                    ++current_token_index;
                }

                if (symbol != ' ' && symbol != '\r' && symbol != '\n')
                {
                    string current_symbol(1, symbol);
                    token new_token(current_symbol, current_line, current_pos);
                    tokens.push_back(new_token);

                    ++current_token_index;
                }
            }
            else
            {
                current_lexeme += symbol;
            }
        }

        if (!current_lexeme.empty())
        {
            token new_token(current_lexeme, current_line, current_pos);
            tokens.push_back(new_token);
            current_lexeme.clear();

            ++current_token_index;
        }
    }

    void merge()
    {
        vector<token> temp_tokens;


        size_t current_line = 1;
        size_t current_pos = 0;

        string temp_lexeme;
        bool in_string = false;




        for (int i = 0; i < tokens.size() - 2; ++i)
        {
            if (tokens[i].type == token_type::INTEGER_CONST)
            {
                if (tokens[i + 1].type == token_type::POINT && tokens[i + 2].type == token_type::INTEGER_CONST)
                {
                    string new_lexeme = tokens[i].lexeme + tokens[i + 1].lexeme + tokens[i + 2].lexeme;

                    current_line = tokens[i].line;
                    current_pos = tokens[i].pos;

                    token new_token(new_lexeme, current_line, current_pos);

                    temp_tokens.push_back(new_token);

                    i += 3;
                }
            }

            temp_tokens.push_back(tokens[i]);
        }
        tokens = temp_tokens;
        temp_tokens.clear();




        for (int i = 0; i < tokens.size() - 1; ++i)
        {
            if (is_complex_token(tokens[i].lexeme[0], tokens[i + 1].lexeme[0]))
            {
                string new_lexeme = tokens[i].lexeme + tokens[i + 1].lexeme;

                current_line = tokens[i].line;
                current_pos = tokens[i].pos;

                token new_token(new_lexeme, current_line, current_pos);


                temp_tokens.push_back(new_token);

                i += 2;
            }

            temp_tokens.push_back(tokens[i]);
        }
        tokens = temp_tokens;
        temp_tokens.clear();



        for (int i = 0; i < tokens.size(); ++i)
        {
            if (tokens[i].type == token_type::BLOCK_COMMENT_START)
            {
                while (tokens[i].type != token_type::BLOCK_COMMENT_END)
                {
                    ++i;
                }
                ++i;
            }

            temp_tokens.push_back(tokens[i]);
        }
        tokens = temp_tokens;
        temp_tokens.clear();



        for (int i = 0; i < tokens.size() - 1; ++i)
        {
            if (tokens[i].lexeme == "#")
            {
                string new_lexeme = tokens[i].lexeme + tokens[i + 1].lexeme;
                current_line = tokens[i].line;
                current_pos = tokens[i].pos;

                token new_token(new_lexeme, current_line, current_pos);

                temp_tokens.push_back(new_token);

                i += 2;
            }

            temp_tokens.push_back(tokens[i]);
        }
        tokens = temp_tokens;
        temp_tokens.clear();


        current_token_index = -1;
        for (auto& token : tokens)
        {
            ++current_token_index;

            if (token.type == token_type::IDENTIFIER)
            {
                if (!is_correct_identifier(token.lexeme))
                {
                    error_handle::raise(error_handle_type::LEXER, "Incorrect identifier \"" + token.lexeme + "\"!", this);
                }
            }
            else if (token.type == token_type::CHAR_CONST)
            {
                if (token.lexeme.size() != 3)
                {
                    error_handle::raise(error_handle_type::LEXER, "Multi-character character constant " + token.lexeme + "!", this);
                }
            }

        }
        current_token_index = 0;
    }

    static bool is_complex_token(char symbol1, char symbol2)
    {
        switch (symbol1)
        {
            case '<':
            {
                return symbol2 == '=';
            }
            case '>':
            {
                return symbol2 == '=';
            }
            case '+':
            {
                return symbol2 == '=' || symbol2 == '+';
            }
            case '-':
            {
                return symbol2 == '=' || symbol2 == '-';
            }
            case '=':
            {
                return symbol2 == '=';
            }
            case '!':
            {
                return symbol2 == '=';
            }
            case '&':
            {
                return symbol2 == '&';
            }
            case '|':
            {
                return symbol2 == '|';
            }
            case ':':
            {
                return symbol2 == ':';
            }
            case '/':
            {
                return symbol2 == '*';
            }
            case '*':
            {
                return symbol2 == '/';
            }

            default:
                return false;
        }

    }

    void print()
    {
        for (auto& token : tokens)
        {
            token.print();
        }
    }

    static bool is_separate_symbol(char symbol)
    {
        return  symbol == ':' || symbol == ';' ||
                symbol == ',' || symbol == '.' ||
                symbol == '{' || symbol == '}' ||
                symbol == '(' || symbol == ')' ||
                symbol == '[' || symbol == ']' ||
                symbol == '*' || symbol == '/' ||
                symbol == '+' || symbol == '-' ||
                symbol == '&' || symbol == '|' ||
                symbol == '=' || symbol == '!' ||
                symbol == '<' || symbol == '>' ||
                symbol == '\''|| symbol == '"' ||
                symbol == '^' || symbol == '?' ||
                symbol == '%' || symbol == '\\'||
                symbol == '~' || symbol == ' ' ||
                symbol == '\r' ||symbol == '\n'||
                symbol == '#';
    }

    static bool is_correct_identifier(const string& token)
    {
        if (!isalpha(token[0]) && token[0] != '_')
            return false;

        for (const auto& symbol : token)
            if (!isalpha(symbol) && !isdigit(symbol) && symbol != '_')
                return false;

        return true;
    }


    void print_current_token_line()
    {
        int i = current_token_index;
        int j = current_token_index;
        auto current_line = current_token().line;

        while (i > 0 && tokens[i].line == current_line)
        {
            --i;
        }
        ++i;

        int count_symbol_before = current_token_index - i;

        while (j < tokens.size() && tokens[j].line == current_line)
        {
            ++j;
        }


        cout << "line " << current_line << ": ";


        size_t count_symbol_before_current = 0;
        string underline;
        size_t size_current_token = 0;
        for (int k = i; k < j; ++k)
        {
            if (k - i == count_symbol_before)
            {
                cout << "" << tokens[k].lexeme << "" << " ";
                size_current_token = tokens[k].lexeme.size();
                continue;
            }

            cout << tokens[k].lexeme << " ";

            if (k - i <= count_symbol_before)
            {
                count_symbol_before_current += tokens[k].lexeme.size() + 1;
            }
        }

        count_symbol_before_current += 7 + to_string(current_line).size();

        for (int l = 0; l < count_symbol_before_current; ++l)
        {
            underline += ' ';
        }

        for (int l = 0; l < size_current_token; ++l)
        {
            underline += '~';
        }

        cout << endl;
        cout << underline;
        cout << endl;
    }

};