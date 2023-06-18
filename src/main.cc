#include "main.h"

auto main(const int argc, const char **argv) -> int
{
    if (argc != 2)
    {
        std::cout << RED_BOLD << "error" << RESET_COLOR << ": incorrect number of arguments\n\n";
        std::cout << "usage: " << argv[0] << " filename\n";
        return 0;
    }
    std::string_view file_path = argv[1];
    std::ifstream file(file_path);
    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    Scanner scanner(code);
    std::vector<Token> tokens = scanner.scan_tokens();
    // for (int i = 0; i < tokens.size(); i++)
    // {
    //     std::cout << tokens[i].to_string() << "\n";
    // }
    Parser parser{tokens};
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();

    init_llvm_cfg();
    Codegen generator;
    generator.generate(statements);

    auto &llvm_module = get_llvm_cfg_module();
    llvm_module->print(llvm::errs(), nullptr);

    return 0;
}
