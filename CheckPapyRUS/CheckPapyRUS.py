import glob
import os
from shutil import copyfile
import ntpath
import sys

scripts = 'c:\\Games\\modding\\Data\\SCRIPTS\\scripts\\source\\'
#scripts = 'c:\\Games\\modding\\PapyrusFormatter\\Testing\\SCRIPTS\\'
out_dir = 'c:\\Games\\modding\\PapyrusFormatter\\Testing\\out\\'

def test_one(scr):
    cmd = f'PapyrusFormatter "{scr}">nul'
    errno = os.system(cmd)
    if errno != 0:
        if errno == 1:
            print(f'[SYNTAX] {scr}')
        else:
            print(f'[CRASH] {scr} (sory)')
        return False
    else:
        return True


def main():
    scripts = sys.argv[1]
    out_dir = sys.argv[2]
    
    skipfirst = 0
    if len(sys.argv) == 4:
        skipfirst = int(sys.argv[3])
    
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    oks   = 0
    total = 0
    os.chdir(scripts)
    for scr in glob.glob(f'{scripts}/*.psc'):
        if skipfirst > 0:
            skipfirst -= 1
            continue
        
        total += 1
        if test_one(scr):
            oks += 1
        else:
            copyfile(scr, out_dir + ntpath.basename(scr))
        
        if total % 200 == 0:
            print(f'{oks}/{total}, cur: {scr}')
        
        if total - oks > 100:
            print("too many errs, terminating. btw, check out folder..")
            return
    
    print(f'\n{oks}/{total}');
    if (oks == total):
        print("OK!");
    else:
        print("Lox");


main()
