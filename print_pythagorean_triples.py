#!/usr/bin/python3
'''
功能：打印1到10000以内所有的勾股数
'''

#求最大公约数，最大公约数为1的2个数互质。
def gcd(x1, x2):
    if x2 == 0:
        return x1
    return gcd(x2, x1 % x2)

if __name__ == "__main__":
    for m in range(2, 101):
        for n in range(m - 1, 0, -1):
            a = m * m - n * n
            b = 2 * m * n
            c = m * m + n * n
            if gcd(a, b) == 1:
                print(str(a) + ', ' + str(b) + ', ' + str(c))