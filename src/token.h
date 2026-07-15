/**
 * @file token.h
 * @brief Token type definitions for the interpreter's lexical analysis stage.
 *
 * Defines the vocabulary shared between the Lexer and Parser: the TokenType
 * enum enumerates every terminal symbol in the grammar, and the Token struct
 * bundles a token's type, source text, and location. This file is intentionally
 * kept free of implementation logic so that downstream modules (parser,
 * interpreter) can include it without pulling in lexer internals.
 */

#pragma once

#include <iostream>
#include <string>

/**
 * @enum TokenType
 * @brief Enumerates every distinct token the lexer can produce.
 */
enum class TokenType {
    // Literals
    NUM,
    ID,

    // Keywords
    IF, WHILE, SWITCH, CASE, DEFAULT,
    FOR, VAR, ELSE, INPUT, OUTPUT,

    // Operators
    PLUS,      // +
    MINUS,     // -
    MULT,      // *
    DIV,       // /
    EQUAL,     // =
    GREATER,   // >
    LESS,      // <
    NOTEQUAL,  // <>
    GEQUAL,    // >=
    LEQUAL,    // <=
    ISEQUAL,   // ==

    // Delimiters
    LBRACE,    // {
    RBRACE,    // }
    LPAREN,    // (
    RPAREN,    // )
    SEMICOLON, // ;
    COMMA,     // ,
    COLON,     // :

    // Special
    END_OF_FILE
};

/**
 * @struct Token
 * @brief A single lexical token produced by the Lexer.
 *
 * Carries the token's classification, the original source text that produced
 * it, and the line number where it appeared (for error reporting).
 */
struct Token {
    TokenType type;
    std::string lexeme;
    int line_no;
};

/**
 * @brief Writes a human-readable TokenType name to an output stream.
 * @param os    The output stream.
 * @param type  The token type to print.
 * @return      Reference to the output stream (for chaining).
 */
std::ostream& operator<<(std::ostream& os, const TokenType& type);

/**
 * @brief Writes a formatted Token representation to an output stream.
 *
 * Output format: `TYPE("lexeme") at line N`
 *
 * @param os  The output stream.
 * @param t   The token to print.
 * @return    Reference to the output stream (for chaining).
 */
std::ostream& operator<<(std::ostream& os, const Token& t);
