#include "token.h"

std::ostream& operator<<(std::ostream& os, const TokenType& type)
{
    switch (type) {
        case TokenType::NUM:          os << "NUM";         break;
        case TokenType::ID:           os << "ID";          break;
        case TokenType::IF:           os << "IF";          break;
        case TokenType::WHILE:        os << "WHILE";       break;
        case TokenType::SWITCH:       os << "SWITCH";      break;
        case TokenType::CASE:         os << "CASE";        break;
        case TokenType::DEFAULT:      os << "DEFAULT";     break;
        case TokenType::FOR:          os << "FOR";         break;
        case TokenType::VAR:          os << "VAR";         break;
        case TokenType::ELSE:         os << "ELSE";        break;
        case TokenType::INPUT:        os << "INPUT";       break;
        case TokenType::OUTPUT:       os << "OUTPUT";      break;
        case TokenType::PLUS:         os << "PLUS";        break;
        case TokenType::MINUS:        os << "MINUS";       break;
        case TokenType::MULT:         os << "MULT";        break;
        case TokenType::DIV:          os << "DIV";         break;
        case TokenType::EQUAL:        os << "EQUAL";       break;
        case TokenType::GREATER:      os << "GREATER";     break;
        case TokenType::LESS:         os << "LESS";        break;
        case TokenType::NOTEQUAL:     os << "NOTEQUAL";    break;
        case TokenType::GEQUAL:       os << "GEQUAL";      break;
        case TokenType::LEQUAL:       os << "LEQUAL";      break;
        case TokenType::ISEQUAL:      os << "ISEQUAL";     break;
        case TokenType::LBRACE:       os << "LBRACE";      break;
        case TokenType::RBRACE:       os << "RBRACE";      break;
        case TokenType::LPAREN:       os << "LPAREN";      break;
        case TokenType::RPAREN:       os << "RPAREN";      break;
        case TokenType::SEMICOLON:    os << "SEMICOLON";   break;
        case TokenType::COMMA:        os << "COMMA";       break;
        case TokenType::COLON:        os << "COLON";       break;
        case TokenType::END_OF_FILE:  os << "END_OF_FILE"; break;
        default:                      os << "UNKNOWN";
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& t)
{
    os << t.type << "(\"" << t.lexeme << "\") at line " << t.line_no;

    return os;
}
