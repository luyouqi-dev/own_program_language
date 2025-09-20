#ifndef CPROGRAMLANGUAGE_PARSER_HPP
#define CPROGRAMLANGUAGE_PARSER_HPP
#include <iostream>
#include "BuildIn.hpp"
#include <vector>
#include "Lexer.hpp"
#include <unordered_map>
#include "Tools.hpp"
#include <stack>
#include <algorithm>
#include "OplType.hpp"

namespace OplParser {

    class Parser;

    typedef OplType::AST *(Parser::*func)();

    struct VarList {
        vector<string> v;
        map<string, OplType::SymbolKind> symbol_kinds;
        map<string, OplType::IdTypeNode> var_type_record;

        void add(string, OplType::SymbolKind);

        void add_var(string, OplType::IdTypeNode);

        bool is_have(string);

        bool is_haveA(string, OplType::SymbolKind);

        VarList();
    };

    VarList::VarList() {}

    void VarList::add_var(string name, OplType::IdTypeNode type) {
        var_type_record[name] = type;
    }

    bool VarList::is_haveA(std::string name, OplType::SymbolKind kind) {
        if (is_have(name) && symbol_kinds[name] == kind)
            return true;
        return false;
    }

    void VarList::add(std::string s, OplType::SymbolKind kd) {
        if (std::find(v.begin(), v.end(), s) != v.end()) {
            err_out(PARSE_TIME_ERROR, "var '%s' double define", s.c_str());
        }
        symbol_kinds[s] = kd;
        v.push_back(s);
    }

    bool VarList::is_have(std::string s) {
        return std::find(v.begin(), v.end(), s) != v.end();
    }

    class Parser {
    public:
        Parser(std::vector<OplType::Token> tokens, int file = 1) {
            _tokens = tokens;
            export_map = &result.export_map;
            lin = col = 1;
            pos = -1;
            advance();
            for (auto i: OplBuildIn::build_in_class) class_map[i] = make_class_id();
            for (auto i: OplBuildIn::build_in_class) class_size_record[i] = 1;
            scope.emplace_back();
            for (auto i: OplBuildIn::build_in_class) add_to_global_scope(i, OplType::SK_CLASS);
            class_map["list"] = 1;   // system type
            class_map["lambda"] = 1; // system type
            for (auto i: OplBuildIn::vm_build_in_fn_return_type_map) func_return_type_record[i.first] = i.second;
            main_parse();
        }

        int lin, col;
        OplType::Program result;

        void main_parse();

        map<string, int> class_size_record;
        string current_class;
        unordered_map<string, OplType::ParseTimeClassRecord> ptcr;
        unordered_map<string, OplType::IdTypeNode> func_return_type_record;
        unordered_map<string, OplType::IdTypeNode> lambda_func_return_type_record;
        unordered_map<string, int> class_map;
        unordered_map<string, OplType::ExportId> *export_map;
        VarList global;
        int class_cnt, func_cnt, var_cnt;

        int make_class_id();

        int pos;
        vector<VarList> scope;
    private:
        void leave();

        void crate();

        bool is_define(string);

        void check_type(OplType::AST *);

        bool is_defineA(string, OplType::SymbolKind);

        void add(string, OplType::SymbolKind);

        void add_var(string, OplType::IdTypeNode);

        OplType::IdTypeNode get_type(string);

        void consume(std::string, std::string);

        void consume_type(std::string, std::string);

        OplType::Token *current;

        OplType::AST *make_string_node();

        std::vector<OplType::Token> _tokens;

        void advance();

        OplType::AST *make_if();

        OplType::AST *make_while();

        OplType::AST *make_a_assign();

        OplType::AST *make_for();

        OplType::AST *make_case_node();

        OplType::AST *make_default_node();

        OplType::AST *make_switch_node();

        vector<OplType::AST *> make_templates();

        OplType::AST *make_load_library();

        OplType::AST *make_else();

        OplType::AST *make_return_node();

        OplType::AST *make_var_define();

        OplType::AST *abs_call();

        OplType::AST *make_assign(OplType::AST *);

        OplType::AST *make_mem_malloc();

        OplType::AST *make_block();

        OplType::AST *atom_make(bool &);

        OplType::AST *make_member_node();

        OplType::AST *make_class();

        OplType::AST *make_inc_node(OplType::AST *);

        OplType::AST *make_dec_node(OplType::AST *);

        OplType::AST *make_id_atom();

        OplType::AST *make_list();

        int get_class_size(OplType::AST *);

        OplType::AST *make_digit();

        bool match_data(std::string);

        OplType::AST *make_not_node();

        OplType::AST *make_bit_node();

        OplType::AST *make_lambda();

        OplType::AST *make_type_node();

        OplType::AST *make_term();

        OplType::Token want_get(string);

        OplType::AST *make_load_module_node();

        OplType::AST *make_templates(OplType::Token);

        OplType::AST *make_expr_term();

        OplType::AST *make_constructor_call(OplType::AST *);

        OplType::AST *make_constructor_call_();

        OplType::AST *make_expr();

        OplType::AST *make_get_value_node(OplType::AST *);

        OplType::IdTypeNode derive_var_type(OplType::AST *);

        OplType::AST *make_and_node();

        OplType::AST *make_comp_node();

        OplType::AST *make_self_add_node(OplType::AST *);

        OplType::AST *make_self_sub_node(OplType::AST *);

        OplType::AST *make_break();

        OplType::AST *make_continue();

        OplType::AST *make_self_div_node(OplType::AST *);

        OplType::AST *make_import_node();

        OplType::AST *make_self_mul_node(OplType::AST *);

        OplType::AST *make_function_define();

        OplType::AST *make_cast_node(OplType::AST *);

        bool type_comp(OplType::IdTypeNode, OplType::IdTypeNode);

        OplType::AST *make_or_node();

        OplType::AST *atom();

        OplType::AST *make_func_call(OplType::AST *);

        void make_lib_class(OplType::Token);

        bool match_type(std::string);

        void add_to_global_scope(string, OplType::SymbolKind);

        OplType::AST *make_bin_op_node(func, std::vector<string>, func);

        bool is_class_member(OplType::Token);

        string get_mem_type(string, string);

        bool is_type(OplType::AST *);

        bool is_exist(string);
    };

    bool Parser::is_exist(std::string var_name) {
        for (auto i: scope)
            if (i.is_have(var_name))
                return true;
        return false;
    }

    bool Parser::type_comp(OplType::IdTypeNode a, OplType::IdTypeNode b) {
        if (a.rootType != b.rootType || a.templateType.size() != b.templateType.size())
            return false;
        for (int i = 0; i < a.templateType.size(); ++i)
            if (!type_comp(a.templateType[i], b.templateType[i]))
                return false;
        return true;
    }

    OplType::IdTypeNode Parser::derive_var_type(OplType::AST *a) {
        if (a->type == OplType::AST_STRING) {
            OplType::IdTypeNode itn;
            itn.rootType = "string";
            return itn;
        } else if (a->type == OplType::AST_LAMBDA) {
            OplType::IdTypeNode itn;
            itn.rootType = "lambda";
            OplType::IdTypeNode titn;
            for (auto i: a->children)
                titn.templateType.push_back(i->idt);
            itn.templateType.push_back(a->idt);
            return itn;
        } else if (a->type ==OplType:: AST_DIGIT) {
            OplType::IdTypeNode itn;
            itn.rootType = "int";
            return itn;
        } else if (a->type == OplType::AST_LIST) {
            OplType::IdTypeNode itn;
            itn.rootType = "list";
            itn.is_block = true;
            if (!a->children.empty()) {
                auto type = derive_var_type(a->children[0]);
                for (auto &i: a->children) {
                    auto ct = derive_var_type(i);
                    if (!type_comp(type, ct)) {
                        printf("TypeError\n");
                        exit(-1);
                    }
                    itn.templateType.push_back(ct);
                }
            } else {
                printf("need a value\n");
                exit(-1);
            }
            return itn;
        } else if (a->type == OplType::AST_BIN_OP) {
            auto left = a->children[0];
            auto right = a->children[1];
            auto lt = derive_var_type(left);
            auto rt = derive_var_type(right);
            if (!type_comp(lt, rt)) {
                printf("TypeError: unsupported operand type for %s: '%s' and '%s'\n",
                       a->data.data.c_str(), lt.rootType.c_str(), rt.rootType.c_str());
                exit(-1);
            }
            return lt;
        } else if (a->type == OplType::AST_MEMBER) {
            if (a->children.size() >= 2) {
                OplType::IdTypeNode itn;
                auto parentName = a->parent_name;
                itn.rootType = ptcr[parentName.rootType].get_root_type(a->children[1]->data.data);
                return itn;
            } else {
                printf("a->children.size() < 2\n");
                exit(-1);
            }
        } else if (a->type == OplType::AST_FUNC_CALL || a->type == OplType::AST_LAMBDA_CALL) {
            if (a->type == OplType::AST_FUNC_CALL) {
                string func_name;
                auto tmp = a->children[0];
                if (tmp->type == OplType::AST_ID) func_name = tmp->data.data;
                else func_name = tmp->children[1]->data.data;
                auto ret_val_t = func_return_type_record.find(func_name);
                if (ret_val_t == func_return_type_record.end()) {
                    printf("TypeError: function return value type unkonwn\n");
                    exit(-1);
                }
                return ret_val_t->second;
            } else {
                auto func_name_node = a->id_node;
                string func_name;
                if (func_name_node->type == OplType::AST_ID)
                    func_name = func_name_node->data.data;
                else
                    func_name = func_name_node->children[1]->data.data;
                auto ret_t = lambda_func_return_type_record.find(func_name);
                if (ret_t == lambda_func_return_type_record.end()) {
                    printf("TypeError: function return value type unkonwn\n");
                    exit(-1);
                }
                return ret_t->second;
            }
        } else if (a->type == OplType::AST_CONV) {
            return make_id_type_node(a->children[0]);
        } else if (a->type == OplType::AST_MEM_MALLOC) {
            return a->idt;
        } else if (a->type == OplType::AST_ID) {
            auto t = scope[scope.size() - 1].var_type_record.find(a->data.data);
            if (t == scope[scope.size() - 1].var_type_record.end()) {
                cout << "Name '" << a->data.data << "' not found\n";
                exit(-1);
            }
            return t->second;
        } else if (a->type == OplType::AST_GET_VALUE) {
            return a->parent_name;
        } else {
            cout << "Unknown type: " << a->type << endl;
            exit(-1);
        }
    }

    void Parser::make_lib_class(OplType::Token t) {
        string path = t.data;
        string data = read_file(path);
    }

    string Parser::get_mem_type(string class_name, string mem_name) {
        return ptcr[class_name].get_root_type(mem_name);
    }

    OplType::IdTypeNode Parser::get_type(string s) {
        for (auto i: scope)
            if (i.var_type_record.find(s) != i.var_type_record.end())
                return i.var_type_record[s];
        return scope[scope.size() - 1].var_type_record[s];
    }

    void Parser::add_var(string vn, OplType::IdTypeNode vt) {
        scope[scope.size() - 1].add_var(vn, vt);
    }

    OplType::AST *Parser::make_bin_op_node(func left, std::vector<string> ops, func right = nullptr) {
        if (!right) right = left;
        OplType::AST *_left = (this->*left)();
        while (current && std::count(ops.begin(), ops.end(), current->data) > 0) {
            OplType::Token op = *current;
            advance();
            OplType::AST *_right = (this->*right)();
            _left = new OplType::BinOpNode(op, _left, _right);
        }
        return _left;
    }

    OplType::AST *Parser::make_bit_node() { return make_bin_op_node(&Parser::make_expr_term, {"<<", ">>", "|", "&"}); }

    bool Parser::is_defineA(std::string name, OplType::SymbolKind kind) {
        for (auto i: scope) if (i.is_haveA(name, kind)) return true;
        return false;
    }

    void Parser::add_to_global_scope(string s, OplType::SymbolKind kd) { scope[0].add(s, kd); }

    OplType::AST *Parser::make_templates(OplType::Token id) {
        vector<OplType::AST *> template_list;
        if (match_data("<")) {
            advance();
            while (current && !match_data(">")) {
                current->debug();
                auto tmp = make_type_node();
                add(tmp->data.data, OplType::SK_CLASS);
                template_list.push_back(tmp);
                if (match_data(">")) break;
                consume(",", __func__);
            }
            consume(">", __func__);
        }
        return new OplType::TypeNode(id, template_list);
    }

    OplType::AST *Parser::make_type_node() {
        auto root_type = *current;
        bool is_block = false;
        OplType::AST *block_size = nullptr;
        advance();
        vector<OplType::AST *> template_list;
        if (match_data("<")) {
            advance();
            while (current && !match_data(">")) {
                template_list.push_back(make_type_node());
                if (match_data(">")) break;
                consume(",", __func__);
            }
            consume(">", __func__);
        }
        if (match_data(":")) {
            advance();
            auto ret_type = make_type_node();
            template_list.push_back(ret_type);
        }
        if (match_data("[")) {
            is_block = true;
            advance();
            if (!match_data("]")) {
                block_size = make_expr();
                consume("]", __func__);
            } else {
                advance();
            }
            auto *a = new OplType::AST;
            a->data = root_type;
            a->type = OplType::AST_TYPE;
            template_list.push_back(a);
            root_type.data = "list";
        }
        auto n = new OplType::TypeNode(root_type, template_list, is_block, block_size);
        check_type(n);
        return n;
    }

    bool Parser::is_class_member(OplType::Token a) {
        return class_map.find(a.data) != class_map.end() || is_defineA(a.data, OplType::SK_CLASS);
    }

    OplType::AST *Parser::make_cast_node(OplType::AST *target_type) {
        auto expr_ = make_expr();
        return new OplType::CastNode(target_type, expr_);
    }

    void Parser::advance() {
        ++pos;
        ++col;
        current = nullptr;
        if (pos < _tokens.size()) {
            current = &_tokens[pos];
            if (current->type == OplType::TT_NEW_LINE) {
                col = 1;
                lin++;
                advance();
            }
        }
    }

    OplType::AST *Parser::make_member_node() {
        OplType::AST *base = make_id_atom();
        OplType::IdTypeNode type;
        if (base->type == OplType::AST_TYPE)
            return base;
        string base_name = base->data.data;
        type = get_type(base_name);
        if (match_data("[")) {
            advance();
            auto expr = make_expr();
            consume("]", __func__);
            base = new OplType::GetValueAtPosition(base, expr);
            type = type.templateType[0];
            base->parent_name = type;
        }
        if (match_data("(")) {
            if (
                    func_return_type_record.find(base_name) == func_return_type_record.end() &&
                    lambda_func_return_type_record.find(base_name) == lambda_func_return_type_record.end()
                    ) {
                printf("Name '%s' not a function\n", base_name.c_str());
                exit(-1);
            }
            base->is_end = true;
            base = make_func_call(base);
            auto rt1 = func_return_type_record.find(base_name);
            auto rt2 = lambda_func_return_type_record.find(base_name);
            if (rt1 != func_return_type_record.end()) {
                type = rt1->second;
            } else {
                type = rt2->second;
            }
        }
        while (current && match_data(".") || match_data("(") || match_data("[")) {
            consume(".", __func__);
            OplType::AST *member = make_id_atom();
            if (current_class != type.rootType) {
                string name = member->data.data;
                if (ptcr[type.rootType].var_access_status[name] != OplType::PUBLIC_MEMBER) {
                    printf("Name '%s' not a public member\n", name.c_str());
                    exit(-1);
                }
            }
            if (member->type == OplType::AST_TYPE)
                return member;
            if (match_data("[")) {
                advance();
                auto size = make_expr();
                consume("]", __func__);
                base = new OplType::MemberNode(base, member);
                base->parent_name = type;
                base->is_end = true;
                base = new OplType::GetValueAtPosition(base, size);
                type = ptcr[type.rootType].get_type(member->data.data);
                base->parent_name = type.templateType[0].rootType;
                type = type.templateType[0].rootType;
            } else if (match_data("(")) {
                string func_name = member->data.data;
                auto father_class = ptcr[type.rootType];
                auto access_status = father_class.func_access_status.find(func_name);
                if (access_status == father_class.func_access_status.end()) {
                    auto vc = father_class.var_access_status.find(func_name);
                    if (vc == father_class.var_access_status.end()) {
                        printf("Name '%s' not found\n", func_name.c_str());
                        exit(-1);
                    } else {
                        auto type_ = father_class.var_type_record.find(func_name);
                        if (type_ == father_class.var_type_record.end()) {
                            printf("Var '%s' not found\n", func_name.c_str());
                            exit(-1);
                        }
                        auto tt = type_->second.rootType;
                        if (tt != "lambda") {
                            printf("Var '%s' not a function\n", func_name.c_str());
                            exit(-1);
                        }
                        base = new OplType::MemberNode(base, member);
                        base->parent_name = type;
                        base->children[1]->is_end = true;
                        base = make_func_call(base);
                        type = father_class.var_type_record[func_name].templateType[1];
                    }
                } else {
                    if (current_class != type.rootType) {
                        if (access_status->second != OplType::PUBLIC_MEMBER) {
                            printf("FunctionName '%s' not a public member\n", func_name.c_str());
                            exit(-1);
                        }
                    }
                    base = new OplType::MemberNode(base, member);
                    base->parent_name = type;
                    base->children[1]->is_end = true;
                    base = make_func_call(base);
                    type = father_class.func_type_record[func_name];
                }
            } else {
                string data = member->data.data;
                base = new OplType::MemberNode(base, member);
                if (
                        ptcr[type.rootType].var_type_record.find(data) == ptcr[type.rootType].var_type_record.end() &&
                        ptcr[type.rootType].func_type_record.find(data) == ptcr[type.rootType].func_type_record.end()
                        ) {
                    printf("Name '%s' not found in class '%s'\n", data.c_str(), type.rootType.c_str());
                    exit(-1);
                }
                if (current_class != type.rootType) {
                    if (ptcr[type.rootType].func_access_status[data] != OplType::PUBLIC_MEMBER) {
                        printf("FunctionName '%s' not a public member\n", data.c_str());
                        exit(-1);
                    }
                }
                base->parent_name = type;
                type = ptcr[type.rootType].get_type(member->data.data);
            }
        }
        if (base->type == OplType::AST_ID)
            base->is_end = true;
        if (base->type == OplType::AST_MEMBER)
            base->children[1]->is_end = true;
        return base;
    }

    void Parser::consume(std::string s, string call_id) {
        if (!current) err_out(PARSE_TIME_ERROR, "Unexpected EOF in lin %d, col %d, pos %d", lin, col, pos);
        if (current->data == s) {
            advance();
            return;
        }
        cout << "call_id = " << call_id << endl;
        err_out(PARSE_TIME_ERROR, "want '%s', meet '%s', at lin %d, col %d, pos %d", s.c_str(), current->data.c_str(),
                lin, col, pos);
    }

    vector<OplType::AST *> Parser::make_templates() {
        vector<OplType::AST *> temps;
        consume("<", __func__);
        while (current && !match_data(">")) {
            if (current->type != OplType::TT_ID) {
                err_out(PARSE_TIME_ERROR, "Expected template parameter name at lin %d, col %d, pos %d", lin, col, pos);
            }
            OplType::Token param = *current;
            advance();
            add(param.data, OplType::SK_CLASS);
            temps.push_back(new OplType::IdNode(param));
            if (match_data(">")) break;
            consume(",", __func__);
        }
        consume(">", __func__);
        return temps;
    }

    OplType::AST *Parser::make_id_atom() {
        if (!current) return nullptr;
        OplType::AST *idp = new OplType::IdNode(*current);
        advance();
        auto nxt = _tokens[pos + 1];
        bool is_t = class_map.find(idp->data.data) != class_map.end();
        if (is_t && match_data("<") && class_map[nxt.data]) {
            auto temp = make_templates();
            idp->children = temp;
        }
        return idp;
    }

    bool Parser::match_data(std::string d) {
        if (!current) return false;
        return current->data == d;
    }

    bool Parser::match_type(std::string t) {
        if (!current) return false;
        return current->type == t;
    }

    OplType::AST *Parser::make_term() {
        return make_bin_op_node(&Parser::atom, {"*", "/", "%"});
    }

    OplType::AST *Parser::atom() {
        if (match_data("lambda")) {
            return make_lambda();
        }
        if (match_data("-")) {
            advance();
            auto num = atom();
            return new OplType::BinOpNode(OplType::Token{
                    .type = OplType::TT_MUL,
                    .data = "*"
            }, new OplType::DigitNode(OplType::Token{
                    .type = OplType::TT_INTEGER,
                    .data = "-1"
            }), num);
        }
        if (match_data("load_library")) {
            return make_load_library();
        }
        if (match_data("["))
            return make_list();
        if (match_data("!")) {
            return make_not_node();
        }
        if (match_data("++")) {
            advance();
            auto id = make_member_node();
            return make_inc_node(id);
        }
        if (match_data("--")) {
            advance();
            auto id = make_member_node();
            return make_dec_node(id);
        }
        if (match_type(OplType::TT_KEY)) {
            if (match_data("new")) return make_mem_malloc();
            else err_out(PARSE_TIME_ERROR, "Key '%s' not supposed, at lin %d, col %d, pos %d", current->data.c_str(),
                         lin, col, pos);
        }
        if (match_type(OplType::TT_DOUBLE) || match_type(OplType::TT_INTEGER)) return make_digit();
        if (match_type(OplType::TT_ID)) {
            auto tmp = make_member_node();
            string data = tmp->data.data;
            if (!is_define(data) && match_type(OplType::TT_ID)) {
                printf("Error: symbol '%s' is not define in this scope\n", data.c_str());
                printf("At lin %d, col %d, pos %d\n", lin, col, pos);
                printf("Current data: ");
                current->debug();
                exit(-1);
            }
            if (match_data("=")) {
                return make_assign(tmp);
            }
            if (match_data("("))
                return make_func_call(tmp);
            if (match_data("++")) {
                advance();
                return make_inc_node(tmp);
            }
            if (match_data("--")) {
                advance();
                return make_dec_node(tmp);
            }
            return tmp;
        }
        if (match_type(OplType::TT_STRING)) return make_string_node();
        consume("(", __func__);
        auto tok = *current;
        auto tmp = make_expr();
        if (is_class_member(tok)) {
            auto tp = make_templates(tok);
            consume(")", __func__);
            return make_cast_node(tp);
        }
        consume(")", __func__);
        return tmp;
    }

    OplType::AST *Parser::make_digit() {
        auto ni = new OplType::DigitNode(*current);
        advance();
        return ni;
    }

    OplType::AST *Parser::make_expr_term() {
        return make_bin_op_node(&Parser::make_term, {"+", "-"});
    }

    OplType::AST *Parser::make_string_node() {
        auto tmp = new OplType::StringNode(*current);
        advance();
        return tmp;
    }

    OplType::AST *Parser::make_var_define() {
        ++var_cnt;
        OplType::Token var_name;
        OplType::AST *type = nullptr;
        OplType::AST *data = nullptr;
        if (match_data("let")) advance();
        if (!match_type(OplType::TT_ID)) err_out(PARSE_TIME_ERROR, "want TT_ID, meet %s, lin %d, col %d, pos %d",
                                        current->type.c_str(), lin, col, pos);
        var_name = *current;
        add(var_name.data, OplType::SK_VAR);
        advance();
        if (match_data(":")) {
            advance();
            type = make_type_node();
            check_type(type);
        }
        if (match_data("=")) {
            advance();
            data = make_expr();
        }
        auto v = new OplType::VarDefineNode(var_name, type, data);
        if (type) {
            auto type_ = make_id_type_node(type);
            v->idt = type_;
            scope[scope.size() - 1].add_var(var_name.data, type_);
        } else {
            if (!data) {
                printf("Need a type or initial value\n");
                exit(-1);
            }
            auto _type = derive_var_type(data);
            v->idt = _type;
            scope[scope.size() - 1].add_var(var_name.data, _type);
        }
        if (v->idt.rootType == "lambda") {
            auto ret_t = v->idt.templateType[v->idt.templateType.size() - 1];
            lambda_func_return_type_record[var_name.data] = ret_t;
        }
        return v;
    }

    OplType::AST *Parser::make_and_node() {
        return make_bin_op_node(&Parser::make_comp_node, {"and", "&&"});
    }

    OplType::AST *Parser::make_or_node() {
        return make_bin_op_node(&Parser::make_and_node, {"or", "||"});
    }

    OplType::AST *Parser::make_comp_node() {
        return make_bin_op_node(&Parser::make_bit_node, {"<", ">", "<=", ">=", "==", "!="});
    }

    OplType::AST *Parser::make_expr() {
        return make_or_node();
    }

    OplType::AST *Parser::make_func_call(OplType::AST *name) {
        vector<OplType::AST *> value;
        consume("(", __func__);
        while (current && !match_data(")")) {
            value.push_back(make_expr());
            if (match_data(")")) break;
            consume(",", __func__);
        }
        consume(")", __func__);
        string func_name;
        if (name->type == OplType::AST_ID)
            func_name = name->data.data;
        else if (name->type == OplType::AST_MEMBER)
            func_name = name->children[1]->data.data;
        if (lambda_func_return_type_record.find(func_name) != lambda_func_return_type_record.end())
            return new OplType::LambdaCallNode(name, value);
        auto r = new OplType::FunctionCallNode(name, value);
        return r;
    }

    void Parser::consume_type(std::string type, std::string func) {
        if (!current || current->type != type) {
            err_out(PARSE_TIME_ERROR, "want %s, meet %s, at lin %d, col %d, pos %d", type.c_str(),
                    current->data.c_str(), lin, col, pos);
        }
    }

    OplType::AST *Parser::abs_call() {
        auto mn = make_id_atom();
        auto res = make_func_call(mn);
        return res;
    }

    OplType::AST *Parser::atom_make(bool &should_split) {
        if (match_data("++")) {
            should_split = true;
            advance();
            auto id = make_member_node();
            return make_inc_node(id);
        }
        if (match_data("--")) {
            should_split = true;
            advance();
            auto id = make_member_node();
            return make_dec_node(id);
        }
        if (match_type(OplType::TT_KEY)) {
            if (match_data("let")) {
                auto tmp = make_var_define();
                should_split = true;
                return tmp;
            } else if (match_data("export")) {
                advance();
                if (match_data("function")) {
                    should_split = false;
                    auto node = make_function_define();
                    (*export_map)[node->data.data] = OplType::FUNCTION_EXPORT;
                    return node;
                } else if (match_data("let")) {
                    should_split = true;
                    auto node = make_var_define();
                    string name = node->data.data;
                    (*export_map)[node->data.data] = OplType::VAR_EXPORT;
                    return node;
                } else if (match_data("class")) {
                    auto node = make_class();
                    (*export_map)[node->data.data] = OplType::CLASS_EXPORT;
                    should_split = false;
                    return node;
                } else {
                    printf("Parse time error, at lin %d, col %d, pos %d, when export\n", lin, col, pos);
                    exit(-1);
                }
            } else if (match_data("module")) {
                should_split = true;
                return make_load_module_node();
            } else if (match_data("using")) {
                should_split = true;
                return make_import_node();
            } else if (match_data("continue")) {
                should_split = true;
                return make_continue();
            } else if (match_data("switch")) {
                should_split = false;
                return make_switch_node();
            } else if (match_data("break")) {
                should_split = true;
                return make_break();
            } else if (match_data("return")) {
                should_split = true;
                auto node = make_return_node();
                return node;
            } else if (match_data("class")) {
                should_split = false;
                return make_class();
            } else if (match_data("for")) {
                should_split = false;
                return make_for();
            } else if (match_data("while")) {
                return make_while();
            } else if (match_data("if")) {
                should_split = false;
                return make_if();
            } else if (match_data("function")) {
                should_split = false;
                return make_function_define();
            } else {
                err_out(PARSE_TIME_ERROR, "Key '%s' not supposed at lin %s, col %d, pos %d", current->data.c_str(), lin,
                        col, pos);
            }
        } else if (match_type(OplType::TT_ID)) {
            should_split = true;
            auto id = make_member_node();
            if (match_data("++")) {
                advance();
                return make_inc_node(id);
            }
            if (match_data("--")) {
                advance();
                return make_dec_node(id);
            }
            if (match_data("+=")) return make_self_add_node(id);
            if (match_data("-=")) return make_self_sub_node(id);
            if (match_data("/=")) return make_self_div_node(id);
            if (match_data("*=")) return make_self_mul_node(id);
            if (match_data("=")) return make_assign(id);
            if (match_data("(")) return make_func_call(id);
            return id;
        }
        return nullptr;
    }

    OplType::AST *Parser::make_block() {
        vector<OplType::AST *> tmp;
        if (match_data("{")) {
            consume("{", __func__);
            while (current && !match_data("}")) {
                if (match_data("}")) break;
                bool isr = false;
                auto _tmp = atom_make(isr);
                tmp.push_back(_tmp);
                if (isr && current && match_data(";")) {
                    consume(";", __func__);
                } else if (isr) {
                    err_out(PARSE_TIME_ERROR, "Expected ; but got %s at lin %d, col %d, pos %d", current->data.c_str(),
                            lin, col, pos);
                }
            }
            consume("}", __func__);
        } else {
            bool isr = false;
            auto _tmp = atom_make(isr);
            tmp.push_back(_tmp);
            if (isr && current && match_data(";")) {
                consume(";", __func__);
            } else if (isr) {
                err_out(PARSE_TIME_ERROR, "Expected ; but got %s at lin %d, col %d, pos %d", current->data.c_str(), lin,
                        col, pos);
            }
        }
        return new OplType::BlockNode(tmp);
    }

    OplType::AST *Parser::make_function_define() {
        crate();
        OplType::Token func_name;
        OplType::AST *ret_type;
        vector<OplType::AST *> vars;
        vector<OplType::AST *> templates;
        OplType::AST *body;
        ++func_cnt;
        if (match_data("function"))
            advance();
        if (!match_type(OplType::TT_ID)) {
            cout << "want a name, but get " << current->data << endl;
            exit(-1);
        }
        func_name = *current;
        if (!func_name.data.empty())
            add(func_name.data, OplType::SK_FUNCTION);
        advance();
        if (match_data("<")) templates = make_templates();
        consume("(", __func__);
        while (current && !match_data(")")) {
            vars.push_back(make_var_define());
            if (match_data(")")) break;
            consume(",", __func__);
        }
        consume(")", __func__);
        if (!match_type(OplType::TT_ID)) {
            ERROR__:
            err_out(PARSE_TIME_ERROR, "Parse time error, want a return type at lin %d, col %d, pos %d", lin, col, pos);
        }
        ret_type = make_type_node();
        check_type(ret_type);
        body = make_block();
        leave();
        func_return_type_record[func_name.data] = make_id_type_node(ret_type);
        auto nf = new OplType::FunctionNode(func_name, new OplType::BlockNode(vars), body, ret_type, templates);
        nf->idt = func_return_type_record[func_name.data];
        return nf;
    }

    OplType::AST *Parser::make_if() {
        crate();
        if (match_data("if"))
            advance();
        OplType::AST *condition;
        OplType::AST *body;
        OplType::AST *else_node = nullptr;
        consume("(", __func__);
        condition = make_or_node();
        consume(")", __func__);
        body = make_block();
        if (match_data("else"))
            else_node = make_else();
        leave();
        return new OplType::IfNode(condition, body, else_node);
    }

    OplType::AST *Parser::make_else() {
        consume("else", __func__);
        return new OplType::ElseNode(make_block());
    }

    OplType::AST *Parser::make_assign(OplType::AST *id) {
        consume("=", __func__);
        auto data = make_expr();
        return new OplType::AssignNode(id, data);
    }

    OplType::AST *Parser::make_mem_malloc() {
        consume("new", __func__);
        OplType::AST *object_name = make_type_node();
        OplType::IdTypeNode itn = make_id_type_node(object_name);
        if (match_data("(") && !object_name->is_block) {
            auto a = new OplType::MemoryMallocNode(make_func_call(object_name));
            a->idt = itn;
            if (match_data("[")) {
                cout << "need ';'\n";
                exit(-1);
            }
            return a;
        }
        auto ret = new OplType::MemoryMallocNode(object_name);
        ret->idt = itn;
        return ret;
    }

    OplType::AST *Parser::make_class() {
        crate();
        OplType::Token name;
        add("this", OplType::SK_CLASS);
        int ofst = 0;
        OplType::AST *constructor = {};
        vector<OplType::AST *> v;
        vector<OplType::AST *> temps;
        if (match_data("class")) advance();
        if (!match_type(OplType::TT_ID)) err_out(PARSE_TIME_ERROR, "want a ID meet '%s', at lin %d, col %d, pos %d",
                                        current->data.c_str(), lin, col, pos);
        name = *current;
        OplType::IdTypeNode itn;
        itn.rootType = name.data;
        current_class = name.data;
        add_var("this", itn);
        ptcr[name.data] = OplType::ParseTimeClassRecord();
        add(name.data, OplType::SK_CLASS);
        vector<string> parent_map;
        class_cnt++;
        class_map[name.data] = class_cnt;
        class_map["this"] = class_cnt;
        advance();
        if (match_data("<"))
            temps = make_templates();
        if (match_data("(")) {
            advance();
            while (current && !match_data(")")) {
                auto t = want_get(OplType::TT_ID);
                parent_map.push_back(t.data);
                advance();
            }
            consume(")", __func__);
        }
        consume("{", __func__);
        while (current && !match_data("}")) {
            if (current->data == "constructor") {
                constructor = make_function_define();
            }
            OplType::AccessStatus as = OplType::PRIVATE_MEMBER;
            if (match_data("public")) {
                as = OplType::PUBLIC_MEMBER;
                advance();
            } else if (match_data("private")) {
                advance();
            } else if (match_data("protected")) {
                as = OplType::PROTECTED_MEMBER;
                advance();
            }
            if (match_data("let")) {
                auto a = make_var_define();
                auto vtype = a->idt;
                auto vname = a->data.data;
                ptcr[name.data].var_type_record[vname] = vtype;
                ptcr[name.data].var_access_status[vname] = as;
                scope[scope.size() - 1].var_type_record[vname] = vtype;
                ptcr["this"].var_type_record[vname] = vtype;
                v.push_back(a);
                consume(";", __func__);
            }
            if (match_data("function")) {
                auto a = make_function_define();
                ptcr[name.data].func_type_record[a->data.data] = a->idt;
                ptcr[name.data].func_access_status[a->data.data] = as;
                ptcr["this"].func_type_record[a->data.data] = a->idt;
                v.push_back(a);
            }
        }
        consume("}", __func__);
        leave();
        auto cls = new OplType::ClassNode(name, constructor, parent_map, temps, v);
        class_size_record[name.data] = get_class_size(cls);
        cls->class_size = class_size_record[name.data];
        class_map.erase("this");
        ptcr.erase("this");
        current_class = "";
        return cls;
    }

    void Parser::main_parse() {
        while (current) {
            if (match_data("class"))
                result.add(make_class());
            else if (match_data("function"))
                result.add(make_function_define());
            else if (match_data("using")) {
                result.add(make_import_node());
                consume(";", __func__);
            } else if (match_data("module")) {
                result.add(make_load_module_node());
                consume(";", __func__);
            } else if (match_data("let")) {
                result.add(make_var_define());
                consume(";", __func__);
            } else if (match_data("export")) {
                advance();
                if (match_data("function")) {
                    auto node = make_function_define();
                    (*export_map)[node->data.data] = OplType::FUNCTION_EXPORT;
                    result.add(node);
                } else if (match_data("let")) {
                    auto node = make_var_define();
                    string name = node->data.data;
                    (*export_map)[node->data.data] = OplType::VAR_EXPORT;
                    result.add(node);
                    consume(";", __func__);
                } else if (match_data("class")) {
                    auto node = make_class();
                    (*export_map)[node->data.data] = OplType::CLASS_EXPORT;
                    result.add(node);
                } else {
                    printf("Parse time error, at lin %d, col %d, pos %d, when export\n", lin, col, pos);
                    exit(-1);
                }
            }
        }
    }

    OplType::AST *Parser::make_return_node() {
        consume("return", __func__);
        if (match_data(";")) return new OplType::ReturnNode();
        auto node = make_expr();
        return new OplType::ReturnNode(node);
    }

    OplType::AST *Parser::make_for() {
        crate();
        OplType::AST *init, *end, *change, *body;
        if (match_data("for"))
            advance();
        consume("(", __func__);
        init = make_var_define();
        consume(";", __func__);
        end = make_expr();
        consume(";", __func__);
        change = make_expr();
        consume(")", __func__);
        body = make_block();
        leave();
        return new OplType::ForLoopNode(init, end, change, body);
    }

    OplType::AST *Parser::make_a_assign() {
        auto tmp = make_member_node();
        return make_assign(tmp);
    }

    OplType::AST *Parser::make_while() {
        crate();
        if (match_data("while"))
            advance();
        consume("(", __func__);
        OplType::AST *condition = nullptr;
        OplType::AST *body = nullptr;
        condition = make_expr();
        consume(")", __func__);
        body = make_block();
        leave();
        return new OplType::WhileLoopNode(condition, body);
    }

    OplType::AST *Parser::make_inc_node(OplType::AST *a) {
        return new OplType::IncNode(a);
    }

    OplType::AST *Parser::make_dec_node(OplType::AST *a) {
        return new OplType::DecNode(a);
    }

    void Parser::leave() {
        debug_out("Leave scope[%zu]", scope.size());
        if (scope.size() >= 2) {
            scope.pop_back();
        }
    }

    void Parser::crate() {
        scope.push_back(VarList());
        debug_out("Crate scope[%zu]", scope.size());
    }

    bool Parser::is_define(string s) {
        for (auto i: scope)
            if (i.is_have(s))
                return true;
        return false;
    }

    void Parser::add(std::string s, OplType::SymbolKind kd) {
        scope[scope.size() - 1].add(s, kd);
    }

    int Parser::make_class_id() {
        return ++class_cnt;
    }

    OplType::AST *Parser::make_self_add_node(OplType::AST *v) {
        if (match_data("+="))
            advance();
        auto expr_ = make_expr();
        return new OplType::SelfAdd(v, expr_);
    }

    OplType::AST *Parser::make_self_sub_node(OplType::AST *v) {
        if (match_data("-="))
            advance();
        auto expr_ = make_expr();
        return new OplType::SelfSub(v, expr_);
    }

    OplType::AST *Parser::make_self_div_node(OplType::AST *v) {
        if (match_data("/="))
            advance();
        auto expr_ = make_expr();
        return new OplType::SelfDiv(v, expr_);
    }

    OplType::AST *Parser::make_self_mul_node(OplType::AST *v) {
        if (match_data("*="))
            advance();
        auto expr_ = make_expr();
        return new OplType::SelfMul(v, expr_);
    }

    OplType::AST *Parser::make_case_node() {
        crate();
        consume("case", __func__);
        auto value = make_expr();
        consume(":", __func__);
        auto code = make_block();
        leave();
        return new OplType::CaseNode(value, code);
    }

    OplType::AST *Parser::make_default_node() {
        crate();
        consume("default", __func__);
        consume(":", __func__);
        auto code = make_block();
        leave();
        return new OplType::DefaultNode(code);
    }

    OplType::AST *Parser::make_switch_node() {
        vector<OplType::AST *> codes;
        consume("switch", __func__);
        consume("(", __func__);
        auto id = make_member_node();
        consume(")", __func__);
        consume("{", __func__);
        while (current && !match_data("}")) {
            if (match_data("case")) {
                codes.push_back(make_case_node());
            }
            if (match_data("default")) {
                codes.push_back(make_default_node());
            }
        }
        consume("}", __func__);
        return new OplType::SwitchNode(id, codes);
    }

    OplType::AST *Parser::make_continue() {
        if (match_data("continue")) advance();
        return new OplType::ContinueNode();
    }

    OplType::AST *Parser::make_break() {
        if (match_data("break")) advance();
        return new OplType::BreakNode();
    }

    OplType::AST *Parser::make_list() {
        consume("[", __func__);
        vector<OplType::AST *> values;
        while (current && !match_data("]")) {
            values.push_back(make_expr());
            if (match_data("]")) break;
            consume(",", __func__);
        }
        consume("]", __func__);
        return new OplType::ListNode(values);
    }

    OplType::AST *Parser::make_get_value_node(OplType::AST *id) {
        consume("[", __func__);
        auto expr = make_expr();
        if (match_data(",")) {
            err_out(PARSE_TIME_ERROR, "need one value", "");
        }
        consume("]", __func__);
        auto tmp = new OplType::GetValueAtPosition(id, expr);
        tmp->idt = derive_var_type(id);
        return tmp;
    }

    OplType::AST *Parser::make_constructor_call_() {
        auto id = make_type_node();
        check_type(id);
        return make_constructor_call(id);
    }

    OplType::AST *Parser::make_constructor_call(OplType::AST *name) {
        vector<OplType::AST *> args;
        if (match_data("(")) {
            advance();
            while (current && !match_data(")")) {
                args.push_back(make_expr());
                if (match_data(")")) break;
                consume(",", __func__);
            }
            consume(")", __func__);
        }
        return new OplType::ClassBuildNode(name, args);
    }

    OplType::AST *Parser::make_not_node() {
        consume("!", __func__);
        auto expr = make_expr();
        return new OplType::NotNode(expr);
    }

    OplType::AST *Parser::make_import_node() {
        if (match_data("using"))
            advance();
        if (!match_type(OplType::TT_STRING)) {
            printf("Parse time error, at lin %d, col %d, pos %d, want a string\n", lin, col, pos);
            exit(-1);
        }
        auto tok_tmp = *current;
        string path = current->data;
        string buffer, code;
        ifstream ifs(path);
        while (getline(ifs, buffer))
            code += buffer;
        OplLexer::Lexer lexer(code);
        Parser parser(lexer.tokens);
        auto v = parser.result.ast;
        advance();
        return new OplType::Import(tok_tmp, v);
    }

    OplType::AST *Parser::make_load_library() {
        consume("load_library", __func__);
        consume("(", __func__);
        OplType::Token gt = want_get(OplType::TT_STRING);
        advance();
        consume(")", __func__);
        return new OplType::LoadLibraryNode(gt);
    }

    OplType::Token Parser::want_get(string s) {
        if (match_type(s)) return *current;
        err_out(PARSE_TIME_ERROR, "want '%s' get '%s', at lin %d, col %d, pos %d", s.c_str(), current->data.c_str(),
                lin, col, pos);
        return OplType::Token();
    }

    OplType::AST *Parser::make_load_module_node() {
        if (match_data("module")) advance();
        auto name = make_member_node();
        string name_str = name->data.data;
        add(name_str, OplType::SK_MODULE);
        consume("=", __func__);
        auto mod = make_load_library();
        return new OplType::LoadModuleNode(name, mod);
    }

    int Parser::get_class_size(OplType::AST *a) {
        auto si = a->children.size();
        for (auto i: a->parents)
            si += class_size_record[i];
        class_size_record[a->data.data] = si;
        return si;
    }

    OplType::AST *Parser::make_lambda() {
        vector<OplType::AST *> value;
        OplType::AST *body;
        if (match_data("lambda"))
            advance();
        consume("(", __func__);
        while (current && !match_data(")")) {
            value.push_back(make_var_define());
            if (match_data(")")) break;
            consume(",", __func__);
        }
        consume(")", __func__);
        OplType::IdTypeNode idt = make_id_type_node(make_type_node());
        body = make_block();
        auto res = new OplType::LambdaNode(value, body);
        res->idt = idt;
        return res;
    }

    void Parser::check_type(OplType::AST *a) {
        string type = a->type;
        if (type != OplType::AST_CLASS && type != OplType::AST_TYPE && a->data.data != "this") err_out(PARSE_TIME_ERROR, "want a type",
                                                                                     0);
        if (type == OplType::AST_CLASS) {
            if (class_map[a->data.data] == 0) err_out(PARSE_TIME_ERROR, "class %s not exist", a->data.data.c_str());
            return;
        }
        if (type == OplType::AST_TYPE) {
            string root_type = a->data.data;
            if (class_map[root_type] == 0) err_out(PARSE_TIME_ERROR, "class '%s' not exist", root_type.c_str());
            if (!a->children.empty()) {
                for (auto i: a->children)
                    check_type(i);
            }
            return;
        }
        if (a->data.data == "list")
            return;
        err_out(PARSE_TIME_ERROR, "want a type", 0);
    }

    bool Parser::is_type(OplType::AST *a) {
        string type = a->type;
        if (type != OplType::AST_CLASS && type != OplType::AST_TYPE)
            return false;
        if (type == OplType::AST_CLASS) {
            if (class_map[a->data.data] == 0)
                return false;
            return true;
        }
        if (type == OplType::AST_TYPE) {
            string root_type = a->data.data;
            if (class_map[root_type] == 0)
                return false;
            if (!a->children.empty())
                for (auto i: a->children)
                    check_type(i);
            return true;
        }
        return false;
    }
}
#endif