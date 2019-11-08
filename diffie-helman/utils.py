def mod_inverse(n, q):
    for i in range(q):
        if (n * i) % q == 1:
            return i


def mod_sqrt(n, q):
    for i in range(1, q):
        if pow(i, 2, q) == n:
            return i, q - i