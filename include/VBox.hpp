#ifndef VBOX
#define VBOX
#include "BuildIn.hpp"
#include <unordered_map>
#include <stack>
#include <vector>
#include <string>

namespace OplVBox {

#define CURRENT_FRAME frames.top()

    struct Frame {
        std::unordered_map<int, int> var_list;
        std::stack<int> st;
        std::vector<int> code;
        int pc, a, function_point;

        void psh(int);

        int pop();

        void stor(int, int);

        int get(int);

        int run();

        Frame();
    };

    Frame::Frame() {
        pc = a = function_point = 0;
    }

    int Frame::run() {
        if (pc >= code.size())
            return OplBuildIn::_lea;
        return code[pc++];
    }

    int Frame::get(int addr) {
        return var_list[addr];
    }

    void Frame::stor(int addr, int val) {
        var_list[addr] = val;
    }

    int Frame::pop() {
        int v = st.top();
        st.pop();
        return v;
    }

    void Frame::psh(int value) {
        st.push(value);
    }

    class VirtualMachine {
    public:
        VirtualMachine(std::vector<int>);

        void show_byte_code();

        void init();

        int execute();

        bool is_debug = false;
    private:
        int main_point = -1;
        std::vector<int> heap;
        int htop = 0;
        std::vector<int> opcs;
        std::stack<int> func_value;
        std::vector<OplBuildIn::Function> functions;
        std::vector<OplBuildIn::Object> objects;

        OplBuildIn::Object *find_object(int);

        OplBuildIn::Function *find_function(int);

        std::stack<Frame> frames;

        int new_array(int);

        int store_string(std::string);

        int crate_object(int);

        void crate(int);

        void leave();

        void load();

        void call_build_in(int);

        bool is_build_in(int);
    };

    int VirtualMachine::new_array(int size) {
        int t = htop;
        htop += size + 1; // +1 for length
        heap[t] = size; // store length at the beginning
        return t;
    }

    int VirtualMachine::store_string(std::string s) {
        int thtop = htop;
        htop += s.size() + 1;
        for (int i = 0; i < s.size(); ++i)
            heap[thtop + i] = s[i];
        heap[thtop + s.size()] = 0; // null terminator
        return thtop;
    }

    void VirtualMachine::leave() {
        frames.pop();
    }

    void VirtualMachine::init() {
        load();
        if (main_point == -1) {
            printf("NoMain\n");
            exit(-1);
        }
        crate(main_point);
    }

    void VirtualMachine::show_byte_code() {
        printf("main_function -> %d\n", main_point);
        for (auto i: functions) {
            printf("Function %d: \n", i.id);
            OplBuildIn::format_assembly(i.codes, "      ");
        }
        for (auto i: objects) {
            printf("Object %d: \n", i.id);
            for (auto u: i.member_map)
                printf("      %d   :   %d\n", u.first, u.second);
        }
    }

    void VirtualMachine::load() {
        int i = 0;
        while (i < opcs.size()) {
            int j = opcs[i++];
            if (j == OplBuildIn::SET_MAIN) main_point = opcs[i++];
            if (j == OplBuildIn::SET_CLASS) {
                OplBuildIn::Object object;
                object.id = opcs[i++];
                object.size = opcs[i++];
                int ps = opcs[i++];
                for (int k = 0; k < ps; ++k)
                    object.parent_id.push_back(opcs[i++]);
                for (int k = 0; k < object.size; ++k) {
                    object.member_map[opcs[i++]] = opcs[i++];
                }
                objects.push_back(object);
            }
            if (j == OplBuildIn::SET_FUNC) {
                OplBuildIn::Function func;
                func.id = opcs[i++];
                func.value_list_size = opcs[i++];
                int bs = opcs[i++];
                for (int k = 0; k < bs; ++k)
                    func.codes.push_back(opcs[i++]);
                if (is_build_in(func.id))
                    func.type = OplBuildIn::FT_BUILD_IN;
                functions.push_back(func);
            }
        }
    }

    VirtualMachine::VirtualMachine(std::vector<int> opcs) {
        this->opcs = opcs;
        heap.resize(10000); // Allocate heap memory
    }

    void VirtualMachine::crate(int id) {
        Frame nf;
        auto fn = find_function(id);
        nf.code = fn->codes;
        nf.function_point = id;
        frames.push(nf);
        while (!func_value.empty()) {
            CURRENT_FRAME.psh(func_value.top());
            func_value.pop();
        }
    }

    void VirtualMachine::call_build_in(int id) {
        switch (id) {
            case OplBuildIn::PRINT_: {
                int addr = CURRENT_FRAME.pop();
                std::string s;
                int i = 0;
                while (heap[addr + i] != 0) {
                    s += (char)heap[addr + i];
                    i++;
                }
                printf("%s", s.c_str());
                break;
            }
            case OplBuildIn::INPUT_: {
                std::string input;
                std::cin >> input;
                int addr = store_string(input);
                CURRENT_FRAME.psh(addr);
                break;
            }
            case OplBuildIn::ARR_SET: {
                int val = CURRENT_FRAME.pop();
                int offset = CURRENT_FRAME.pop();
                int addr = CURRENT_FRAME.pop();
                heap[addr + offset + 1] = val; // +1 to skip length
                break;
            }
            case OplBuildIn::ARR_GET: {
                int offset = CURRENT_FRAME.pop();
                int addr = CURRENT_FRAME.pop();
                int value = heap[addr + offset + 1]; // +1 to skip length
                CURRENT_FRAME.psh(value);
                break;
            }
            case OplBuildIn::TO_STRING_: {
                int num = CURRENT_FRAME.pop();
                std::string s = std::to_string(num);
                int addr = store_string(s);
                CURRENT_FRAME.psh(addr);
                break;
            }
            case OplBuildIn::TO_INTEGER_: {
                int addr = CURRENT_FRAME.pop();
                std::string s;
                int i = 0;
                while (heap[addr + i] != 0) {
                    s += (char)heap[addr + i];
                    i++;
                }
                int num = std::stoi(s);
                CURRENT_FRAME.psh(num);
                break;
            }
            case OplBuildIn::ARR_LEN : {
                int addr = CURRENT_FRAME.pop();
                int len = heap[addr]; // length is stored at the beginning
                CURRENT_FRAME.psh(len);
                break;
            }
            default: {
                break;
            }
        }
    }

    bool VirtualMachine::is_build_in(int id) {
        if (
                id == OplBuildIn::PRINT_ ||
                id == OplBuildIn::INPUT_ ||
                id == OplBuildIn::TO_STRING_ ||
                id == OplBuildIn::TO_INTEGER_ ||
                id == OplBuildIn::ARR_LEN ||
                id == OplBuildIn:: ARR_GET ||
                id == OplBuildIn::ARR_SET
                )
            return true;
        return false;
    }

    int VirtualMachine::execute() {
        while (!frames.empty()) {
            int i = CURRENT_FRAME.run();
            switch (i) {
                case OplBuildIn::_add: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - %d + %d\n", a, b);
                    CURRENT_FRAME.psh(a + b);
                    break;
                }
                case OplBuildIn::_sub: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - %d - %d\n", a, b);
                    CURRENT_FRAME.psh(a - b);
                    break;
                }
                case OplBuildIn::_div: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - %d / %d\n", a, b);
                    CURRENT_FRAME.psh(a / b);
                    break;
                }
                case OplBuildIn::_hp_stor: {
                    int value = CURRENT_FRAME.pop();
                    int addr = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - _hp_stor value:%d  address:%d\n", value, addr);
                    heap[addr] = value;
                    break;
                }
                case OplBuildIn::_new_array: {
                    int size = CURRENT_FRAME.pop();
                    int type = CURRENT_FRAME.run();
                    if (is_debug) printf(" - _new_array size:%d  type:%d\n", size, type);
                    int addr = new_array(size);
                    CURRENT_FRAME.psh(addr);
                    break;
                }
                case OplBuildIn::_mul: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - %d * %d\n", a, b);
                    CURRENT_FRAME.psh(a * b);
                    break;
                }
                case OplBuildIn::_or: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - %d or %d\n", a, b);
                    CURRENT_FRAME.psh(a | b);
                    break;
                }
                case OplBuildIn::_and: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - %d and %d\n", a, b);
                    CURRENT_FRAME.psh(a & b);
                    break;
                }
                case OplBuildIn::_cls: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - cmp %d < %d\n", a, b);
                    CURRENT_FRAME.psh(a < b);
                    break;
                }
                case OplBuildIn::_ceq: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - cmp %d == %d\n", a, b);
                    CURRENT_FRAME.psh(a == b);
                    break;
                }
                case OplBuildIn::_cels: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - cmp %d <= %d\n", a, b);
                    CURRENT_FRAME.psh(a <= b);
                    break;
                }
                case OplBuildIn::_hsst: {
                    std::string s;
                    while (true) {
                        int ch = CURRENT_FRAME.pop();
                        if (ch == 0) break;
                        s += (char)ch;
                    }
                    int addr = store_string(s);
                    if (is_debug) printf(" - _hsst %s\n", s.c_str());
                    CURRENT_FRAME.psh(addr);
                    break;
                }
                case OplBuildIn::_cebg: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - cmp %d >= %d\n", a, b);
                    CURRENT_FRAME.psh(a >= b);
                    break;
                }
                case OplBuildIn::_cbg: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - cmp %d > %d\n", a, b);
                    CURRENT_FRAME.psh(a > b);
                    break;
                }
                case OplBuildIn::_jt: {
                    int condition = CURRENT_FRAME.pop();
                    int addr = CURRENT_FRAME.run();
                    if (is_debug) printf(" - jmp if true %d to %d\n", condition, addr);
                    if (condition) CURRENT_FRAME.pc = addr;
                    break;
                }
                case OplBuildIn::_jmp: {
                    int addr = CURRENT_FRAME.run();
                    if (is_debug) printf(" - jmp to %d\n", addr);
                    CURRENT_FRAME.pc = addr;
                    break;
                }
                case OplBuildIn::_left_mv: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - %d << %d\n", a, b);
                    CURRENT_FRAME.psh(a << b);
                    break;
                }
                case OplBuildIn::_right_mv: {
                    int b = CURRENT_FRAME.pop();
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - %d >> %d\n", a, b);
                    CURRENT_FRAME.psh(a >> b);
                    break;
                }
                case OplBuildIn::_not: {
                    int a = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - not %d\n", a);
                    CURRENT_FRAME.psh(!a);
                    break;
                }
                case OplBuildIn::_lea: {
                    if (is_debug) printf(" - leave %d\n", CURRENT_FRAME.function_point);
                    leave();
                    break;
                }
                case OplBuildIn::_psh: {
                    int v = CURRENT_FRAME.run();
                    if (is_debug) printf(" - push %d\n", v);
                    CURRENT_FRAME.psh(v);
                    break;
                }
                case OplBuildIn::_dup: {
                    CURRENT_FRAME.psh(CURRENT_FRAME.st.top());
                    break;
                }
                case OplBuildIn::_i_stor: {
                    int value = CURRENT_FRAME.pop();
                    int offset = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - store offset[%d], value[%d]\n", offset, value);
                    CURRENT_FRAME.stor(offset, value);
                    break;
                }
                case OplBuildIn::_imm: {
                    int addr = CURRENT_FRAME.pop();
                    int value = heap[addr];
                    if (is_debug) printf(" - _imm addr:%d value:%d\n", addr, value);
                    CURRENT_FRAME.psh(value);
                    break;
                }
                case OplBuildIn::_sparam: {
                    int val = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - _sparam %d\n", val);
                    func_value.push(val);
                    break;
                }
                case OplBuildIn::_param: {
                    int val = CURRENT_FRAME.run();
                    if (is_debug) printf(" - _param %d\n", val);
                    func_value.push(val);
                    break;
                }
                case OplBuildIn::_new: {
                    int obj_id = CURRENT_FRAME.run();
                    int obj_addr = crate_object(obj_id);
                    if (is_debug) printf(" - obj_id %d addr:%d\n", obj_id, obj_addr);
                    CURRENT_FRAME.psh(obj_addr);
                    break;
                }
                case OplBuildIn::_call: {
                    int id = CURRENT_FRAME.run();
                    if (is_debug) printf(" - call %d\n", id);
                    if (is_build_in(id)) {
                        call_build_in(id);
                    } else {
                        crate(id);
                    }
                    break;
                }
                case OplBuildIn::_i_load: {
                    int offset = CURRENT_FRAME.pop();
                    int val = CURRENT_FRAME.get(offset);
                    if (is_debug) printf(" - i_load offset[%d], value[%d]\n", offset, val);
                    CURRENT_FRAME.psh(val);
                    break;
                }
                case OplBuildIn::_ret: {
                    int val = CURRENT_FRAME.pop();
                    if (is_debug) printf(" - return %d\n", val);
                    leave();
                    if (!frames.empty()) {
                        CURRENT_FRAME.psh(val);
                    }
                    break;
                }
                case OplBuildIn::_nop: {
                    if (is_debug) printf(" - nop\n");
                    break;
                }
                default: {
                    auto a = OplBuildIn::asm_string_map.find(i);
                    if (a != OplBuildIn::asm_string_map.end()) {
                        printf("Operator %s not supposed\n", a->second.c_str());
                    } else {
                        printf("Operator 0x%x not found\n", i);
                        printf("Dec = %d\n", i);
                    }
                    break;
                }
            }
        }
        return 0;
    }

    OplBuildIn::Function *VirtualMachine::find_function(int id) {
        for (auto &function: functions)
            if (function.id == id)
                return &function;
        return nullptr;
    }

    OplBuildIn::Object *VirtualMachine::find_object(int id) {
        for (auto &object: objects)
            if (object.id == id)
                return &object;
        return nullptr;
    }

    int VirtualMachine::crate_object(int id) {
        auto obj = find_object(id);
        if (!obj) {
            printf("Object %d not found\n", id);
            exit(-1);
        }
        int hhtop = htop;
        htop += obj->size + 1;
        heap[hhtop] = obj->id; // store object id at the beginning
        return hhtop;
    }
}
#endif