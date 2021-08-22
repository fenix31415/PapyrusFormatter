import glob
from distutils.dir_util import copy_tree

def copyfolder(src, dst):
    #print("   ", src)
    #copy_tree(src, dst)
    pass


def main(src, dst):
    fs = glob.glob(src + '*')
    for f in fs:
        fs1 = glob.glob(f + '\\*')
        if len(fs1) == 0:
            print(f)
        for f1 in fs1:
            if f1.lower() == f.lower() + '\\scripts':
                copyfolder(f1, dst + '\\scripts')
            if f1.lower() == f.lower() + '\\source':
                copyfolder(f1, dst + '\\source')


main('d:\\Games\\SSE\\LastDark\\mods\\', 'c:\\Games\\modding\\Data\\SCRIPTS\\tmp\\')
