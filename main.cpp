#include "include/Compiler.hpp"
#include "include/Parser.hpp"
#include "include/Lexer.hpp"
#include "include/Color256.hpp"
#include <windows.h>
#include <dir.h>
#include "include/VBox.hpp"
#include "include/Tools.hpp"

#define VM_TEST

#ifdef DEBUG_MODE
int main() {
	string exam = ".\\script\\exam.opl";
	string test1 = ".\\script\\test1.opl";
	string test2 = ".\\script\\test2.opl";
	string data = read_file(exam);
	Lexer lexer(data);
	Parser parser(lexer.tokens);
	Compiler compiler(parser.result.ast);
	MainCompiler compiler1(compiler.get_compile_result().opcs);
	auto functions = compiler1.vmbc.functions;
	auto objects = compiler1.vmbc.objects;
	for (auto i : objects) {
		printf("Object[%d]: \n", i.id);
		for (auto j : i.member_map)
			printf("    %d : %d\n", j.first, j.second);
	}
	for (auto i : functions) {
		printf("Function[%d]: \n", i.id);
		format_assembly(i.codes, "    ");
	}
	auto a = make_program(compiler1.vmbc);
	for (int i = 0; i < a.size(); ++i) {
		if (i % 12 == 0) printf("\n");
		printf("0x%x ", a[i]);
	}
	cout << "\nVirtualMachine start: \n";
}
#endif


#ifdef VM_TEST
int main() {
    char buffer[1024];
	string exam = R"(..\script\exam.opl)";
	string test1 = R"(..\script\test1.opl)";
	string test2 = R"(..\script\test2.opl)";
	string data = read_file(test1);
	OplLexer::Lexer lexer(data);
	OplParser::Parser parser(lexer.tokens);
	for (auto i : parser.result.ast)
		print_tree(i);
}
#endif

#ifdef RELEASE_MODE
int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s <InputFileName> <OutputFileName>\n", argv[0]);
		exit(-1);
	}
	string in_  = argv[1];
	string out_ = argv[2];
	string sc   = read_file(in_);
}
#endif