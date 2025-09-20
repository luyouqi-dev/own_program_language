#ifndef OPP
#define OPP
#include <vector>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <map>

#define IS_WORD(x) (('a' <= (x) && (x) <= 'z') || ('A' <= (x) && (x) <= 'Z') || (x) == '_')

namespace OplType {

    class AST;

    class IdTypeNode {
    public:
        bool is_block = false;
        AST *block_size = nullptr;
        std::string rootType;
        std::vector<IdTypeNode> templateType;

        IdTypeNode(std::string);

        IdTypeNode(std::string, std::string);

        IdTypeNode(bool, AST *);

        IdTypeNode();
    };

    IdTypeNode::IdTypeNode(std::string root_type) {
        this->rootType = root_type;
    }

    IdTypeNode::IdTypeNode(bool is_block, AST *block_size) {
        this->is_block = is_block;
        this->block_size = block_size;
    }

    IdTypeNode::IdTypeNode() {
        this->is_block = false;
    }

    IdTypeNode::IdTypeNode(std::string root_type, std::string templ_type) {
        this->is_block = true;
        root_type = root_type;
        templateType.push_back(templ_type);
    }


    void prd(int dep) {
        while (dep--) std::cout << "   ";
    }

    void print_type_node(const IdTypeNode &itn, int dep = 1) {
        prd(dep - 1);
        std::cout << "ROOT_TYPE = " << itn.rootType << " \n";
        if (!itn.templateType.empty()) {
            for (auto i: itn.templateType)
                print_type_node(i, dep + 1);
        }
    }

    std::string hex_to_dec(std::string hexStr) {
        unsigned long long decimal = 0;
        for (char c: hexStr) {
            int val;
            if (c >= '0' && c <= '9') val = c - '0';
            else if (c >= 'A' && c <= 'F') val = 10 + (c - 'A');
            else if (c >= 'a' && c <= 'f') val = 10 + (c - 'a');
            else {
                std::cerr << "Error in hex to dec " << c << std::endl;
                return "0";
            }
            decimal = decimal * 16 + val;
        }
        return std::to_string(decimal);
    }

    std::string bin_to_dec(std::string x) {
        unsigned long long num = 0;
        for (char c: x) {
            num = num * 2 + (c - '0');
        }
        return std::to_string(num);
    }

    std::string oct_to_dec(std::string x) {
        unsigned long long num = 0;
        for (char c: x) {
            num = num * 8 + (c - '0');
        }
        return std::to_string(num);
    }

    enum SymbolKind {
        SK_FUNCTION,
        SK_CLASS,
        SK_VAR,
        SK_MODULE,
        SK_LIST
    };

    const std::string TT_INTEGER = "TT_INTEGER";
    const std::string TT_DOUBLE = "TT_DOUBLE";
    const std::string TT_NEW_LINE = "TT_NEW_LINE";
    const std::string TT_CHAR = "TT_CHAR";
    const std::string TT_STRING = "TT_STRING";
    const std::string TT_BOOL = "TT_BOOL";
    const std::string TT_MOD = "TT_MOD";
    const std::string TT_SELF_MOD = "TT_MOD";
    const std::string TT_NOT = "TT_NOT";
    const std::string TT_NOT_EQ = "TT_NOT_EQ";
    const std::string TT_ID = "TT_ID";
    const std::string TT_LBRACKET_S = "TT_LBRACKET_S";
    const std::string TT_KEY = "TT_KEY";
    const std::string TT_PLUS = "TT_PLUS";
    const std::string TT_SUB = "TT_SUB";
    const std::string TT_MUL = "TT_MUL";
    const std::string TT_DIV = "TT_DIV";
    const std::string AST_LAMBDA_CALL = "AST_LAMBDA_CALL";
    const std::string TT_POW = "TT_POW";
    const std::string TT_RBRACKET_S = "TT_RBRACKET_S";
    const std::string TT_LBRACKET_M = "TT_LBRACKET_M";
    const std::string TT_RBRACKET_M = "TT_RBRACKET_M";
    const std::string TT_LBRACKET_B = "TT_LBRACKET_B";
    const std::string TT_RBRACKET_B = "TT_RBRACKET_B";
    const std::string TT_PERIOD = "TT_PERIOD";
    const std::string TT_SEMICOLON = "TT_SEMICOLON";
    const std::string TT_EQ = "TT_EQ";
    const std::string TT_BIGER = "TT_BIGER";
    const std::string TT_SMALLER = "TT_SMALLER";
    const std::string TT_BRE = "TT_BRE";
    const std::string TT_SRE = "TT_SRE";
    const std::string TT_LM = "TT_LM";
    const std::string TT_AND = "TT_AND";
    const std::string TT_BIT_AND = "TT_BIT_AND";
    const std::string TT_OR = "TT_OR";
    const std::string TT_LEFT_MOVE = "TT_LEFT_MOVE";
    const std::string TT_RIGHT_MOVE = "TT_RIGHT_MOVE";
    const std::string TT_BIT_OR = "TT_BIT_OR";
    const std::string TT_RM = "TT_RM";
    const std::string TT_SLM = "TT_SLM";
    const std::string TT_SRM = "TT_SRM";
    const std::string TT_COLON = "TT_COLON";
    const std::string TT_COMMA = "TT_COMMA";
    const std::string TT_ASSIGN = "TT_ASSIGN";
    const std::string TT_SELFADD = "TT_SELFADD";
    const std::string TT_SELFSUB = "TT_SELFSUB";
    const std::string TT_SELFMUL = "TT_SELFMUL";
    const std::string TT_SELFDIV = "TT_SELFDIV";

    class Token {
    public:
        std::string type;
        std::string data;
        void debug();
    };

    void Token::debug() {
        std::cout << type << " : '" << data << "'" << std::endl;
    }

    enum AccessStatus {
        PUBLIC_MEMBER, PRIVATE_MEMBER, PROTECTED_MEMBER
    };

    const std::string AST_MEMBER = "MEMBER";
    const std::string AST_ID = "ID";
    const std::string AST_BIN_OP = "BIN_OP";
    const std::string AST_DIGIT = "DIGIT";
    const std::string AST_STRING = "STRING";
    const std::string AST_VAR_DEF = "VAR_DEFINE";
    const std::string AST_FUNC_CALL = "FUNC_CALL";
    const std::string AST_FUNCTION_DEFINE = "FUNC_DEFINE";
    const std::string AST_BLOCK = "BLOCK";
    const std::string AST_IF = "IF";
    const std::string AST_ELSE = "ELSE";
    const std::string AST_ASSIGN = "ASSIGN";
    const std::string AST_MEM_MALLOC = "MEM_MALLOC";
    const std::string AST_CLASS = "CLASS_NODE";
    const std::string AST_RETURN = "RETURN";
    const std::string AST_FOR_LOOP = "FOR_LOOP";
    const std::string AST_WHILE_LOOP = "WHILE_LOOP";
    const std::string AST_CHAR = "AST_CHAR";
    const std::string AST_INC = "SELF_INC";
    const std::string AST_CONV = "TYPE_CONVERT";
    const std::string AST_DEC = "SELF_DEC";
    const std::string AST_SELF_ADD = "SELF_ADD";
    const std::string AST_SELF_SUB = "SELF_SUB";
    const std::string AST_SELF_DIV = "SELF_DIV";
    const std::string AST_SELF_MUL = "SELF_MUL";
    const std::string AST_BREAK = "AST_BREAK";
    const std::string AST_LAMBDA = "AST_LAMBDA";
    const std::string AST_CONTINUE = "AST_CONTINUE";
    const std::string AST_LIST = "AST_LIST";
    const std::string AST_GET_VALUE = "AST_GET_VALUE";
    const std::string AST_TYPE = "TYPE_NODE";
    const std::string AST_CONSTRUCTOR_CALL = "CONSTRUCTOR_CALL";
    const std::string AST_NOT = "NOT";
    const std::string AST_IMPORT = "IMPORT";
    const std::string AST_LOAD_LIB = "LOAD_LIB";
    const std::string AST_MODULE_LOAD = "MODULE_LOAD";
    const std::string AST_SWITCH = "AST_SWITCH";
    const std::string AST_CASE = "AST_CASE";

    const std::string AST_DEFAULT = "AST_DEFAULT";

    enum SymbolId {
        VAR_ID,
        FUNC_ID,
        CLASS_ID
    };

    enum ExportId {
        FUNCTION_EXPORT,
        CLASS_EXPORT,
        VAR_EXPORT
    };

    class ParseTimeClassRecord {
    public:
        std::map<std::string, IdTypeNode> var_type_record;
        std::map<std::string, AccessStatus> var_access_status;
        std::map<std::string, IdTypeNode> func_type_record;
        std::map<std::string, AccessStatus> func_access_status;

        std::string get_child_type(std::string, int);

        std::string get_root_type(std::string);

        IdTypeNode get_type(std::string);
    };

    IdTypeNode ParseTimeClassRecord::get_type(std::string n) {
        return var_type_record[n];
    }

    std::string ParseTimeClassRecord::get_root_type(std::string s) {
        if (var_type_record.find(s) == var_type_record.end()) {
            return {};
        }
        std::string tmp = var_type_record[s].rootType;
        return tmp;
    }

    std::string ParseTimeClassRecord::get_child_type(std::string s, int i) {
        if (var_type_record.find(s) == var_type_record.end()) {
            return {};
        }
        return var_type_record[s].templateType[i].rootType;
    }

    class AST {
    public:
        AST() {
            offset = 0;
            id_node = nullptr;
            as = PUBLIC_MEMBER;
            class_size = 0;
        }

        IdTypeNode idt;
        int class_size, offset;
        IdTypeNode parent_name;
        bool is_end = false;
        bool is_block = false;
        AST *block_size = nullptr;
        AccessStatus as;
        std::vector<AST *> children;
        std::vector<std::string> parents;
        std::vector<std::vector<AST *>> v_ast;
        AST *id_node;
        Token data;
        std::string type;
    };

    class DefaultNode : public AST {
    public:
        DefaultNode(AST *a) {
            children.push_back(a);
            type = AST_DEFAULT;
        }
    };

    class CaseNode : public AST {
    public:
        CaseNode(AST *value, AST *code) {
            type = AST_CASE;
            children.push_back(value);
            children.push_back(code);
        }
    };

    class SwitchNode : public AST {
    public:
        SwitchNode(AST *name, std::vector<AST *> a) {
            type = AST_SWITCH;
            children.push_back(name);
            for (auto i: a)
                children.push_back(i);
        }
    };

    class GetValueAtPosition : public AST {
    public:
        GetValueAtPosition(AST *list_name, AST *expr) {
            type = AST_GET_VALUE;
            children.push_back(list_name);
            children.push_back(expr);
        }
    };

    class Program {
    public:
        Program() {}

        AST *&operator[](int);

        int size();

        void add(AST *);

        std::vector<AST *> ast;
        std::unordered_map<std::string, ExportId> export_map;
        std::unordered_map<std::string, Program *> import_map;
    };

    AST *&Program::operator[](int pos) { return ast[pos]; }

    int Program::size() { return ast.size(); }

    void Program::add(AST *a) { ast.push_back(a); }

    class BlockNode : public AST {
    public:
        BlockNode(std::vector<AST *> v) {
            type = AST_BLOCK;
            children = v;
        }
    };

    class IncNode : public AST {
    public:
        IncNode(AST *name) {
            type = AST_INC;
            children.push_back(name);
        }
    };

    class NotNode : public AST {
    public:
        NotNode(AST *condition) {
            children.push_back(condition);
            type = AST_NOT;
        }
    };

    class DecNode : public AST {
    public:
        DecNode(AST *name) {
            type = AST_DEC;
            children.push_back(name);
        }
    };

    class LambdaCallNode : public AST {
    public:
        LambdaCallNode(AST *name, std::vector<AST *> value_list) {
            this->id_node = name;
            this->children = value_list;
            type = AST_LAMBDA_CALL;
        }
    };

    class SelfAdd : public AST {
    public:
        SelfAdd(AST *var, AST *value) {
            type = AST_SELF_ADD;
            children.push_back(var);
            children.push_back(value);
        }
    };

    class SelfSub : public AST {
    public:
        SelfSub(AST *var, AST *value) {
            type = AST_SELF_SUB;
            children.push_back(var);
            children.push_back(value);
        }
    };

    class Import : public AST {
    public:
        Import(Token file_path, std::vector<AST *> file_data) {
            type = AST_IMPORT;
            data = file_path;
            children = file_data;
        }
    };

    class SelfMul : public AST {
    public:
        SelfMul(AST *var, AST *value) {
            type = AST_SELF_MUL;
            children.push_back(var);
            children.push_back(value);
        }
    };

    class TypeNode : public AST {
    public:
        TypeNode(Token name, std::vector<AST *> template_list, bool is_block = false, AST *block_size = nullptr) {
            type = AST_TYPE;
            data = name;
            children = template_list;
            this->is_block = is_block;
            this->block_size = block_size;
        }

        TypeNode(Token name, bool is_block = false, AST *block_size = nullptr) {
            type = AST_TYPE;
            data = name;
            this->is_block = is_block;
            this->block_size = block_size;
        }

        TypeNode() {

        }
    };

    IdTypeNode make_id_type_node(AST *node) {
        auto a = node;
        if (node->data.data == "lambda") {
            IdTypeNode it;
            it.rootType = "lambda";
            IdTypeNode itt;
            if (node->children.empty()) {
                std::cout << "no return type\n";
                exit(-1);
            }
            for (int i = 0; i < node->children.size() - 1; ++i)
                itt.templateType.push_back(make_id_type_node(node->children[i]));
            it.templateType.push_back(itt);
            it.templateType.push_back(make_id_type_node(node->children[node->children.size() - 1]));
            return it;
        }
        IdTypeNode itn;
        itn.rootType = node->data.data;
        itn.is_block = node->is_block;
        itn.block_size = node->block_size;
        if (!node->children.empty())
            for (auto i: node->children)
                itn.templateType.push_back(make_id_type_node(i));
        return itn;
    }

    class SelfDiv : public AST {
    public:
        SelfDiv(AST *var, AST *value) {
            type = AST_SELF_DIV;
            children.push_back(var);
            children.push_back(value);
        }
    };

    class ClassBuildNode : public AST {
    public:
        ClassBuildNode(AST *name, const std::vector<AST *> &args) {
            type = AST_CONSTRUCTOR_CALL;
            children.push_back(name);
            for (auto i: args)
                children.push_back(i);
        }
    };

    class ReturnNode : public AST {
    public:
        ReturnNode(AST *data = nullptr) {
            type = AST_RETURN;
            children.push_back(data);
        }
    };

    class ClassNode : public AST {
    public:
        ClassNode(Token class_name, AST *constructor, std::vector<AST *> v) {
            type = AST_CLASS;
            data = class_name;
            id_node = constructor;
            children = v;
        }

        ClassNode(Token class_name, AST *constructor, std::vector<std::string> parents, std::vector<AST *> templates,
                  std::vector<AST *> member) {
            type = AST_CLASS;
            data = class_name;
            id_node = constructor;
            v_ast.push_back(templates);
            children = member;
            this->parents = parents;
        }

        ClassNode(Token class_name, AST *constructor, std::vector<AST *> templates, std::vector<AST *> member) {
            type = AST_CLASS;
            data = class_name;
            id_node = constructor;
            v_ast.push_back(templates);
            children = member;
        }
    };

    class IfNode : public AST {
    public:
        IfNode(AST *condition, AST *body, AST *else_node = nullptr) {
            type = AST_IF;
            children.push_back(condition);
            children.push_back(body);
            children.push_back(else_node);
        }
    };

    class ListNode : public AST {
    public:
        ListNode(std::vector<AST *> value) {
            children = value;
            type = AST_LIST;
        }
    };

    class MemoryMallocNode : public AST {
    public:
        MemoryMallocNode(AST *a) {
            type = AST_MEM_MALLOC;
            children.push_back(a);
        }
    };

    class ElseNode : public AST {
    public:
        ElseNode(AST *a) {
            type = AST_ELSE;
            children.push_back(a);
        }
    };

    class AssignNode : public AST {
    public:
        AssignNode(AST *id, AST *value) {
            type = AST_ASSIGN;
            children.push_back(id);
            children.push_back(value);
        }
    };

    class FunctionNode : public AST {
    public:
        FunctionNode(Token func_name, AST *value_list, AST *body, AST *return_type, AccessStatus as = PUBLIC_MEMBER) {
            type = AST_FUNCTION_DEFINE;
            data = func_name;
            this->as = as;
            children.push_back(value_list);
            children.push_back(body);
            children.push_back(return_type);
        }

        FunctionNode(Token func_name, AST *value_list, AST *body, AST *return_type, const std::vector<AST *> &templates,
                     AccessStatus as = PUBLIC_MEMBER) {
            type = AST_FUNCTION_DEFINE;
            data = func_name;
            this->as = as;
            children.push_back(value_list);
            children.push_back(body);
            children.push_back(return_type);
            v_ast.push_back(templates);
        }
    };

    class CastNode : public AST {
    public:
        CastNode(AST *_type, AST *expression) {
            type = AST_CONV;
            children.push_back(_type);
            children.push_back(expression);
        }
    };

    class VarDefineNode : public AST {
    public:
        VarDefineNode(Token var_name, AST *var_type = nullptr, AST *var_data = nullptr) {
            type = AST_VAR_DEF;
            data = var_name;
            children.push_back(var_type);
            children.push_back(var_data);
        }
    };

    class IdNode : public AST {
    public:
        IdNode(Token tok) {
            type = AST_ID;
            data = tok;
        }

        IdNode(Token tok, std::vector<AST *> t) {
            type = AST_ID;
            data = tok;
            children = t;
        }
    };

    class LambdaNode : public AST {
    public:
        LambdaNode(std::vector<AST *> value, AST *code) {
            v_ast.push_back(value);
            children.push_back(code);
            type = AST_LAMBDA;
        }
    };

    class DigitNode : public AST {
    public:
        DigitNode(Token num) {
            type = AST_DIGIT;
            data = num;
        }
    };

    class StringNode : public AST {
    public:
        StringNode(Token str) {
            type = AST_STRING;
            data = str;
        }
    };

    class CharNode : public AST {
    public:
        CharNode(Token chr) {
            type = AST_CHAR;
            data = chr;
        }
    };

    class FunctionCallNode : public AST {
    public:
        FunctionCallNode(AST *func_name, std::vector<AST *> func_value_list) {
            children.push_back(func_name);
            for (auto i: func_value_list)
                children.push_back(i);
            type = AST_FUNC_CALL;
        }

        FunctionCallNode(AST *func_name, std::vector<AST *> func_value_list, std::vector<AST *> templates) {
            children.push_back(func_name);
            for (auto i: func_value_list)
                children.push_back(i);
            type = AST_FUNC_CALL;
            v_ast.push_back(templates);
        }
    };

    class ForLoopNode : public AST {
    public:
        ForLoopNode(AST *init, AST *end, AST *change, AST *body) {
            type = AST_FOR_LOOP;
            children.push_back(init);
            children.push_back(end);
            children.push_back(change);
            children.push_back(body);
        }
    };

    class ContinueNode : public AST {
    public:
        ContinueNode() { type = AST_CONTINUE; }
    };

    class BreakNode : public AST {
    public:
        BreakNode() { type = AST_BREAK; }
    };

    class WhileLoopNode : public AST {
    public:
        WhileLoopNode(AST *condition, AST *body) {
            type = AST_WHILE_LOOP;
            children.push_back(condition);
            children.push_back(body);
        }
    };

    class LoadModuleNode : public AST {
    public:
        LoadModuleNode(AST *id, AST *load_node) {
            type = AST_MODULE_LOAD;
            children.push_back(id);
            children.push_back(load_node);
        }
    };

    class LoadLibraryNode : public AST {
    public:
        LoadLibraryNode(Token id) {
            type = AST_LOAD_LIB;
            data = id;
        }
    };

    class MemberNode : public AST {
    public:
        MemberNode(AST *base, AST *member) {
            type = AST_MEMBER;
            children.push_back(base); // children[0]
            children.push_back(member); // children[1]
        }
    };

    class BinOpNode : public AST {
    public:
        BinOpNode(Token op, AST *left, AST *right) {
            data = op;
            type = AST_BIN_OP;
            children.push_back(left);
            children.push_back(right);
        }
    };

    void pd(int dp) { while (dp--) printf("  "); }

    void pt(AST *a) {
        std::cout << "<" << a->type << ": '" << a->data.data << "'> ";
    }

#define COLOR_RESET  std::string("\033[0m")
#define COLOR_GRAY   std::string("\033[90m")
#define COLOR_RED    std::string("\033[31m")
#define COLOR_GREEN  std::string("\033[32m")
#define COLOR_YELLOW std::string("\033[33m")
#define COLOR_BLUE   std::string("\033[34m")

    void print_tree(AST *a, int depth = 0, bool is_last = true, std::vector<bool> indents = {}) {
        if (!a) return;
        std::string prefix;
        for (size_t i = 0; i < indents.size(); ++i)
            prefix += "|   ";
        prefix += is_last ? "+---" : "|---";
        std::string type_color = a->type == AST_MEMBER ? COLOR_GREEN : COLOR_BLUE;
        std::string node_info = type_color + "<" + a->type + COLOR_RESET;
        if (!a->parent_name.rootType.empty())
            node_info +=
                    COLOR_GRAY + " parent:" + COLOR_RESET + "'" + COLOR_YELLOW + a->parent_name.rootType + COLOR_RESET +
                    "'";
        if (!a->data.data.empty())
            node_info += COLOR_GRAY + " data:" + COLOR_RESET + "'" + COLOR_YELLOW + a->data.data + COLOR_RESET + "'";
        if (a->offset != 0 || a->class_size != 0) {
            node_info += COLOR_GRAY + " [" + COLOR_RESET;
            if (a->offset != 0) node_info += COLOR_RED + "offset=" + std::to_string(a->offset) + COLOR_RESET;
            if (a->class_size != 0)
                node_info += (a->offset != 0 ? " " : "") + COLOR_RED + "size=" + std::to_string(a->class_size) + COLOR_RESET;
            node_info += COLOR_GRAY + "]" + COLOR_RESET;
        }
        node_info += ">";
        std::cout << COLOR_GRAY << prefix << COLOR_RESET << node_info << std::endl;
        std::vector<bool> new_indents(indents);
        new_indents.push_back(!is_last);
        if (a->type == AST_MEMBER) {
            std::cout << COLOR_GRAY << prefix << "    |--- [Base]" << COLOR_RESET << std::endl;
            print_tree(a->children[0], depth + 1, false, new_indents);
            std::cout << COLOR_GRAY << prefix << "    +--- [Member]" << COLOR_RESET << std::endl;
            print_tree(a->children[1], depth + 1, true, new_indents);
            return;
        }
        for (size_t i = 0; i < a->children.size(); ++i) {
            bool last_child = (i == a->children.size() - 1);
            print_tree(a->children[i], depth + 1, last_child, new_indents);
        }
        if (!a->v_ast.empty()) {
            std::string tmpl_prefix = prefix + (new_indents.empty() ? "" : "    ");
            std::cout << COLOR_GRAY << tmpl_prefix << "+--- <Templates>" << COLOR_RESET << std::endl;
            std::vector<bool> tmpl_indents(new_indents);
            tmpl_indents.push_back(false);
            for (auto &tlist: a->v_ast) {
                for (size_t i = 0; i < tlist.size(); ++i) {
                    print_tree(tlist[i], depth + 2, i == tlist.size() - 1, tmpl_indents);
                }
            }
        }
    }

    std::vector<std::string> keys = {
            "if", "else", "for", "while", "class",
            "function", "let", "const", "new",
            "public", "private", "protected", "return",
            "continue", "break", "using", "module",
            "switch", "case", "default", "export",
            "lambda"
    };
}

#endif 