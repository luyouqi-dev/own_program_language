#ifndef COMPILER
#include <vector>
#include <unordered_map>
#include "OplType.hpp"

namespace OplCompiler {

    struct BitCode {
        enum CodeType {
            CT_NUM, CT_LABEL
        } code_type;
        std::string label_name;
        int bit;
        BitCode();
        BitCode(int);
        BitCode(std::string);
    };

    BitCode::BitCode() {
        code_type = CT_NUM;
    }

    BitCode::BitCode(int dig) {
        bit = dig;
        code_type = CT_NUM;
    }

    BitCode::BitCode(std::string s) {
        label_name = s;
        code_type = CT_LABEL;
    }

    struct OperatorCommand {
        std::string label;
        std::vector<BitCode> codes;
        OperatorCommand(std::string, std::vector<BitCode>);
    };

    OperatorCommand::OperatorCommand(std::string label, std::vector<BitCode> codes) {
        this->label = label;
        this->codes = codes;
    }

    struct Business {
        std::unordered_map<std::string, int> label_map;
        std::vector<OperatorCommand> codes;
        void append(OperatorCommand);
        void back_fill();
    };

    void Business::back_fill() {
        for (auto & code : codes)
            for (auto & j : code.codes)
                if (j.code_type == BitCode::CT_LABEL)
                    j.bit = label_map[j.label_name];
    }

    void Business::append(OperatorCommand opera) {
        int s = 0;
        for (auto i : codes) s += i.codes.size();
        label_map[opera.label] = s;
        codes.push_back(opera);
    }

    class Compiler {
    public:
        Compiler(std::vector<OplType::AST*>);
    private:
        std::vector<OplType::AST*> asts;
        void visit_bin_op_node(OplType::AST*);
    };

    void Compiler::visit_bin_op_node(OplType::AST* a) {
        
    }

    Compiler::Compiler(std::vector<OplType::AST *>) {

    }
}

#endif