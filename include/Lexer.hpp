#ifndef LEXER
#define LEXER
#include <iostream>
#include <vector>
#include "OplType.hpp"
#include <algorithm>

namespace OplLexer {

    class Lexer {
    public:
        Lexer(std::string expr) {
            this->expr = expr;
            pos = -1;
            advance();
            ln = 1;
            col = 1;
            make_token();
        }

        std::vector<OplType::Token> tokens;
    private:
        std::string expr;
        int pos;
        char current;
        int ln, col;

        void advance() {
            ++pos;
            col++;
            current = 0;
            if (pos < (int) expr.size()) {
                current = expr[pos];
                if (current == '\n' || current == '\r') {
                    tokens.push_back(OplType::Token{
                            .type = OplType::TT_NEW_LINE,
                            .data = "\n"
                    });
                    col = 1;
                    ln++;
                }
            }
        }

        OplType::Token make_digit() {
            OplType::Token result;
            result.type = OplType::TT_INTEGER;
            char nxt = expr[pos + 1];
            int type = 0; // 0 -> DEC, 1 -> HEX, 2 -> BIN, 3 -> OCT
            if (current == '0' && nxt == 'x') {
                type = 1;
                advance();
                advance();
            } else if (current == '0' && nxt == 'b' || nxt == 'B') {
                type = 2;
            } else if (current == '0')
                type = 3;
            while (current != 0 &&
                   (std::isdigit(current) || current == '.' || current == 'x' || current == 'b' || current == 'B' ||
                    ('a' <= current && current <= 'z') || ('A' <= current && current <= 'Z'))) {
                if (current == '.')
                    result.type = OplType::TT_DOUBLE;
                result.data += current;
                advance();
            }
            if (type == 1) result.data = OplType::hex_to_dec(result.data);
            else if (type == 2) result.data = OplType::bin_to_dec(result.data);
            else if (type == 3) result.data = OplType::oct_to_dec(result.data);
            return result;
        }

        OplType::Token make_ampersand() {
            OplType::Token result;
            int new_pos = pos + 1;
            if (new_pos < expr.size()) {
                char nxt = expr[new_pos];
                if (nxt == '&') {
                    advance();
                    advance();
                    return OplType::Token{.type = OplType::TT_AND, .data = "&&"};
                }
            }
            advance();
            return OplType::Token{
                    .type = OplType::TT_BIT_AND,
                    .data = "&"
            };
        }

        OplType::Token make_pipe() {
            OplType::Token result;
            int new_pos = pos + 1;
            if (new_pos < expr.size()) {
                char nxt = expr[new_pos];
                if (nxt == '|') {
                    advance();
                    advance();
                    return OplType::Token{.type = OplType::TT_OR, .data = "||"};
                }
            }
            advance();
            return OplType::Token{
                    .type = OplType::TT_BIT_OR,
                    .data = "|"
            };
        }

        OplType::Token make_string() {
            OplType::Token result;
            result.type = OplType::TT_STRING;
            char eof = current;
            advance();
            while (current != 0 && current != eof) {
                result.data += current;
                advance();
            }
            if (current != eof) throw std::runtime_error("error");
            advance();
            return result;
        }

        OplType::Token make_id() {
            OplType::Token result;
            result.type = OplType::TT_ID;
            while (current != 0 && IS_WORD(current) || isdigit(current)) {
                result.data += current;
                advance();
            }
            if (result.data == "true" || result.data == "false") {
                result.type = OplType::TT_INTEGER;
                result.data = result.data == "true" ? "1" : "0";
                return result;
            }
            if (std::count(OplType::keys.begin(), OplType::keys.end(), result.data)) result.type = OplType::TT_KEY;
            return result;
        }

        OplType::Token make_plus() {
            int new_pos = pos + 1;
            if (new_pos < expr.size()) {
                char tmp = expr[new_pos];
                if (tmp == '=' || tmp == '+') {
                    std::string a;
                    a += current;
                    advance();
                    a += current;
                    advance();
                    return OplType::Token{.type = OplType::TT_SELFADD, .data = a};
                } else {
                    advance();
                    return OplType::Token{.type = OplType::TT_PLUS, .data = "+"};
                }
            } else {
                advance();
                return OplType::Token{.type = OplType::TT_PLUS, .data = "+"};
            }
        }

        OplType::Token make_sub() {
            int new_pos = pos + 1;
            if (new_pos < expr.size()) {
                char tmp = expr[new_pos];
                if (tmp == '=' || tmp == '-') {
                    std::string a;
                    a += current;
                    advance();
                    a += current;
                    advance();
                    return OplType::Token{.type = OplType::TT_SELFSUB, .data = a};
                } else {
                    advance();
                    return OplType::Token{.type = OplType::TT_SUB, .data = "-"};
                }
            } else {
                advance();
                return OplType::Token{.type = OplType::TT_SUB, .data = "-"};
            }
        }

        OplType::Token make_mul() {
            int new_pos = pos + 1;
            if (new_pos < expr.size()) {
                char tmp = expr[new_pos];
                if (tmp == '=') {
                    std::string a;
                    a += current;
                    advance();
                    a += current;
                    advance();
                    return OplType::Token{.type = OplType::TT_SELFMUL, .data = a};
                } else {
                    advance();
                    return OplType::Token{.type = OplType::TT_MUL, .data = "*"};
                }
            } else {
                advance();
                return OplType::Token{.type = OplType::TT_MUL, .data = "*"};
            }
        }

        OplType::Token make_percent() {
            int new_pos = pos + 1;
            if (new_pos < expr.size()) {
                char tmp = expr[new_pos];
                if (tmp == '=') {
                    std::string a;
                    a += current;
                    advance();
                    a += current;
                    advance();
                    return OplType::Token{.type = OplType::TT_SELF_MOD, .data = a};
                } else {
                    advance();
                    return OplType::Token{.type = OplType::TT_MOD, .data = "%"};
                }
            } else {
                advance();
                return OplType::Token{.type = OplType::TT_MOD, .data = "%"};
            }
        }

        OplType::Token make_div() {
            int new_pos = pos + 1;
            if (new_pos < expr.size()) {
                char tmp = expr[new_pos];
                if (tmp == '=') {
                    std::string a;
                    a += current;
                    advance();
                    a += current;
                    advance();
                    return OplType::Token{.type = OplType::TT_SELFDIV, .data = a};
                } else {
                    advance();
                    return OplType::Token{.type = OplType::TT_DIV, .data = "/"};
                }
            } else {
                advance();
                return OplType::Token{.type = OplType::TT_DIV, .data = "/"};
            }
        }

        OplType::Token make_pow() {
            advance();
            return OplType::Token{.type = OplType::TT_POW, .data = "^"};
        }

        OplType::Token make_lbracket_s() {
            advance();
            return OplType::Token{.type = OplType::TT_LBRACKET_S, .data = "("};
        }

        OplType::Token make_rbracket_s() {
            advance();
            return OplType::Token{.type = OplType::TT_RBRACKET_S, .data = ")"};
        }

        OplType::Token make_lbracket_m() {
            advance();
            return OplType::Token{.type = OplType::TT_LBRACKET_M, .data = "["};
        }

        OplType::Token make_rbracket_m() {
            advance();
            return OplType::Token{.type = OplType::TT_RBRACKET_M, .data = "]"};
        }

        OplType::Token make_lbracket_b() {
            advance();
            return OplType::Token{.type = OplType::TT_LBRACKET_B, .data = "{"};
        }

        OplType::Token make_rbracket_b() {
            advance();
            return OplType::Token{.type = OplType::TT_RBRACKET_B, .data = "}"};
        }

        OplType::Token make_eq() {
            int new_pos = pos + 1;
            if (new_pos >= expr.size()) {
                EQ:
                advance();
                return OplType::Token{.type = OplType::TT_ASSIGN, .data = "="};
            }
            char s = expr[new_pos];
            if (s != '=') goto EQ;
            advance();
            advance();
            return OplType::Token{.type = OplType::TT_EQ, .data = "=="};
        }

        OplType::Token make_comma() {
            advance();
            return OplType::Token{.type = OplType::TT_COMMA, .data = ","};
        }

        OplType::Token make_period() {
            advance();
            return OplType::Token{.type = OplType::TT_PERIOD, .data="."};
        }

        OplType::Token make_biger() {
            if (pos < (int) expr.size() - 1) {
                char nxt = expr[pos + 1];
                if (nxt == '=') {
                    advance();
                    advance();
                    return OplType::Token{.type = OplType::TT_BRE, .data = ">="};
                }
                if (nxt == '>') {
                    advance();
                    advance();
                    return OplType::Token{.type = OplType::TT_RIGHT_MOVE, .data = ">>"};
                }
            }
            advance();
            return OplType::Token{.type = OplType::TT_BIGER, .data = ">"};
        }

        OplType::Token make_smaller() {
            if (pos < (int) expr.size() - 1) {
                char nxt = expr[pos + 1];
                // <=
                if (nxt == '=') {
                    advance();
                    advance();
                    return OplType::Token{.type = OplType::TT_SRE, .data = "<="};
                }
                // <<
                if (nxt == '<') {
                    advance();
                    advance();
                    return OplType::Token{.type = OplType::TT_LEFT_MOVE, .data = "<<"};
                }
            }
            advance();
            return OplType::Token{.type = OplType::TT_SMALLER, .data = "<"};
        }

        OplType::Token make_semicolon() {
            advance();
            return OplType::Token{.type = OplType::TT_SEMICOLON, .data = ";"};
        }

        OplType::Token make_not() {
            int new_pos = pos + 1;
            if (new_pos < expr.size()) {
                char nxt = expr[new_pos];
                if (nxt == '=') {
                    advance();
                    advance();
                    return OplType::Token{
                            .type = OplType::TT_NOT_EQ,
                            .data = "!="
                    };
                }
            }
            advance();
            return OplType::Token{
                    .type = OplType::TT_NOT,
                    .data = "!"
            };
        }

        OplType::Token make_colon() {
            advance();
            return OplType::Token{.type = OplType::TT_COLON, .data = ":"};
        }

        void skip() { while (current != 0 && current != '\n' && current != '\r') advance(); }

        void make_token() {
            while (current != 0) {
                if (IS_WORD(current)) tokens.push_back(make_id());
                else if (std::isdigit(current)) tokens.push_back(make_digit());
                else if (current == '"' || current == '\'') tokens.push_back(make_string());
                else if (current == '+') tokens.push_back(make_plus());
                else if (current == '.') tokens.push_back(make_period());
                else if (current == '!') tokens.push_back(make_not());
                else if (current == '-') tokens.push_back(make_sub());
                else if (current == ';') tokens.push_back(make_semicolon());
                else if (current == '|') tokens.push_back(make_pipe());
                else if (current == '>') tokens.push_back(make_biger());
                else if (current == '<') tokens.push_back(make_smaller());
                else if (current == '*') tokens.push_back(make_mul());
                else if (current == '&') tokens.push_back(make_ampersand());
                else if (current == '#') skip();
                else if (current == ':') tokens.push_back(make_colon());
                else if (current == '/') tokens.push_back(make_div());
                else if (current == '^') tokens.push_back(make_pow());
                else if (current == '(') tokens.push_back(make_lbracket_s());
                else if (current == ')') tokens.push_back(make_rbracket_s());
                else if (current == '%') tokens.push_back(make_percent());
                else if (current == '=') tokens.push_back(make_eq());
                else if (current == '[') tokens.push_back(make_lbracket_m());
                else if (current == ']') tokens.push_back(make_rbracket_m());
                else if (current == ',') tokens.push_back(make_comma());
                else if (current == '{') tokens.push_back(make_lbracket_b());
                else if (current == '}') tokens.push_back(make_rbracket_b());
                else if (std::isspace(current)) advance();
                else {
                    printf("Parse time error, at line %d, column %d, position %d: \n", ln, col, pos);
                    printf("  Info: un supposed char '%c'\n", current);
                    exit(-1);
                }
            }
        }
    };
}

#endif 