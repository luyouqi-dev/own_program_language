//
// Created by User on 2025/9/19.
//

#ifndef PLINC_PROGRAM_HPP
#define PLINC_PROGRAM_HPP
#include "BuildIn.hpp"
#include "OplType.hpp"

namespace OplInterpreter {

    enum ValueKind {
        VK_CLASS, VK_FUNC, VK_VAR, VK_VALUE, VK_STRING, VK_DIGIT, VK_CHAR, VK_BOOL, VK_ARRAY, VK_RTR
    };

    class Value {
    public:
        Value();
        Value(OplType::AST*);
        Value(ValueKind, OplType::AST*);
        ValueKind vk;
        OplType::AST* value;
        virtual Value* __add__(Value*);
        virtual Value* __sub__(Value*);
        virtual Value* __div__(Value*);
        virtual Value* __mul__(Value*);
        virtual Value* __lmv__(Value*);
        virtual Value* __rmv__(Value*);
        virtual Value* __mod__(Value*);
        virtual Value* __or__(Value*);
        virtual Value* __and__(Value*);

        virtual Value* __greater_than__(Value*);
        virtual Value* __less_than__(Value*);
        virtual Value* __eq__(Value*);
        virtual Value* __not_eq__(Value*);
        virtual Value* __not__();
        virtual Value* __bg_or_eq__(Value*);
        virtual Value* __ls_or_eq__(Value*);

        virtual Value* __get_val__(Value*);
        virtual Value* get_member(std::string);
        virtual void  __set__(Value*);
    };

    Value* Value::__bg_or_eq__(Value*) {
        printf("Value '%d' not suppose operator symbol '>='\n", vk);
        exit(-1);
    }

    Value* Value::__ls_or_eq__(Value*) {
        printf("Value '%d' not suppose operator symbol '<='\n", vk);
        exit(-1);
    }

    Value* Value::__not_eq__(Value*) {
        printf("Value '%d' not suppose operator symbol '!='\n", vk);
        exit(-1);
    }

    Value* Value::__not__() {
        printf("Value '%d' not suppose operator symbol '!'\n", vk);
        exit(-1);
    }

    Value* Value::__greater_than__(Value*) {
        printf("Value '%d' not suppose operator symbol '>'\n", vk);
        exit(-1);
    }

    Value* Value::__less_than__(Value*) {
        printf("Value '%d' not suppose operator symbol '<'\n", vk);
        exit(-1);
    }

    Value* Value::__eq__(Value*) {
        printf("Value '%d' not suppose operator symbol '=='\n", vk);
        exit(-1);
    }

    Value *Value::__add__(Value *a) {
        printf("Value '%d' not suppose operator symbol '+'\n", vk);
        exit(-1);
    }

    Value *Value::__sub__(Value *a) {
        printf("Value '%d' not suppose operator symbol '-'\n", vk);
        exit(-1);
    }

    Value *Value::__div__(Value *a) {
        printf("Value '%d' not suppose operator symbol '/'\n", vk);
        exit(-1);
    }

    Value *Value::__mul__(Value *a) {
        printf("Value '%d' not suppose operator symbol '*'\n", vk);
        exit(-1);
    }

    Value *Value::__lmv__(Value *a) {
        printf("Value '%d' not suppose operator symbol '<<'\n", vk);
        exit(-1);
    }

    Value *Value::__rmv__(Value *a) {
        printf("Value '%d' not suppose operator symbol '>>'\n", vk);
        exit(-1);
    }

    Value *Value::__mod__(Value *a) {
        printf("Value '%d' not suppose operator symbol '%'\n", vk);
        exit(-1);
    }

    Value *Value::__get_val__(Value *a) {
        printf("Value '%d' not suppose operator symbol '[]'\n", vk);
        exit(-1);
    }

    void Value::__set__(Value *a) {
        printf("Value '%d' not suppose operator symbol '='\n", vk);
        exit(-1);
    }

    Value *Value::get_member(std::string m) {
        printf("Value '%d' not suppose operator symbol '.'\n", vk);
        exit(-1);
    }

    Value *Value::__or__(Value *a) {
        printf("Value '%d' not suppose operator symbol '||'\n", vk);
        exit(-1);
    }

    Value *Value::__and__(Value *a) {
        printf("Value '%d' not suppose operator symbol '&&'\n", vk);
        exit(-1);
    }

    Value::Value() {

    }

    Value::Value(OplType::AST *a) {
        this->value = a;
    }

    Value::Value(ValueKind vk, OplType::AST *a) {
        this->vk = vk;
        this->value = a;
    }

    class BaseObject : public Value {
    public:
        std::unordered_map<std::string, Value*> members;
        Value* get_member(std::string);
        BaseObject();
    };

    BaseObject::BaseObject() { vk = VK_CLASS; }

    class Bool : public Value {
    public:
        bool b = false;
        Bool(bool);
        Value* __or__(Value*) override;
        Value* __and__(Value*) override;
        Value* __not__() override;
    };

    Value* Bool::__or__(Value* v) {
        return new Bool( b || ((Bool*)v)->b );
    }

    Value* Bool::__and__(Value* v) {
        return new Bool( b && ((Bool*)v)->b );
    }

    Value* Bool::__not__() {
        return new Bool(!b);
    }

    Bool::Bool(bool a) { vk = VK_BOOL; b = a; }

    class Digit : public Value { 
    public: 
        Digit(OplType::AST*);
        Value* __add__(Value*) override;
        Value* __sub__(Value*) override;
        Value* __div__(Value*) override;
        Value* __mul__(Value*) override;
        Value* __mod__(Value*) override;
        Value* __lmv__(Value*) override;
        Value* __rmv__(Value*) override;
        Value* __and__(Value*) override;
        Value* __or__(Value*) override;

        Value* __greater_than__(Value*) override;
        Value* __less_than__(Value*) override;
        Value* __eq__(Value*) override;
        Value* __not_eq__(Value*) override;
        Value* __not__() override;
        Value* __bg_or_eq__(Value*) override;
        Value* __ls_or_eq__(Value*) override;

        void __set__(Value*) override;
    };

    Value* Digit::__bg_or_eq__(Value* v) {
        return new Bool(
            std::stod(this->value->data.data) >= std::stod(v->value->data.data)
        );
    }

    Value* Digit::__ls_or_eq__(Value* v) {
        return new Bool(
            std::stod(this->value->data.data) <= std::stod(v->value->data.data)
        );
    }

    void Digit::__set__(Value* v) {
        this->value = v->value;
    }

    Value* Digit::__greater_than__(Value*v) {
        return new Bool(
            std::stod(this->value->data.data) > std::stod(v->value->data.data)
        );
    }

    Value* Digit::__less_than__(Value* v) {
        return new Bool(
            std::stod(this->value->data.data) < std::stod(v->value->data.data)
        );
    }

    Value* Digit::__eq__(Value* v) {
        return new Bool(
            std::stod(this->value->data.data) == std::stod(v->value->data.data)
        );
    }

    Value* Digit::__not_eq__(Value* v) {
        return new Bool(
            std::stod(this->value->data.data) != std::stod(v->value->data.data)
        );
    }

    Value* Digit::__not__() {
        return new Digit(new OplType::DigitNode(OplType::Token {
            .type = OplType::TT_INTEGER,
            .data = std::to_string(!std::stoi(this->value->data.data))
        }));
    }

    Value* Digit::__add__(Value* a) {
        if (a->vk == VK_CHAR) {
            return new Digit(new OplType::DigitNode(OplType::Token {
                .type = OplType::TT_INTEGER,
                .data = std::to_string(std::stoi(this->value->data.data) + (int)a->value->data.data[0])
            }));
        }
        return new Digit(new OplType::DigitNode(OplType::Token {
            .type = OplType::TT_INTEGER,
            .data = std::to_string(std::stod(this->value->data.data) + std::stod(a->value->data.data))
        }));
    } 
    
    Value* Digit::__sub__(Value* a) {
        if (a->vk == VK_CHAR) {
            return new Digit(new OplType::DigitNode(OplType::Token {
                .type = OplType::TT_INTEGER,
                .data = std::to_string(std::stoi(this->value->data.data) - (int)a->value->data.data[0])
            }));
        }
        return new Digit(new OplType::DigitNode(OplType::Token {
            .type = OplType::TT_INTEGER,
            .data = std::to_string(std::stod(this->value->data.data) - std::stod(a->value->data.data))
        }));
    }
    
    Value* Digit::__div__(Value* a) {
        if (a->vk == VK_CHAR) {
            return new Digit(new OplType::DigitNode(OplType::Token {
                .type = OplType::TT_INTEGER,
                .data = std::to_string(std::stoi(this->value->data.data) / (int)a->value->data.data[0])
            }));
        }
        return new Digit(new OplType::DigitNode(OplType::Token {
            .type = OplType::TT_INTEGER,
            .data = std::to_string(std::stod(this->value->data.data) / std::stod(a->value->data.data))
        }));
    }
    
    Value* Digit::__mul__(Value* a) {
        if (a->vk == VK_CHAR) {
            return new Digit(new OplType::DigitNode(OplType::Token {
                .type = OplType::TT_INTEGER,
                .data = std::to_string(std::stoi(this->value->data.data) * (int)a->value->data.data[0])
            }));
        }
        return new Digit(new OplType::DigitNode(OplType::Token {
            .type = OplType::TT_INTEGER,
            .data = std::to_string(std::stod(this->value->data.data) * std::stod(a->value->data.data))
        }));
    }
    
    Value* Digit::__mod__(Value* a) {
        if (a->vk == VK_CHAR) {
            return new Digit(new OplType::DigitNode(OplType::Token {
                .type = OplType::TT_INTEGER,
                .data = std::to_string(std::stoi(this->value->data.data) % (int)a->value->data.data[0])
            }));
        }
        return new Digit(new OplType::DigitNode(OplType::Token {
            .type = OplType::TT_INTEGER,
            .data = std::to_string(std::stoi(this->value->data.data) % std::stoi(a->value->data.data))
        }));
    }
    
    Value* Digit::__lmv__(Value* a) {
        if (a->vk == VK_CHAR) {
            return new Digit(new OplType::DigitNode(OplType::Token {
                .type = OplType::TT_INTEGER,
                .data = std::to_string(std::stoi(this->value->data.data) << (int)a->value->data.data[0])
            }));
        }
        return new Digit(new OplType::DigitNode(OplType::Token {
            .type = OplType::TT_INTEGER,
            .data = std::to_string(std::stoi(this->value->data.data) << std::stoi(a->value->data.data))
        }));
    }
    
    Value* Digit::__rmv__(Value* a) {
        if (a->vk == VK_CHAR) {
            return new Digit(new OplType::DigitNode(OplType::Token {
                .type = OplType::TT_INTEGER,
                .data = std::to_string(std::stoi(this->value->data.data) >> (int)a->value->data.data[0])
            }));
        }
        return new Digit(new OplType::DigitNode(OplType::Token {
            .type = OplType::TT_INTEGER,
            .data = std::to_string(std::stoi(this->value->data.data) >> std::stoi(a->value->data.data))
        }));
    }
    

    Digit::Digit(OplType::AST *a): Value(a) { vk = VK_DIGIT; }

    Value *Digit::__and__(Value *a) {
        if (a->vk == VK_CHAR) {
            return new Digit(new OplType::DigitNode(OplType::Token {
                .type = OplType::TT_INTEGER,
                .data = std::to_string(std::stoi(this->value->data.data) & (int)a->value->data.data[0])
            }));
        }
        return new Digit(new OplType::DigitNode(OplType::Token {
            .type = OplType::TT_INTEGER,
            .data = std::to_string(std::stoi(this->value->data.data) & std::stoi(a->value->data.data))
        }));
    }

    Value *Digit::__or__(Value *a) {
        if (a->vk == VK_CHAR) {
            return new Digit(new OplType::DigitNode(OplType::Token {
                .type = OplType::TT_INTEGER,
                .data = std::to_string(std::stoi(this->value->data.data) | (int)a->value->data.data[0])
            }));
        }
        return new Digit(new OplType::DigitNode(OplType::Token {
            .type = OplType::TT_INTEGER,
            .data = std::to_string(std::stoi(this->value->data.data) | std::stoi(a->value->data.data))
        }));
    }

    class Char : public Value {
    public:
        Char(OplType::AST*);
        Value* __add__(Value*) override;
        Value* __sub__(Value*) override;
        void __set__(Value*) override;
    };

    void Char::__set__(Value* v) {
        this->value = v->value;
    }

    Value* Char::__add__(Value* a) {
        if (a->vk == VK_CHAR) {
            return new Value(new OplType::CharNode(OplType::Token {
                .type = OplType::AST_CHAR,
                .data = std::to_string((char)(this->value->data.data[0] + a->value->data.data[0]))
            }));
        }
        return new Value(new OplType::CharNode(OplType::Token {
            .type = OplType::AST_CHAR,
            .data = std::to_string((char)(this->value->data.data[0] + std::stoi(a->value->data.data)))
        }));
    }

    Char::Char(OplType::AST *a): Value(a) {
        vk = VK_CHAR;
    }

    Value *Char::__sub__(Value *a) {
        if (a->vk == VK_CHAR) {
            return new Value(new OplType::CharNode(OplType::Token {
                .type = OplType::AST_CHAR,
                .data = std::to_string((char)(this->value->data.data[0] - a->value->data.data[0]))
            }));
        }
        return new Value(new OplType::CharNode(OplType::Token {
            .type = OplType::AST_CHAR,
            .data = std::to_string((char)(this->value->data.data[0] - std::stoi(a->value->data.data)))
        }));
    }

    class Array : public Value {
    public:
        std::vector<Value*> values;
        Array();
        Value* __get_val__(Value*) override;
    };

    Value* Array::__get_val__(Value* position) {
        return values[std::stoi(position->value->data.data)];
    }

    Array::Array() {
        vk = VK_ARRAY;
    }

    class String : public Value {
    public:
        String(OplType::AST*);
        Value* __add__(Value*) override;
        Value* __mul__(Value*) override;
        Value* __get_val__(Value*) override;
    };

    Value* String::__get_val__(Value* pos) {
        std::string nc;
        nc += this->value->data.data[std::stoi(pos->value->data.data)];
        return new Char(new OplType::CharNode(OplType::Token {
            .type = OplType::TT_CHAR,
            .data = nc
        }));
    }

    Value* String::__mul__(Value* ov) {
        std::string new_string;
        for (int i = 0; i < std::stoi(ov->value->data.data); ++i)
            new_string += this->value->data.data;
        return new String(new OplType::StringNode(OplType::Token {
            .type = OplType::TT_STRING,
            .data = new_string
        }));
    }

    Value* String::__add__(Value* other) {
        return new String(new OplType::StringNode(OplType::Token {
            .type = OplType::TT_STRING,
            .data = this->value->data.data + other->value->data.data
        }));
    }

    String::String(OplType::AST* a): Value(a) {
        vk = VK_STRING;
    }

    Value *BaseObject::get_member(std::string name) {
        auto c = members.find(name);
        if (c == members.end()) {
            printf("RuntimeError: member '%s' not found in object '%s'\n", name.c_str(), value->data.data.c_str());
            exit(-1);
        }
        return c->second;
    }

    class Function : public Value {
    public:
        Function(OplType::AST*);
    };

    Function::Function(OplType::AST *a) {
        this->value = a;
    }

    struct Context {
        Context* parent_context = nullptr;
        std::string display_name;
        std::unordered_map<std::string, Value*> id_map;
        Value* get_value(std::string);
        Context(std::string);
        Context(std::string, Context*);
        Context* get_global_context();
    };

    Context* Context::get_global_context() {
        if (!parent_context) return this;
        return parent_context->get_global_context();
    }

    Value* Context::get_value(std::string name) {
        auto this_scope = id_map.find(name);
        if (this_scope == id_map.end() && parent_context) 
            return parent_context->get_value(name);
        return this_scope->second;
    }

    Context::Context(std::string display_name) {
        this->display_name = display_name;
    }

    Context::Context(std::string display_name, Context *parent) {
        this->display_name = display_name;
        this->parent_context = parent;
    }

    class RTResult : public Value {
    public:
        RTResult();
        bool sh_break = false, sh_continue = false, sh_return = false;
        void _break();
        void _continue();
        void _return();
    };

    void RTResult::_break() {
        sh_break = true;
    }

    void RTResult::_continue() {
        sh_continue = true;
    }

    void RTResult::_return() {
        sh_return = true;
    }
    
    RTResult::RTResult() {
        vk = VK_RTR;
    }

    class Program {
    public:
        Program(OplType::AST*, Context*, std::vector<Value*>);
        Program(Value*, Context*, std::vector<Value*>);
        Value* result;
    private:
        void crate(std::string);
        void leave();
        std::string fn = "<lambdaExpression>";
        OplType::AST* ast;
        Context* context;
        Value* visit_atom(OplType::AST*);
        Value* visit_function_define(OplType::AST*);
        Value* visit_func_call(OplType::AST*);
        Value* visit_bin_op_node(OplType::AST*);
        Value* visit_if_node(OplType::AST*);
        Value* visit_while_node(OplType::AST*);
        Value* visit_for_node(OplType::AST*);
        Value* visit_type_cast_node(OplType::AST*);
		Value* visit_not_node(OplType::AST*);
        Value* visit_digit_node(OplType::AST*);
        Value* visit_string_node(OplType::AST*);
        Value* visit_get_value(OplType::AST*);
        Value* visit_lambda_node(OplType::AST*);
        Value* visit_lambda_call(OplType::AST*);
        Value* visit_new_node(OplType::AST*);
        Value* visit_assign_node(OplType::AST*);
        Value* visit_var_define_node(OplType::AST*);
        Value* evalute_member_access_expr(OplType::AST*);
        Value* visit_char_node(OplType::AST*);
    };
	
	Value* Program::visit_not_node(OplType::AST* a) {
		return visit_bin_op_node(a)->__not__();
	}

    void Program::crate(std::string s) {
        context = new Context(s, context);
    }

    void Program::leave() {
        context = context->parent_context;
    }

    Value* Program::evalute_member_access_expr(OplType::AST* a) {
        if (a->type == OplType::AST_MEMBER) // a.b.c.d
            return ((BaseObject*) evalute_member_access_expr(a->children[0]))->get_member(a->children[1]->data.data);
        if (a->type == OplType::AST_ID) // a
            return context->get_value(a->data.data);
        if (a->type == OplType::AST_GET_VALUE) // a[position]
            return visit_get_value(a);
    }

    Value *Program::visit_function_define(OplType::AST *fn_) {
        std::string name = fn_->data.data;
        context->id_map[name] = new Value(VK_FUNC, fn_);
        return context->id_map[name];
    }

    Program::Program(OplType::AST* ast, Context* context, std::vector<Value*> args) {
        this->ast = ast;
        this->context = context;
        fn = ast->data.data;
    }

    Value *Program::visit_atom(OplType::AST *a) {
        std::string type = a->type;
        RTResult* rtr = new RTResult();
        if (type == OplType::AST_ASSIGN) return visit_assign_node(a);
        if (type == OplType::AST_BIN_OP) return visit_bin_op_node(a);
        if (type == OplType::AST_DIGIT) return visit_digit_node(a);
        if (type == OplType::AST_STRING) return visit_string_node(a);
        if (type == OplType::AST_FOR_LOOP) return visit_for_node(a);
        if (type == OplType::AST_WHILE_LOOP) return visit_while_node(a);
        if (type == OplType::AST_FUNC_CALL) return visit_func_call(a);
        if (type == OplType::AST_GET_VALUE) return visit_get_value(a);
        if (type == OplType::AST_MEM_MALLOC) return visit_new_node(a);
		if (type == OplType::AST_NOT) return visit_not_node(a);
        if (type == OplType::AST_VAR_DEF) return visit_var_define_node(a);
        if (type == OplType::AST_LAMBDA_CALL) return visit_lambda_call(a);
        if (type == OplType::AST_LAMBDA) return visit_lambda_node(a);
        if (type == OplType::AST_RETURN) {
            if (a->children[0]) result = visit_bin_op_node(a->children[0]);
            rtr->_return();
        }
        if (type == OplType::AST_CONTINUE) rtr->_continue(); 
        if (type == OplType::AST_BREAK) rtr->_break();
        if (type == OplType::AST_IF) return visit_if_node(a);
        if (type == OplType::AST_FUNCTION_DEFINE) return visit_function_define(a);
        if (type == OplType::AST_CONV) return visit_type_cast_node(a);
        if (type == OplType::AST_BLOCK)
            for (auto i : a->children)
                visit_atom(i);
        return rtr;
    }

    Program::Program(Value* val, Context* context, std::vector<Value*> args) {
        if (val->vk != VK_FUNC) {
            printf("not a function\n");
            exit(-1);
        }
        this->ast = val->value;
        this->context = context;
    }

    Value *Program::visit_func_call(OplType::AST *a) {
        std::vector<Value*> func_args;
        for (int i = 1; i < a->children.size(); ++i)
            func_args.push_back(visit_bin_op_node(a->children[i]));
        return (
            new OplInterpreter::Program(
                evalute_member_access_expr(a),
                new Context(a->children[1]->data.data, context->get_global_context()),
                func_args
            )
        )->result;
    }

    Value *Program::visit_bin_op_node(OplType::AST *a) {
        if (a->type != OplType::AST_BIN_OP)
            return visit_atom(a);
        auto left_ = visit_bin_op_node(a->children[0]);
        auto right_ = visit_bin_op_node(a->children[1]);
        std::string op = a->data.data;
        if (op == "+") return left_->__add__(right_);
        if (op == "-") return left_->__sub__(right_);
        if (op == "<=") return left_->__ls_or_eq__(right_);
        if (op == ">=") return left_->__bg_or_eq__(right_);
        if (op == "/") return left_->__div__(right_);
        if (op == "*") return left_->__mul__(right_);
        if (op == "%") return left_->__mod__(right_);
        if (op == "<<") return left_->__lmv__(right_);
        if (op == ">>") return left_->__rmv__(right_);
        if (op == "|" || op == "||" || op == "or") return left_->__or__(right_);
        if (op == "&" || op == "&&" || op == "and") return left_->__and__(right_);
        printf("Unknown operator symbol '%s'\n", op.c_str());
        exit(-1);
    }

    Value *Program::visit_if_node(OplType::AST *a) {
        RTResult* rtr = new RTResult();
        auto condition = a->children[0];
        auto tr = a->children[1];
        auto fr = a->children[2];
        auto ist = (Bool*) visit_bin_op_node(condition);
        crate("<ifExpression>");
        if (ist->b) return visit_atom(tr);
        else if (fr) return visit_atom(fr);
        leave();
        return rtr;
    }

    Value *Program::visit_while_node(OplType::AST *a) {
        auto rtr = new RTResult();
        auto condition = a->children[0];
        auto body = a->children[1];
        int pc = 0;
        while (pc < body->children.size()) {
            auto rtr_ = (RTResult*) visit_atom(a->children[pc]);
            if (rtr_->sh_break) break;
            else if (rtr_->sh_continue) pc = 0;
            else if (rtr_->sh_return) {
				rtr_->_return();
                return rtr_;
            }
            else ++pc;
        }
        return rtr;
    }

    Value *Program::visit_for_node(OplType::AST *a) {
        auto rtr = new RTResult();
        auto init = a->children[0];
        auto chang = a->children[2];
        auto body = a->children[3]->children;
        visit_atom(init);
		auto is_loop  = (Bool*) visit_atom(a->children[1]);
        int pc = 0;
        while (pc < body.size() && is_loop->b) {
			auto rr = (RTResult*) visit_atom(body[pc]);
			if (rr->sh_break) break;
			if (rr->sh_continue) pc = 0;
			else if (rr->sh_return) {
				rr->_return();
				return rr;
			}
			visit_atom(chang);
			is_loop = (Bool*) visit_atom(a->children[1]);
        }
        return rtr;
    }

    Value *Program::visit_type_cast_node(OplType::AST *) {
        return nullptr;
    }

    Value *Program::visit_assign_node(OplType::AST *a) {

        return nullptr;
    }

    Value *Program::visit_var_define_node(OplType::AST *a) {
        std::string name = a->data.data;
        auto data = a->children[1];
        context->id_map[name] = visit_bin_op_node(data);
        return nullptr;
    }

    Value *Program::visit_lambda_call(OplType::AST *) {
        return nullptr;
    }

    Value *Program::visit_string_node(OplType::AST *a) {
        return new String(a);
    }

    Value *Program::visit_digit_node(OplType::AST *a) {
        return new Digit(a);
    }

    Value *Program::visit_lambda_node(OplType::AST *lb) {
        return new Function(lb);
    }

    Value *Program::visit_new_node(OplType::AST *n) {
        auto value_ = n->children[0];
        if (value_->type == OplType::AST_FUNC_CALL) {
            std::string obj_name = value_->children[0]->data.data;
            auto obj = new BaseObject();
            for (auto i : n->children) {
                if (i->type == OplType::AST_VAR_DEF)
                    obj->members[i->data.data] = new Value(VK_VALUE, i->children[1]);
                if (i->type == OplType::AST_FUNCTION_DEFINE)
                    obj->members[i->data.data] = new Function(i);
            }
            std::vector<Value*> values;
            auto new_context = new Context(obj_name + ".constructor", context->get_global_context());
            new_context->id_map["this"] = context->id_map[obj_name];
            for (int j = 1; j < value_->children.size(); ++j)
                values.push_back(visit_bin_op_node(value_->children[j]));
            Program (
                ((BaseObject*)context->id_map[obj_name])->members["constructor"],
                new_context,
                values
            );
            return obj;
        } else {
            if (value_->is_block) return new Array();
            std::string obj_name = value_->children[0]->data.data;
            auto obj = new BaseObject();
            for (auto i : n->children) {
                if (i->type == OplType::AST_VAR_DEF)
                    obj->members[i->data.data] = new Value(VK_VALUE, i->children[1]);
                if (i->type == OplType::AST_FUNCTION_DEFINE)
                    obj->members[i->data.data] = new Function(i);
            }
            return obj;
        }
    }

    Value *Program::visit_get_value(OplType::AST *a) {
        return evalute_member_access_expr(a->children[0])->__get_val__(
            visit_bin_op_node(a->children[1])
        );
    }

    Value *Program::visit_char_node(OplType::AST *) {
        return nullptr;
    }

}
#endif //PLINC_PROGRAM_HPP
