class DiffieHellman:
    def __init__(self, point):
        self.point = point
        self.n = point.order

    def get_public_key(self, private_key):
        return self.point * private_key

    def encrypt(self, data_point, public_key, q):
        return self.point * q, data_point + public_key * q

    def decrypt(self, encrypted_points, private_key):
        return encrypted_points[1] + -(encrypted_points[0] * private_key)
