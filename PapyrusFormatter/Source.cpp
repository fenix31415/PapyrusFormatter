#include <iostream>
#include <filesystem>
#include "parser.tab.h"
#define EXPORT_TEST_FUNCTIONS
#include "Header.h"

namespace fs = std::filesystem;

const char* outname = nullptr;
void openinfile(const char* name);

void handle_files(const char* inp = nullptr, const char* out = nullptr) {
    openinfile(inp);
    outname = out;
}

int format_papyrus(const char* inp, const char* out = nullptr) {
    handle_files(inp, out);
    int ans = yyparse();
    return ans;
}

bool backup(const char* src) {
    try {
        return fs::copy_file(src, std::string(src) + "_back", fs::copy_options::overwrite_existing);
    }
    catch (fs::filesystem_error& e) {
        std::cerr << "Could not copy: " << e.what() << '\n';
        return false;
    }
}

int main_impl_dbg() {
    return format_papyrus("test.psc", "ans.psc");
    //return format_papyrus("test.psc");
}

int main_impl(int argc, char** argv) {
    if (argc == 2 && !strcmp(argv[1], "-h")) {
        std::cout << 
            "Usage: PapyrusFormatter [-b] [-i] [src [dst]]\n" <<
            "    -b  (backup) : if passed, create backup with name '<src>_back'\n" <<
            "    -i  (inplace): if passed, inplace change script (no need to pass 'dst')"
            "    src, dst     : input/output file's path. If unspecified, uses std input/output";
        return 0;
    }

    unsigned char ops = 0x0;
    const char* src = nullptr;
    const char* dst = nullptr;

    size_t ind = 1;
    if (argc > ind && !strcmp(argv[ind], "-b")) {
        ops |= 0x1;
        ++ind;
    }
    if (argc > ind && !strcmp(argv[ind], "-i")) {
        ops |= 0x2;
        ++ind;
    }
    if (argc > ind) src = argv[ind++];
    if (argc > ind) dst = argv[ind++];
    if (ops & 0x2) dst = src;
    if (ops & 0x1 && src && !backup(src)) {
        std::cerr << "Cant create backup, no formating";
        return 1;
    }
    
    return format_papyrus(src, dst);
}
