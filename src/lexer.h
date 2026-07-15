/**
 * @file lexer.h
 * @brief Lexer (tokenizer) for the interpreter.
 *
 * The Lexer reads a character stream from any std::istream source and produces
 * a sequence of Tokens for the Parser. It supports single-token lookahead via
 * peek(), allowing the parser to inspect the next token without consuming it.
 *
 * Design notes:
 *  - Input is injected via std::istream& so the lexer is decoupled from any
 *    specific input source (stdin, file, string buffer).
 *  - All mutable state is encapsulated as private data members; there is no
 *    global or static mutable state.
 *  - Line numbers are tracked for error reporting.
 */

#pragma once

#include <optional>
#include "token.h"


class Lexer {
    public:

        /**
         * @brief Constructs a Lexer that reads from the given input stream.
         * @param in  The character stream to tokenize.
         */
        Lexer(std::istream& in) : input(in), line_no(1) {}

        /**
         * @brief Consumes and returns the next token from the input.
         *
         * If a token has been buffered by a prior peek(), that token is
         * returned and the buffer is cleared. Otherwise a new token is
         * scanned from the input stream.
         *
         * @return The next Token.
         */
        Token getToken();

        /**
         * @brief Returns the next token without consuming it.
         *
         * The token is buffered internally so that a subsequent call to
         * getToken() returns the same token. Multiple consecutive calls
         * to peek() without an intervening getToken() return the same token.
         *
         * @return The next Token (non-consuming).
         */
        Token peek();

    private:

        std::istream& input;
        int line_no;
        std::optional<Token> nextToken;

        /**
         * @brief Scans the input stream and returns the next token.
         *
         * Skips leading whitespace, then dispatches to the appropriate
         * scanning routine based on the first character: digits to
         * scanNumber(), letters to scanIdentifier(), and punctuation
         * to inline switch-case logic for operators and delimiters.
         *
         * @return The scanned Token.
         * @throws std::runtime_error on unrecognized characters.
         */
        Token scanToken();

        /**
         * @brief Scans a contiguous sequence of digits into a NUM token.
         * @return A Token with type NUM and the digit sequence as its lexeme.
         */
        Token scanNumber();

        /**
         * @brief Scans a contiguous sequence of alphanumeric characters and
         *        classifies the result as a keyword or identifier.
         * @return A Token with the appropriate keyword type, or ID.
         */
        Token scanIdentifier();

        /**
         * @brief Advances past whitespace characters in the input stream.
         *
         * Increments line_no for each newline encountered.
         */
        void skipWhitespace();

        /**
         * @brief Constructs an END_OF_FILE token at the current line.
         * @return A Token with type END_OF_FILE.
         */
        Token eofToken() const;

};
