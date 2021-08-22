#include <iostream>
#include <string>

//#define PIPE_OUT
//#define ONLY_NOKS

int test(int i) {
    std::string test = std::to_string(i);
    if (test.size() == 1)
        test = "00" + test;
    if (test.size() == 2)
        test = "0" + test;
    std::string ft_out = test + "_out.txt";
    std::string inp = test + "_test.psc";
    std::string rigans = test + "_ans.psc";
    std::string myans_name = test + "_myans.psc";
    std::string diff_name = test + "_diff.txt";
#ifdef PIPE_OUT
    std::string ft_command = "PapyrusFormatter " + inp + " " + myans_name + ">" + ft_out;
#else
    std::string ft_command = "PapyrusFormatter " + inp + " " + myans_name;
#endif // PIPE_OUT

    int errcode = system(ft_command.c_str());
    if (errcode) {
        if (errcode == 1)
            std::cout << "[SYNTAX] " << inp << std::endl;
        else
            std::cout << "[CRASH] " << inp << std::endl;
        return false;
    }

    std::string fc_command = "fc " + rigans + " " + myans_name + ">" + diff_name;
    bool ans = system(fc_command.c_str()) == 0;
    if (ans) {
        std::string delmyans_command = "del " + myans_name;
        std::string deldiff_command = "del " + diff_name;

#ifdef PIPE_OUT
        std::string delout_command = "del " + ft_out;
        system(delout_command.c_str());
#endif // PIPE_OUT

        system(deldiff_command.c_str());
        system(delmyans_command.c_str());

#ifndef ONLY_NOKS
        std::cout << "[OK] " << inp << std::endl;
#endif // !ONLY_NOKS

    }
    else {
        std::cout << "!!!! " << inp << std::endl;
    }
    return ans;
}

void test_all() {
    const size_t MAX_TEST_IND = 43;
    int succ = 0;
    for (size_t i = MAX_TEST_IND; i > 0; --i)
    {
        succ += test(i);
    }
    std::cout << "\n" << succ << "/" << MAX_TEST_IND << std::endl;
    if (succ == MAX_TEST_IND)
        std::cout << "OK!" << std::endl;
    else
        std::cout << "Lox" << std::endl;
}

#define ALL_TESTS

int main()
{
#ifdef ALL_TESTS
    test_all();
#else
    test(30);
#endif // ALL_TESTS

}
