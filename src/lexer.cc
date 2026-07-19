#include "lexer.h"
#include <unordered_map>
#include <stdexcept>


Token Lexer::getToken()
{
    if (!nextToken)
        return scanToken();

    Token t = *nextToken;
    nextToken = std::nullopt;
    return t;
}


Token Lexer::peek()
{
    if (!nextToken)
        nextToken = scanToken();
    return *nextToken;
}


Token Lexer::scanToken()
{
    skipWhitespace();

    int ch = input.peek();

    if (ch == EOF)
        return Token{TokenType::END_OF_FILE, "", line_no};

    if (std::isdigit(ch))
        return scanNumber();

    if (std::isalpha(ch))
        return scanIdentifier();

    ch = input.get();

    Token t;
    t.line_no = line_no;

    switch (ch) {
        case '+':
            t.type = TokenType::PLUS;
            t.lexeme = "+";
            break;
        case '-':
            t.type = TokenType::MINUS;
            t.lexeme = "-";
            break;
        case '*':
            t.type = TokenType::MULT;
            t.lexeme = "*";
            break;
        case '/':
            t.type = TokenType::DIV;
            t.lexeme = "/";
            break;
        case ';':
            t.type = TokenType::SEMICOLON;
            t.lexeme = ";";
            break;
        case ',':
            t.type = TokenType::COMMA;
            t.lexeme = ",";
            break;
        case ':':
            t.type = TokenType::COLON;
            t.lexeme = ":";
            break;
        case '{':
            t.type = TokenType::LBRACE;
            t.lexeme = "{";
            break;
        case '}':
            t.type = TokenType::RBRACE;
            t.lexeme = "}";
            break;
        case '(':
            t.type = TokenType::LPAREN;
            t.lexeme = "(";
            break;
        case ')':
            t.type = TokenType::RPAREN;
            t.lexeme = ")";
            break;
        // Multi-character operators: peek at the next character to
        // disambiguate between e.g. '<' vs '<=' vs '<>'
        case '<':
            ch = input.peek();
            if (ch == '=') {
                input.get();
                t.type = TokenType::LEQUAL;
                t.lexeme = "<=";
            }
            else if (ch == '>') {
                input.get();
                t.type = TokenType::NOTEQUAL;
                t.lexeme = "<>";
            }
            else {
                t.type = TokenType::LESS;
                t.lexeme = "<";
            }
            break;
        case '>':
            ch = input.peek();
            if (ch == '=') {
                input.get();
                t.type = TokenType::GEQUAL;
                t.lexeme = ">=";
            }
            else {
                t.type = TokenType::GREATER;
                t.lexeme = ">";
            }
            break;
        case '=':
            ch = input.peek();
            if (ch == '=') {
                input.get();
                t.type = TokenType::ISEQUAL;
                t.lexeme = "==";
            }
            else {
                t.type = TokenType::EQUAL;
                t.lexeme = "=";
            }
            break;
        default:
            throw std::runtime_error(
                "Unexpected character '" + std::string(1, static_cast<char>(ch)) +
                "' at line " + std::to_string(line_no)
            );
    }

    return t;
}


Token Lexer::scanNumber()
{
    Token t;
    t.type = TokenType::NUM;
    t.line_no = line_no;

    int ch = input.get();
    if (ch == '0') {
        t.lexeme = "0";
        return t;
    }

    while (std::isdigit(ch)) {
        t.lexeme += static_cast<char>(ch);
        ch = input.get();
    }

    if (ch != EOF)
        input.unget();

    return t;
}


Token Lexer::scanIdentifier()
{
    static const std::unordered_map<std::string, TokenType> keywords = {
        {"if",      TokenType::IF},
        {"while",   TokenType::WHILE},
        {"switch",  TokenType::SWITCH},
        {"case",    TokenType::CASE},
        {"default", TokenType::DEFAULT},
        {"for",     TokenType::FOR},
        {"var",     TokenType::VAR},
        {"else",    TokenType::ELSE},
        {"input",   TokenType::INPUT},
        {"output",  TokenType::OUTPUT},
    };

    Token t;
    t.line_no = line_no;

    int ch = input.get();
    while (std::isalnum(ch)) {
        t.lexeme += static_cast<char>(ch);
        ch = input.get();
    }

    if (ch != EOF)
        input.unget();

    auto it = keywords.find(t.lexeme);
    if (it != keywords.end())
        t.type = it->second;
    else
        t.type = TokenType::ID;

    return t;
}


void Lexer::skipWhitespace()
{
    int ch = input.get();
    while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
        if (ch == '\n')
            line_no++;
        ch = input.get();
    }

    if (ch != EOF)
        input.unget();
}

