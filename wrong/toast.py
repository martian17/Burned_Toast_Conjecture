def flip(n, array):
    sub = array[:n]
    for i in sub:
        i[1] = not i[1]
    sub.reverse()
    array[:n] = sub


def problem(x):
    array = [[i, True] for i in range(x)]
    
    itr = 0
    n = 1
    while True:
        itr +=1
        flip(n, array)
        if all(i[1] for i in array):
            last = -1
            to_ret = True
            for idx, i in enumerate(array):
                if idx != i[0]:
                    to_ret = False
                    break
            if to_ret:
                return itr

        n = (n+1) % x
        
def main():
    with open("ret.txt", "w") as ret:
        for i in range(2, 100):
            print(i,problem(i))
        
main()
