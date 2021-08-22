import os, shutil
import glob
import sys
import ntpath
from filecmp import dircmp
from distutils.dir_util import copy_tree
from shutil import copyfile

fastmode = False
withcomtiletests = True
withcheckorigin  = False
verbose = True

break_errnos = []

formatted = 'formatted/'
rootdir = 'c:\\Games\\modding\\PapyrusFormatter\\Testing\\'
compiled = rootdir + 'compiled_tmp\\'

def rmrf(path):
    if not os.path.exists(path):
        return
    shutil.rmtree(path)


def trycompile(scr):
    cmd = f'PapyrusCompiler.exe {scr} -q -f=D:\\Games\\TESVSSE\\Data\\Source\\Scripts\\TESV_Papyrus_Flags.flg -i=d:\\Games\\TESVSSE\\Data\\src -o={compiled}'
    return os.system(cmd) == 0


'''
errno:
    0 - ok
    1 - format error
    2 - compile error
'''
def testscript_(scr):
    copy = scr + '_origin'
    copyfile(scr, copy)
    ok = os.system(f'{rootdir}PapyrusFormatter.exe "{copy}" "{scr}"') == 0
    
    ans = 0
    if not ok:
        print("  format error, skipin compiling")
        ans = 1
    else:
        if not fastmode and withcomtiletests:
            ok = trycompile(scr)
            ans = 0 if ok else 2
        else:
            ans = 0
    
    copyfile(copy, scr)
    os.remove(copy)
    return ans


def checkoriginscript(scr):
    cmd = f'PapyrusCompiler.exe {scr} -q -f=D:\\Games\\TESVSSE\\Data\\Source\\Scripts\\TESV_Papyrus_Flags.flg -i=c:\\Games\\modding\\PapyrusFormatter\\Testing\\src -o={compiled}'
    return os.system(cmd) == 0


def checkoriginscripts_fast(scripts_data, path):
    if fastmode or not withcheckorigin:
        return True
    
    for line in scripts_data:
        ok, folder = line.split(" ", 1)
        ok = bool(ok)
        if folder == path:
            return ok
    print(f'unknown path {path}')
    return False


'''
errno:
    012 testscript_
    3 - origin compile error
'''
def testscript(scripts_data, scr):
    if checkoriginscripts_fast(scripts_data, scr):
        return testscript_(scr)
    else:
        return 3


def repairorigins():
    for i in glob.glob("*.psc_origin"):
        copyfile(i, i[:-7])
        os.remove(i)
        


def test_all(scripts):
    oldcwd = os.getcwd()
    os.chdir(scripts)
    
    repairorigins()
    
    scripts_data = []
    if withcheckorigin:
        with open(rootdir + "compiling_info.txt", "r", encoding='utf-8') as inp:
            scripts_data = list(map(lambda x: x.rstrip(), inp.readlines()))
    
    total = 0
    oks = 0
    errno = 0
    skipfirst = 0
    skipfirst = 19205
    
    with open(rootdir + 'log.txt', "w", encoding='utf-8') as log:
        for i in glob.glob("*.psc"):
            if skipfirst > 0:
                skipfirst -= 1
                if skipfirst == 0:
                    print(f'skipin {i}')
                continue
            
            if not fastmode or total % 500 == 0:
                print(f'{oks}/{total}/48936: {i}')
            
            errno = testscript(scripts_data, i)
            if errno in break_errnos:
                print(f'[{errno}]: breakin...')
                break
            else:
                if errno == 3:
                    print(f'   broken, skipin: {i}')
                    log.write(f'   broken, skipin: {i}\n')
                elif errno == 0:
                    if not fastmode:
                        print('  OK!')
                    total += 1
                    oks += 1
                else:
                    print(f'  [ER] {"format" if errno == 1 else "compile"} error: {i}')
                    log.write(f'  [ER] {"format" if errno == 1 else "compile"} error: {i}\n')
    
    os.chdir(oldcwd)
    
    print(f'{"OK" if errno == 0 else ("interrupted" if errno in break_errnos else "error")}, {oks}/{total} scripts!')


def updateCompilingInfo(scripts):
    rmrf(compiled)
    os.mkdir(compiled)
    
    oldcwd = os.getcwd()
    os.chdir(scripts)
    with open("c:\\Games\\modding\\PapyrusFormatter\\Testing\\compiling_info.txt", "w", encoding='utf-8') as out:
        skipfirst = 0
        for scr in glob.glob("*.psc"):
            if skipfirst > 0:
                skipfirst -= 1
                print(f'skipin {scr}')
                continue
            
            ans = f'{checkoriginscript(scr)} {scr}'
            print(ans)
            out.write(ans + "\n")
    
    os.chdir(oldcwd)


def main():
    scripts = 'SCRIPTS\\scripts'
    scripts = 'c:\\Games\\modding\\Data\\SCRIPTS\\scripts\\source'
    
    #updateCompilingInfo(scripts)
    test_all(scripts)

main()
