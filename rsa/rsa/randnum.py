import os
import struct

from rsa import common, transform


def read_random_bits(nbits: int) -> bytes:
    nbytes, rbits = divmod(nbits, 8)

    randomdata = os.urandom(nbytes)

    # Add the remaining random bits
    if rbits > 0:
        randomvalue = ord(os.urandom(1))
        randomvalue >>= (8 - rbits)
        randomdata = struct.pack("B", randomvalue) + randomdata

    return randomdata


def read_random_int(nbits: int) -> int:
    randomdata = read_random_bits(nbits)
    value = transform.bytes2int(randomdata)

    # Ensure that the number is large enough to just fill out the required
    # number of bits.
    value |= 1 << (nbits - 1)

    return value


def read_random_odd_int(nbits: int) -> int:
    return read_random_int(nbits) | 1


def randint(maxvalue: int) -> int:
    bit_size = common.bit_size(maxvalue)

    tries = 0
    while True:
        value = read_random_int(bit_size)

        if value <= maxvalue: break

        if tries % 10 == 0 and tries: bit_size -= 1
        
        tries += 1

    return value
