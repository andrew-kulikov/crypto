import random 


MAX_NUM = 1000


def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a


def is_prime(n):
    if n <= 1:
        return False
    for i in range(2, n):
        if n % i == 0:
            return False
    return True


def primitive_root(modulo):
    required_set = set(num for num in range(1, modulo) if gcd(num, modulo) == 1)
    for g in range(1, modulo):
        actual_set = set(pow(g, powers) % modulo for powers in range(1, modulo))
        if required_set == actual_set:
            return g


def generate_prime_number():
    num = random.randint(500, MAX_NUM)
    while not is_prime(num):
        num = random.randint(500, MAX_NUM)
    return num
