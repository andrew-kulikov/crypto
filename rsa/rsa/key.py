import typing

import rsa.prime
import rsa.pem
import rsa.common
import rsa.randnum
import rsa.core


DEFAULT_EXPONENT = 65537


class AbstractKey:
    __slots__ = ('n', 'e')

    def __init__(self, n: int, e: int) -> None:
        self.n = n
        self.e = e

    @classmethod
    def _load_pkcs1_pem(cls, keyfile: bytes) -> 'AbstractKey':
        pass

    @classmethod
    def _load_pkcs1_der(cls, keyfile: bytes) -> 'AbstractKey':
        pass

    def _save_pkcs1_pem(self) -> bytes:
        pass

    def _save_pkcs1_der(self) -> bytes:
        pass

    @classmethod
    def load_pkcs1(cls, keyfile: bytes, format='PEM') -> 'AbstractKey':
        methods = {
            'PEM': cls._load_pkcs1_pem,
            'DER': cls._load_pkcs1_der,
        }

        method = cls._assert_format_exists(format, methods)
        return method(keyfile)

    @staticmethod
    def _assert_format_exists(file_format: str, methods: typing.Mapping[str, typing.Callable]) \
            -> typing.Callable:
        try:
            return methods[file_format]
        except KeyError:
            formats = ', '.join(sorted(methods.keys()))
            raise ValueError('Unsupported format: %r, try one of %s' % (file_format,
                                                                        formats))

    def save_pkcs1(self, format='PEM') -> bytes:
        methods = {
            'PEM': self._save_pkcs1_pem,
            'DER': self._save_pkcs1_der,
        }

        method = self._assert_format_exists(format, methods)
        return method()


class PublicKey(AbstractKey):
    __slots__ = ('n', 'e')

    @classmethod
    def _load_pkcs1_der(cls, keyfile: bytes) -> 'PublicKey':
        """Loads a key in PKCS#1 DER format.

        :param keyfile: contents of a DER-encoded file that contains the public
            key.
        :return: a PublicKey object

        First let's construct a DER encoded key:

        >>> import base64
        >>> b64der = 'MAwCBQCNGmYtAgMBAAE='
        >>> der = base64.standard_b64decode(b64der)

        This loads the file:

        >>> PublicKey._load_pkcs1_der(der)
        PublicKey(2367317549, 65537)

        """

        from pyasn1.codec.der import decoder
        from rsa.asn1 import AsnPubKey

        (priv, _) = decoder.decode(keyfile, asn1Spec=AsnPubKey())
        return cls(n=int(priv['modulus']), e=int(priv['publicExponent']))

    def _save_pkcs1_der(self) -> bytes:
        from pyasn1.codec.der import encoder
        from rsa.asn1 import AsnPubKey

        # Create the ASN object
        asn_key = AsnPubKey()
        asn_key.setComponentByName('modulus', self.n)
        asn_key.setComponentByName('publicExponent', self.e)

        return encoder.encode(asn_key)

    @classmethod
    def _load_pkcs1_pem(cls, keyfile: bytes) -> 'PublicKey':
        """Loads a PKCS#1 PEM-encoded public key file.

        The contents of the file before the "-----BEGIN RSA PUBLIC KEY-----" and
        after the "-----END RSA PUBLIC KEY-----" lines is ignored.

        :param keyfile: contents of a PEM-encoded file that contains the public
            key.
        :return: a PublicKey object
        """

        der = rsa.pem.load_pem(keyfile, 'RSA PUBLIC KEY')
        return cls._load_pkcs1_der(der)

    def _save_pkcs1_pem(self) -> bytes:
        der = self._save_pkcs1_der()
        return rsa.pem.save_pem(der, 'RSA PUBLIC KEY')


class PrivateKey(AbstractKey):
    __slots__ = ('n', 'e', 'd', 'p', 'q', 'exp1', 'exp2', 'coef')

    def __init__(self, n: int, e: int, d: int, p: int, q: int) -> None:
        AbstractKey.__init__(self, n, e)
        self.d = d
        self.p = p
        self.q = q

        # Calculate exponents and coefficient.
        self.exp1 = int(d % (p - 1))
        self.exp2 = int(d % (q - 1))
        self.coef = rsa.common.inverse(q, p)

    def blinded_decrypt(self, encrypted: int) -> int:
        return rsa.core.decrypt_int(encrypted, self.d, self.n)

    def blinded_encrypt(self, message: int) -> int:
        return rsa.core.encrypt_int(message, self.d, self.n)

    @classmethod
    def _load_pkcs1_der(cls, keyfile: bytes) -> 'PrivateKey':
        from pyasn1.codec.der import decoder
        (priv, _) = decoder.decode(keyfile)

        if priv[0] != 0:
            raise ValueError('Unable to read this file, version %s != 0' % priv[0])

        as_ints = map(int, priv[1:6])
        key = cls(*as_ints)

        return key

    def _save_pkcs1_der(self) -> bytes:
        from pyasn1.type import univ, namedtype
        from pyasn1.codec.der import encoder

        class AsnPrivKey(univ.Sequence):
            componentType = namedtype.NamedTypes(
                namedtype.NamedType('version', univ.Integer()),
                namedtype.NamedType('modulus', univ.Integer()),
                namedtype.NamedType('publicExponent', univ.Integer()),
                namedtype.NamedType('privateExponent', univ.Integer()),
                namedtype.NamedType('prime1', univ.Integer()),
                namedtype.NamedType('prime2', univ.Integer()),
                namedtype.NamedType('exponent1', univ.Integer()),
                namedtype.NamedType('exponent2', univ.Integer()),
                namedtype.NamedType('coefficient', univ.Integer()),
            )

        # Create the ASN object
        asn_key = AsnPrivKey()
        asn_key.setComponentByName('version', 0)
        asn_key.setComponentByName('modulus', self.n)
        asn_key.setComponentByName('publicExponent', self.e)
        asn_key.setComponentByName('privateExponent', self.d)
        asn_key.setComponentByName('prime1', self.p)
        asn_key.setComponentByName('prime2', self.q)
        asn_key.setComponentByName('exponent1', self.exp1)
        asn_key.setComponentByName('exponent2', self.exp2)
        asn_key.setComponentByName('coefficient', self.coef)

        return encoder.encode(asn_key)

    @classmethod
    def _load_pkcs1_pem(cls, keyfile: bytes) -> 'PrivateKey':
        der = rsa.pem.load_pem(keyfile, b'RSA PRIVATE KEY')
        return cls._load_pkcs1_der(der)

    def _save_pkcs1_pem(self) -> bytes:
        der = self._save_pkcs1_der()
        return rsa.pem.save_pem(der, b'RSA PRIVATE KEY')


def find_p_q(nbits: int, getprime_func=rsa.prime.getprime, accurate=True) -> typing.Tuple[int, int]:
    total_bits = nbits * 2

    # Make sure that p and q aren't too close or the factoring programs can
    # factor n.
    shift = nbits // 16
    pbits = nbits + shift
    qbits = nbits - shift

    # Choose the two initial primes

    p = getprime_func(pbits)
    q = getprime_func(qbits)

    def is_acceptable(p, q):
        if p == q: return False

        if not accurate: return True

        # Make sure we have just the right amount of bits
        found_size = rsa.common.bit_size(p * q)
        return total_bits == found_size

    # Keep choosing other primes until they match our requirements.
    change_p = False
    while not is_acceptable(p, q):
        # Change p on one iteration and q on the other
        if change_p: p = getprime_func(pbits)
        else: q = getprime_func(qbits)

        change_p = not change_p

    # We want p > q as described on
    # http://www.di-mgt.com.au/rsa_alg.html#crt
    return max(p, q), min(p, q)


def calculate_keys_custom_exponent(p: int, q: int, exponent: int) -> typing.Tuple[int, int]:
    """Calculates an encryption and a decryption key given p, q and an exponent,
    and returns them as a tuple (e, d)

    :param p: the first large prime
    :param q: the second large prime
    :param exponent: the exponent for the key; only change this if you know
        what you're doing, as the exponent influences how difficult your
        private key can be cracked. A very common choice for e is 65537.
    :type exponent: int

    """

    phi_n = (p - 1) * (q - 1)

    try:
        d = rsa.common.inverse(exponent, phi_n)
    except rsa.common.NotRelativePrimeError as ex:
        raise rsa.common.NotRelativePrimeError(
            exponent, phi_n, ex.d,
            msg="e (%d) and phi_n (%d) are not relatively prime (divider=%i)" %
                (exponent, phi_n, ex.d))

    if (exponent * d) % phi_n != 1:
        raise ValueError("e (%d) and d (%d) are not mult. inv. modulo "
                         "phi_n (%d)" % (exponent, d, phi_n))

    return exponent, d


def gen_keys(nbits: int,
             getprime_func: typing.Callable[[int], int],
             accurate=True,
             exponent=DEFAULT_EXPONENT) -> typing.Tuple[int, int, int, int]:
    # Regenerate p and q values, until calculate_keys doesn't raise a
    # ValueError.
    while True:
        (p, q) = find_p_q(nbits // 2, getprime_func, accurate)
        try:
            (e, d) = calculate_keys_custom_exponent(p, q, exponent=exponent)
            break
        except ValueError:
            pass

    return p, q, e, d


def newkeys(nbits: int, accurate=True, poolsize=1, exponent=DEFAULT_EXPONENT) \
        -> typing.Tuple[PublicKey, PrivateKey]:

    if nbits < 16:
        raise ValueError('Key too small')

    if poolsize < 1:
        raise ValueError('Pool size (%i) should be >= 1' % poolsize)

    # Determine which getprime function to use
    if poolsize > 1:
        from rsa import parallel

        def getprime_func(nbits):
            return parallel.getprime(nbits, poolsize=poolsize)
    else:
        getprime_func = rsa.prime.getprime

    (p, q, e, d) = gen_keys(nbits, getprime_func, accurate=accurate, exponent=exponent)
    n = p * q

    return (
        PublicKey(n, e),
        PrivateKey(n, e, d, p, q)
    )


__all__ = ['PublicKey', 'PrivateKey', 'newkeys']
