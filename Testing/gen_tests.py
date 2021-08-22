import os.path

def main(l, r):
    for i in range(l, r+1):
        name_test = f'{i:03}_test.psc'
        name_answ = f'{i:03}_ans.psc'
        
        if os.path.isfile(name_test) and os.stat(name_test).st_size > 0 or os.path.isfile(name_answ) and os.stat(name_answ).st_size > 0:
            print("WARN:", name_test, name_answ)
            continue
        
        with open(name_test, "w", encoding='utf-8') as out:
            pass
        
        with open(name_answ, "w", encoding='utf-8') as out:
            pass

main(1, 100)
