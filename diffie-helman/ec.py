from utils import mod_inverse, mod_sqrt


class EllipticCurve:
    def __init__(self, a, b, p):
        assert (4 * a ** 3 + 27 * b ** 2) % p != 0
        self.a = a
        self.b = b
        self.p = p

    def create_point(self, x):
        ysq = (x ** 3 + self.a * x + self.b) % self.p
        _, my = mod_sqrt(ysq, self.p)
        return ECPoint(self, x, my)

    def __str__(self):
        return 'a = {}, b = {}, p = {}'.format(self.a, self.b, self.p)


class ECPoint:
    def __init__(self, curve, x, y):
        self.curve = curve
        self.x = x
        self.y = y

    @property
    def check_zero(self):
        return self.x == 0 and self.y == 0

    @property
    def order(self):
        for i in range(1, self.curve.p + 1):
            if (self * i).check_zero:
                return i

    def __neg__(self):
        return ECPoint(self.curve, self.x, -self.y % self.curve.p)

    def __add__(self, other):
        if (self.x, self.y) == (0, 0):
            return other
        if (other.x, other.y) == (0, 0):
            return ECPoint(self.curve, self.x, self.y)
        if self.x == other.x and (self.y != other.y or self.y == 0):
            return ECPoint(self.curve, 0, 0)

        if self.x == other.x:
            l = (3 * self.x ** 2 + self.curve.a) * \
                mod_inverse(2 * self.y, self.curve.p) % self.curve.p
        else:
            l = (other.y - self.y) * mod_inverse(other.x -
                                                 self.x, self.curve.p) % self.curve.p

        x = (l * l - self.x - other.x) % self.curve.p
        y = (l * (self.x - x) - self.y) % self.curve.p

        return ECPoint(self.curve, x, y)

    def __mul__(self, number):
        result = ECPoint(self.curve, 0, 0)
        tmp = ECPoint(self.curve, self.x, self.y)

        while 0 < number:
            if number & 1 == 1:
                result += tmp
            number, tmp = number >> 1, tmp + tmp
        return result

    def __eq__(self, other):
        return (self.curve, self.x, self.y) == (other.curve, other.x, other.y)

    def __str__(self):
        return 'Curve ({}): x = {}, y = {}'.format(self.curve, self.x, self.y)
