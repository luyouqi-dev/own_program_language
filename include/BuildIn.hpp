//
// Created by User on 2025/8/21.
//

#ifndef PROGRAMLANGUAGEINCPP_BUILDIN_HPP
#define PROGRAMLANGUAGEINCPP_BUILDIN_HPP
#include <string>
#include <vector>
#include "OplType.hpp"
#include <unordered_map>

namespace OplBuildIn {

    enum Assembly {
        _add = 0x0a,
        _sub = 0x0b,
        _mul = 0x0c,
        _div = 0x0d,
        _mod = 0x0f,
        _pow = 0x0e,
        _xor = 0xef,
        _left_mv = 0x98f3e,
        _right_mv = 0x911fe,

        _dup = 0x0909090,
        _psh = 0xee, // 1
        _pop = 0xefef,
        _a_psh = 0xae, // 1
        _a_pop = 0xaf,
        _imm = 0xaaff, // 1

        _i_load = 0x123, // 1
        _i_stor = 0x1123, // 2
        _param = 0xfe567,
        _sparam = 0xf0f57,
        _hsst = 0x98fffe,
        _new = 0xeabcd,
        _sdup = 0x9ffffe3,
        _new_array = 0x9e76f,
        _hp_stor = 0x999fac,

        _or = 0x13ef,
        _and = 0x345ef,
        _not = 0xeeeeff,
        _cbg = 0xcccc, // comp bigger
        _cls = 0xcef,  // comp less
        _ceq = 0xefef1,// comp eq
        _cne = 0xeef44,// comp not eq
        _cebg = 0xccccef, // comp bigger or eq
        _cels = 0xcefef,  // comp less or eq

        _jt = 0x1ffee, // 1
        _jmp = 0x4f4fe3, // 1

        _call = 0x88feabc, // 1
        _scall = 0x98af,
        _ret = 0x8899fe,
        _lea = 0x8844fed,

        _nop = 0x0000
    };

    std::unordered_map<int, int> asm_value_size = {
            {_add,       0},
            {_sub,       0},
            {_nop,       0},
            {_cels,      0},
            {_mul,       0},
            {_div,       0},
            {_mod,       0},
            {_pow,       0},
            {_xor,       0},
            {_left_mv,   0},
            {_right_mv,  0},
            {_dup,       0},
            {_psh,       1},
            {_sparam,    0},
            {_pop,       0},
            {_new_array, 1},
            {_a_psh,     0},
            {_a_pop,     0},
            {_imm,       0},
            {_i_load,    0},
            {_i_stor,    0},
            {_param,     1},
            {_hsst,      0},
            {_new,       0},
            {_sdup,      0},
            {_or,        0},
            {_and,       0},
            {_not,       0},
            {_cbg,       0},
            {_cls,       0},
            {_ceq,       0},
            {_cne,       0},
            {_jt,        1},
            {_jmp,       1},
            {_call,      1},
            {_scall,     0},
            {_ret,       0},
            {_lea,       0},
    };

    std::unordered_map<int, std::string> asm_string_map = {
            {_add,       "_add"},
            {_sub,       "_sub"},
            {_nop,       "_nop"},
            {_cels,      "_cels"},
            {_mul,       "_mul"},
            {_div,       "_div"},
            {_new_array, "_new_array"},
            {_mod,       "_mod"},
            {_pow,       "_pow"},
            {_xor,       "_xor"},
            {_left_mv,   "_left_mv"},
            {_right_mv,  "_right_mv"},
            {_dup,       "_dup"},
            {_psh,       "_psh"},
            {_pop,       "_pop"},
            {_a_psh,     "_a_psh"},
            {_a_pop,     "_a_pop"},
            {_imm,       "_imm"},
            {_i_load,    "_i_load"},
            {_i_stor,    "_i_stor"},
            {_param,     "_param"},
            {_hsst,      "_hsst"},
            {_new,       "_new"},
            {_sdup,      "_sdup"},
            {_or,        "_or"},
            {_and,       "_and"},
            {_not,       "_not"},
            {_cbg,       "_cbg"},
            {_cls,       "_cls"},
            {_sparam,    "_sparam"},
            {_ceq,       "_ceq"},
            {_cne,       "_cne"},
            {_jt,        "_jt"},
            {_jmp,       "_jmp"},
            {_call,      "_call"},
            {_scall,     "_scall"},
            {_ret,       "_ret"},
            {_lea,       "_lea"},
    };

    inline bool is_asm(int x) { return asm_value_size.find(x) != asm_value_size.end(); }

    void format_assembly(std::vector<int> asm_, std::string tabs = "") {
        int i = 0;
        while (i < asm_.size()) {
            int c = asm_[i++];
            if (!is_asm(c)) {
                printf("%sWarn: byte code 0x%x not a operator command\n", tabs.c_str(), c);
                continue;
            }
            printf("%s%s     ", tabs.c_str(), asm_string_map[c].c_str());
            for (int j = 0; j < asm_value_size[c]; ++j)
                printf("%d     ", asm_[i++]);
            printf("\n");
        }
    };

    enum Opera {
        SET_MAIN = 0x67fAAbC, SET_CLASS = 0x981, SET_FUNC = 0x8fd401
    };

    enum FunctionType {
        FT_USER_DEFINE, FT_LIB, FT_BUILD_IN
    };

    std::vector<std::string> build_in_function = {
            "print",
            "input",
            "to_string",
            "to_int",
            "len"
    };

    std::vector<std::string> build_in_class = {
            "int",
            "double",
            "bool",
            "string",
            "void",
            "char"
    };

    enum BuildInFunction {
        PRINT_ = 0XEF4,
        INPUT_ = 0X1AC,
        TO_STRING_ = 0XACE,
        TO_INTEGER_ = 0XACEF4,
        ARR_LEN = 0x98bf3,
        ARR_GET = 0x998,
        ARR_SET = 0x98fea
    };

    bool is_build_in(int a) {
        if (a == PRINT_ ||
            a == INPUT_ ||
            a == TO_STRING_ ||
            a == TO_INTEGER_ ||
            a == ARR_LEN)
            return true;
        return false;
    }

    enum BuildInClass {
        BS_INT = 0xcfed1,
        BS_STR = 0xcfed2,
        BS_CHAR = 0xcfed3,
        BS_DOUBLE = 0xcfed4,
        BS_BOOL = 0xcfed5,
        BS_VOID = 0XCFED903
    };

    std::unordered_map<std::string, std::vector<int>> build_in_id_map = {
            {"print",     {PRINT_,      0}},
            {"input",     {INPUT_,      1}},
            {"to_string", {TO_STRING_,  1}},
            {"to_int",    {TO_INTEGER_, 1}},
            {"len",       {ARR_LEN,     1}},
    };

    std::unordered_map<std::string, std::vector<int>> build_in_cls_map = {
            {"int",    {BS_INT,    0xf34}},
            {"double", {BS_DOUBLE, 0xdf4}},
            {"bool",   {BS_BOOL,   0x996f}},
            {"string", {BS_STR,    0x9873}},
            {"void",   {BS_VOID,   0xafe}},
            {"char",   {BS_CHAR,   0x0f0}},
    };


    std::unordered_map<std::string, std::vector<int>> vm_build_in_fn_map = {
            {"print",     {PRINT_,      1, _lea}},
            {"input",     {INPUT_,      1, _ret}},
            {"to_string", {TO_STRING_,  1, _ret}},
            {"to_int",    {TO_INTEGER_, 1, _ret}},
            {"len",       {ARR_LEN,     1, _ret}}
    };

    std::unordered_map<std::string, OplType::IdTypeNode> vm_build_in_fn_return_type_map = {
            {"print",     OplType::IdTypeNode("void")},
            {"input",     OplType::IdTypeNode("string")},
            {"to_string", OplType::IdTypeNode("string")},
            {"to_int",    OplType::IdTypeNode("int")},
            {"len",       OplType::IdTypeNode("int")}
    };

    std::unordered_map<std::string, std::vector<int>> vm_build_in_class = {
            {"int",    {BS_INT,    1}},
            {"string", {BS_STR,    1}},
            {"char",   {BS_CHAR,   1}},
            {"double", {BS_DOUBLE, 1}},
            {"bool",   {BS_BOOL,   1}}
    };

    struct Object {
        int id, size;
        std::vector<int> parent_id;
        std::map<int, int> member_map;
    };

    struct Function {
        std::vector<int> codes;
        int id;
        int value_list_size;
        FunctionType type = FT_USER_DEFINE;
    };

}

#endif //PROGRAMLANGUAGEINCPP_BUILDIN_HPP
