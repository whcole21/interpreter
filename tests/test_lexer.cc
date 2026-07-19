#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "token.h"
#include "lexer.h"
#include <sstream>


// ============================================================
// Single token types
// ============================================================

TEST_CASE("lexer tokenizes integer literals", "[lexer]")
{
    SECTION("multi-digit number") {
        std::istringstream input("42");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::NUM);
        REQUIRE(t.lexeme == "42");
        REQUIRE(lexer.getToken().type == TokenType::END_OF_FILE);
    }

    SECTION("large number") {
        std::istringstream input("12345");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::NUM);
        REQUIRE(t.lexeme == "12345");
        REQUIRE(lexer.getToken().type == TokenType::END_OF_FILE);
    }

    SECTION("single digit") {
        std::istringstream input("7");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::NUM);
        REQUIRE(t.lexeme == "7");
        REQUIRE(lexer.getToken().type == TokenType::END_OF_FILE);
    }

    SECTION("zero") {
        std::istringstream input("0");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::NUM);
        REQUIRE(t.lexeme == "0");
        REQUIRE(lexer.getToken().type == TokenType::END_OF_FILE);
    }

    SECTION("leading zeros are not collapsed") {
        // Expected to FAIL — the lexer currently scans all digits without
        // guarding against leading zeros. This documents the known behavior;
        // fix will be added later.
        std::istringstream input("000");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::NUM);
        REQUIRE(t.lexeme == "0");
    }

    SECTION("number followed by non-digit is split correctly") {
        std::istringstream input("123abc");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::NUM);
        REQUIRE(t.lexeme == "123");

        t = lexer.getToken();
        REQUIRE(t.type == TokenType::ID);
        REQUIRE(t.lexeme == "abc");
    }
}


TEST_CASE("lexer tokenizes identifiers", "[lexer]")
{
    SECTION("all letters") {
        std::istringstream input("myVar");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::ID);
        REQUIRE(t.lexeme == "myVar");
    }

    SECTION("letter followed by number") {
        std::istringstream input("x1");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::ID);
        REQUIRE(t.lexeme == "x1");
    }

    SECTION("letters followed by non-alphanumeric character") {
        std::istringstream input("abc_def");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::ID);
        REQUIRE(t.lexeme == "abc");

        REQUIRE_THROWS_AS(lexer.getToken(), std::runtime_error);
    }
}


TEST_CASE("lexer tokenizes single-character operators", "[lexer]")
{
    std::istringstream input("+ - * /");
    Lexer lexer(input);

    Token t = lexer.getToken();
    REQUIRE(t.type == TokenType::PLUS);
    REQUIRE(t.lexeme == "+");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::MINUS);
    REQUIRE(t.lexeme == "-");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::MULT);
    REQUIRE(t.lexeme == "*");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::DIV);
    REQUIRE(t.lexeme == "/");
}


TEST_CASE("lexer tokenizes delimiters", "[lexer]")
{
    std::istringstream input("{ } ( ) ; , :");
    Lexer lexer(input);

    Token t = lexer.getToken();
    REQUIRE(t.type == TokenType::LBRACE);
    REQUIRE(t.lexeme == "{");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::RBRACE);
    REQUIRE(t.lexeme == "}");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::LPAREN);
    REQUIRE(t.lexeme == "(");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::RPAREN);
    REQUIRE(t.lexeme == ")");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::SEMICOLON);
    REQUIRE(t.lexeme == ";");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::COMMA);
    REQUIRE(t.lexeme == ",");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::COLON);
    REQUIRE(t.lexeme == ":");
}


// ============================================================
// Multi-character operators
// ============================================================

TEST_CASE("lexer tokenizes multi-character operators", "[lexer]")
{
    std::istringstream input("<= >= == <>");
    Lexer lexer(input);

    Token t = lexer.getToken();
    REQUIRE(t.type == TokenType::LEQUAL);
    REQUIRE(t.lexeme == "<=");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::GEQUAL);
    REQUIRE(t.lexeme == ">=");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::ISEQUAL);
    REQUIRE(t.lexeme == "==");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::NOTEQUAL);
    REQUIRE(t.lexeme == "<>");
}


TEST_CASE("lexer distinguishes between relational operators", "[lexer]")
{
    SECTION("distinguishes < from <= and <>") {
        std::istringstream input("< <= <>");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::LESS);
        REQUIRE(t.lexeme == "<");

        t = lexer.getToken();
        REQUIRE(t.type == TokenType::LEQUAL);
        REQUIRE(t.lexeme == "<=");

        t = lexer.getToken();
        REQUIRE(t.type == TokenType::NOTEQUAL);
        REQUIRE(t.lexeme == "<>");
    }

    SECTION("distinguishes > from >=") {
        std::istringstream input("> >=");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::GREATER);
        REQUIRE(t.lexeme == ">");

        t = lexer.getToken();
        REQUIRE(t.type == TokenType::GEQUAL);
        REQUIRE(t.lexeme == ">=");
    }

    SECTION("distinguishes = from ==") {
        std::istringstream input("= ==");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::EQUAL);
        REQUIRE(t.lexeme == "=");

        t = lexer.getToken();
        REQUIRE(t.type == TokenType::ISEQUAL);
        REQUIRE(t.lexeme == "==");
    }
}


// ============================================================
// Keywords vs identifiers
// ============================================================

TEST_CASE("lexer recognizes keywords", "[lexer]")
{
    std::istringstream input("if else while for switch case default var input output");
    Lexer lexer(input);

    Token t = lexer.getToken();
    REQUIRE(t.type == TokenType::IF);
    REQUIRE(t.lexeme == "if");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::ELSE);
    REQUIRE(t.lexeme == "else");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::WHILE);
    REQUIRE(t.lexeme == "while");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::FOR);
    REQUIRE(t.lexeme == "for");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::SWITCH);
    REQUIRE(t.lexeme == "switch");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::CASE);
    REQUIRE(t.lexeme == "case");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::DEFAULT);
    REQUIRE(t.lexeme == "default");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::VAR);
    REQUIRE(t.lexeme == "var");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::INPUT);
    REQUIRE(t.lexeme == "input");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::OUTPUT);
    REQUIRE(t.lexeme == "output");
}


TEST_CASE("lexer does not split identifier that starts with keyword", "[lexer]")
{
    std::istringstream input("ifvar whileloop case1");
    Lexer lexer(input);

    Token t = lexer.getToken();
    REQUIRE(t.type == TokenType::ID);
    REQUIRE(t.lexeme == "ifvar");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::ID);
    REQUIRE(t.lexeme == "whileloop");

    t = lexer.getToken();
    REQUIRE(t.type == TokenType::ID);
    REQUIRE(t.lexeme == "case1");
}


// ============================================================
// Whitespace and line tracking
// ============================================================

TEST_CASE("lexer skips whitespace between tokens", "[lexer]")
{
    SECTION("whitespace followed by token") {
        std::istringstream input(" \t abc");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::ID);
        REQUIRE(t.lexeme == "abc");
    }

    SECTION("token followed by whitespace") {
        std::istringstream input("ab \t");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::ID);
        REQUIRE(t.lexeme == "ab");

        REQUIRE(lexer.getToken().type == TokenType::END_OF_FILE);
    }

    SECTION("token followed by whitespace followed by token") {
        std::istringstream input("ab \t\t cd");
        Lexer lexer(input);

        Token t = lexer.getToken();
        REQUIRE(t.type == TokenType::ID);
        REQUIRE(t.lexeme == "ab");

        t = lexer.getToken();
        REQUIRE(t.type == TokenType::ID);
        REQUIRE(t.lexeme == "cd");
    }

    SECTION("all whitespace returns END_OF_FILE token") {
        std::istringstream input("  \t \t ");
        Lexer lexer(input);

        REQUIRE(lexer.getToken().type == TokenType::END_OF_FILE);
    }
}


TEST_CASE("lexer tracks line numbers across newlines", "[lexer]")
{
    SECTION("tracks identifiers across newlines") {
        std::istringstream input("x\ny\nz");
        Lexer lexer(input);

        REQUIRE(lexer.getToken().line_no == 1);
        REQUIRE(lexer.getToken().line_no == 2);
        REQUIRE(lexer.getToken().line_no == 3);
    }

    SECTION("tracks multi-character operators and numbers across newlines") {
        std::istringstream input("x = 42\ny = 0\nz = 10");
        Lexer lexer(input);

        REQUIRE(lexer.getToken().line_no == 1);
        REQUIRE(lexer.getToken().line_no == 1);
        REQUIRE(lexer.getToken().line_no == 1);
        REQUIRE(lexer.getToken().line_no == 2);
        REQUIRE(lexer.getToken().line_no == 2);
        REQUIRE(lexer.getToken().line_no == 2);
        REQUIRE(lexer.getToken().line_no == 3);
        REQUIRE(lexer.getToken().line_no == 3);
        REQUIRE(lexer.getToken().line_no == 3);
    }

    SECTION("correctly tracks blank lines") {
        std::istringstream input("\n\n\nx");
        Lexer lexer(input);

        REQUIRE(lexer.getToken().line_no == 4);
    }
}


// ============================================================
// EOF handling
// ============================================================

TEST_CASE("lexer returns END_OF_FILE on empty input", "[lexer]")
{
    std::istringstream input("");
    Lexer lexer(input);

    REQUIRE(lexer.getToken().type == TokenType::END_OF_FILE);
}


TEST_CASE("lexer returns END_OF_FILE after all tokens consumed", "[lexer]")
{
    std::istringstream input("a = b + c;");
    Lexer lexer(input);

    for (int i = 0; i < 6; i++)
        lexer.getToken();
    
    REQUIRE(lexer.getToken().type == TokenType::END_OF_FILE);
    REQUIRE(lexer.getToken().type == TokenType::END_OF_FILE);
}


// ============================================================
// Peek behavior
// ============================================================

TEST_CASE("peek returns next token without consuming it", "[lexer]")
{
    std::istringstream input("x = 10;");
    Lexer lexer(input);

    lexer.getToken();

    Token t1, t2;
    t1 = lexer.peek();
    t2 = lexer.peek();
    REQUIRE(t1.type == t2.type);
    REQUIRE(t1.lexeme == t2.lexeme);
    REQUIRE(t1.line_no == t2.line_no);

    t2 = lexer.getToken();
    REQUIRE(t1.type == t2.type);
    REQUIRE(t1.lexeme == t2.lexeme);
    REQUIRE(t1.line_no == t2.line_no);

    t1 = lexer.getToken();
    REQUIRE(t1.type == TokenType::NUM);
    REQUIRE(t1.lexeme == "10");

    REQUIRE(lexer.peek().type == TokenType::SEMICOLON);
}


// ============================================================
// Error handling
// ============================================================

TEST_CASE("lexer throws on unrecognized character", "[lexer]")
{
    std::istringstream input("x = @;");
    Lexer lexer(input);

    lexer.getToken();  // ID("x")
    lexer.getToken();  // EQUAL

    REQUIRE_THROWS_AS(lexer.getToken(), std::runtime_error);
}


// ============================================================
// Integration: token sequences
// ============================================================

TEST_CASE("lexer tokenizes a complete statement", "[lexer]")
{
    std::istringstream input("y = 42 + x;");
    Lexer lexer(input);
    
    REQUIRE(lexer.getToken().type == TokenType::ID);
    REQUIRE(lexer.getToken().type == TokenType::EQUAL);
    REQUIRE(lexer.getToken().type == TokenType::NUM);
    REQUIRE(lexer.getToken().type == TokenType::PLUS);
    REQUIRE(lexer.getToken().type == TokenType::ID);
    REQUIRE(lexer.getToken().type == TokenType::SEMICOLON);
    REQUIRE(lexer.getToken().type == TokenType::END_OF_FILE);
}


TEST_CASE("lexer tokenizes adjacent operators without spaces", "[lexer]")
{
    std::istringstream input("x<=3");
    Lexer lexer(input);

    REQUIRE(lexer.getToken().type == TokenType::ID);
    REQUIRE(lexer.getToken().type == TokenType::LEQUAL);
    REQUIRE(lexer.getToken().type == TokenType::NUM);
}
