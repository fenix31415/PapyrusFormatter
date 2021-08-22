import glob
import os
from shutil import copyfile

only_noks = False
scripts = 'c:\\Games\\modding\\PapyrusFormatter\\Testing\\'

def test_one(test):
    ft_out     = test + "_out.txt";
    inp        = test + "_test.psc";
    rigans     = test + "_ans.psc";
    myans_name = test + "_myans.psc";
    diff_name  = test + "_diff.txt";
    cmd = f'PapyrusFormatter {inp} {myans_name}>{ft_out}'
    errno = os.system(cmd)
    if errno != 0:
        if errno == 1:
            print(f'[SYNTAX] {inp}')
        else:
            print(f'[CRASH] {inp}')
        return False
    
    fc_command = f'fc {rigans} {myans_name}>{diff_name}'
    ans = os.system(fc_command) == 0
    if ans:
        delmyans_command = f'del {myans_name}'
        deldiff_command  = f'del {diff_name}'
        delout_command   = f'del {ft_out}'
        os.system(delout_command)
        os.system(deldiff_command)
        os.system(delmyans_command)

        if not only_noks:
            print(f'[OK] {inp}')
    else:
        print(f'!!!! {inp}')
    return ans


def main():
    os.chdir(scripts)
    copyfile("PapyrusFormatter.exe", "named_tests\\PapyrusFormatter.exe")
    oks = 0
    total = 0
    for test in glob.glob("named_tests\\*_test.psc"):
        total += 1
        if test_one(test[:-9]):
            oks += 1
    
    print(f'\n{oks}/{total}');
    if (oks == total):
        print("OK!");
    else:
        print("Lox");


main()
