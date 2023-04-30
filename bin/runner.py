

f = open('___', 'r')

s = f.readlines()

# print(s)

fins = ''

curaddr = ''

for q in s:
    if q[0] == '.':
        if q[1] != ':':
            curaddr = q[2:-2]
        else:
            curaddr = q[1:-2]
    else:
        if q[0] in 'xwr-'


f.close()





