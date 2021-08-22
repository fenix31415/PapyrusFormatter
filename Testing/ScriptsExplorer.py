import glob
import os
from random import randrange

rootdir = 'c:\\Games\\modding\\PapyrusFormatter\\Testing\\'
scripts = 'c:\\Games\\modding\\Data\\SCRIPTS\\scripts\\source\\'

def get_i_scrname():
    fs = glob.glob("*.psc")
    i = randrange(49015)
    while os.stat(fs[i]).st_size < 2000:
        i = randrange(49015)
    return fs[i]


def main():
    os.chdir(scripts)
    scr = get_i_scrname()
    name = rootdir + "sample.psc"
    os.system(f'{rootdir}PapyrusFormatter.exe "{scr}" "{name}"')


main()
