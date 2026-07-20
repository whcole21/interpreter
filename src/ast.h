#pragma once

#include <memory>
#include <vector>
#include "token.h"


struct Stmt;
struct Block;
struct Expr;
struct Condition;
struct Case;


/**
 * @brief Root node of the AST. Represents an entire program.
 *
 * A program consists of a variable declaration section, a body of
 * statements, and a list of input values consumed by INPUT statements
 * at runtime.
 */
struct Program
{
    std::vector<std::string> vars;
    std::unique_ptr<Block> body;
    std::vector<int> inputs;
};


/**
 * @brief Abstract base class for all statement nodes.
 *
 * Virtual destructor ensures proper cleanup when deleting through
 * a Stmt pointer (e.g., std::unique_ptr<Stmt> holding an IfStmt).
 */
struct Stmt
{
    virtual ~Stmt() = default;
};

struct Block : Stmt
{
    std::vector<std::unique_ptr<Stmt>> stmts;
};

struct AssignStmt : Stmt
{
    std::string target;         // the variable name
    std::unique_ptr<Expr> expr; // the right-hand side expression
};

/**
 * @brief An if statement with optional else/else-if.
 *
 * elseBody is nullptr when there is no else clause. For a plain "else",
 * it holds a Block. For "else if", it holds a nested IfStmt. Using
 * Stmt* as the type allows both cases without a separate union or variant.
 */
struct IfStmt : Stmt
{
    std::unique_ptr<Condition> condition;
    std::unique_ptr<Block> thenBody;
    std::unique_ptr<Stmt> elseBody;
};

struct WhileStmt : Stmt
{
    std::unique_ptr<Condition> condition;
    std::unique_ptr<Block> body;
};

struct ForStmt : Stmt
{
    std::unique_ptr<AssignStmt> init;
    std::unique_ptr<Condition> condition;
    std::unique_ptr<AssignStmt> update;
    std::unique_ptr<Block> body;
};

struct SwitchStmt : Stmt
{
    std::string target; // the variable name being switched on
    std::vector<std::unique_ptr<Case>> cases;
    std::unique_ptr<Block> defaultBody; // optional default block
};

struct InputStmt : Stmt
{
    std::string varName;
};

struct OutputStmt : Stmt
{
    std::unique_ptr<Expr> expr;
};


/** @brief Abstract base class for all expression nodes. */
struct Expr
{
    virtual ~Expr() = default;
};

struct NumLiteral : Expr
{
    int value;
};

struct VarRef : Expr
{
    std::string varName;
};

struct BinaryExpr : Expr
{
    std::unique_ptr<Expr> left;
    TokenType op;
    std::unique_ptr<Expr> right;
};


/**
 * @brief A relational comparison between two expressions.
 *
 * Not a Stmt or Expr — conditions are their own category, used
 * exclusively by if, while, and for statements. The relop field
 * stores a relational TokenType (GREATER, LESS, ISEQUAL, etc.).
 */
struct Condition
{
    std::unique_ptr<Expr> left;
    TokenType relop;
    std::unique_ptr<Expr> right;
};

struct Case
{
    int value;
    std::unique_ptr<Block> body;
};

