from struct import pack


def byte(num: int):
    """
    Converts a number between 0 and 255 (both inclusive) to a base-256 (byte)
    representation.
    """
    return pack("B", num)


def xor_bytes(b1: bytes, b2: bytes) -> bytes:
    return bytes(x ^ y for x, y in zip(b1, b2))
