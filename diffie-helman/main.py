import random
from ec import ECPoint, EllipticCurve
from hellman import DiffieHellman

EC_A = 6
EC_B = 213
EC_P = 17
EC_POINT_COORD = 18


def main():
    curve = EllipticCurve(EC_A, EC_B, EC_P)
    base_point = curve.create_point(EC_POINT_COORD)

    diffie = DiffieHellman(base_point)

    private_key = random.randint(1, 1000)
    public_key = diffie.get_public_key(private_key)

    q = random.randint(1, 100)
    to_encrypt = base_point * 2
    encrypted_points = diffie.encrypt(to_encrypt, public_key, q)
    decrypted = diffie.decrypt(encrypted_points, private_key)

    print('Base eliptic curve point: ', to_encrypt)
    print('Encrypted: ', encrypted_points[0], encrypted_points[1])
    print('Decryped: ', decrypted)

    if decrypted == to_encrypt:
        print('Decrypted successfully')
    else:
        print('Something went wrong')


if __name__ == "__main__":
    main()
