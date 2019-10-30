import rsa.common
import rsa.randnum


def gcd(p: int, q: int) -> int:
    while q != 0:
        (p, q) = (q, p % q)
    return p


def get_primality_testing_rounds(number: int) -> int:
    bitsize = rsa.common.bit_size(number)

    if bitsize >= 1536: return 3
    if bitsize >= 1024: return 4
    if bitsize >= 512: return 7

    return 10


def miller_rabin_primality_testing(n: int, k: int) -> bool:
    # prevent potential infinite loop when d = 0
    if n < 2: return False

    # Decompose (n - 1) to write it as (2 ** r) * d
    # While d is even, divide it by 2 and increase the exponent.
    d = n - 1
    r = 0

    while not (d & 1):
        r += 1
        d >>= 1

    # Test k witnesses.
    for _ in range(k):
        # Generate random integer a, where 2 <= a <= (n - 2)
        a = rsa.randnum.randint(n - 3) + 1

        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue

        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == 1: return False
            if x == n - 1: break
        else: return False

    return True


def is_prime(number: int) -> bool:
    if number < 10:
        return number in {2, 3, 5, 7}

    # Check for even numbers.
    if not (number & 1):
        return False

    # Calculate minimum number of rounds.
    k = get_primality_testing_rounds(number)

    # Run primality testing with (minimum + 1) rounds.
    return miller_rabin_primality_testing(number, k + 1)


def getprime(nbits: int) -> int:
    assert nbits > 3  # the loop wil hang on too small numbers

    while True:
        integer = rsa.randnum.read_random_odd_int(nbits)

        if is_prime(integer):
            return integer


__all__ = ['getprime']
