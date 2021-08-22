#include <iostream>
#include "parser.tab.h"
#define EXPORT_TEST_FUNCTIONS
#include "Header.h"

const char* outname = nullptr;
void openinfile(const char* name);
void closeintpile();

void handle_files(const char* inp = nullptr, const char* out = nullptr) {
    openinfile(inp);
    outname = out;
}

int format_papyrus(const char* inp, const char* out = nullptr) {
    handle_files(inp, out);
    int ans = yyparse();
    closeintpile();
    return ans;
}

int main_impl_dbg() {
    return format_papyrus("test.psc", "ans.psc");
    //return format_papyrus("test.psc");
}

int main_impl(int argc, char** argv) {
    if (argc == 2 && !strcmp(argv[1], "-h")) {
        std::cout << "Usage: PapyrusFormatter [input-file [output-file]]";
        return 0;
    }
    
    return format_papyrus(argc >= 2 ? argv[1] : nullptr, argc == 3 ? argv[2] : nullptr);
}
