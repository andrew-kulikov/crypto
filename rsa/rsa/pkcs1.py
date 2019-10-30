import hashlib
import os
import sys
import typing

from . import common, transform, core, key

class DecryptionError(BaseException):
    """Raised when decryption fails."""

def _pad_for_encryption(message: bytes, target_length: int) -> bytes:
    r"""Pads the message for encryption, returning the padded message.

    :return: 00 02 RANDOM_DATA 00 MESSAGE

    >>> block = _pad_for_encryption(b'hello', 16)
    >>> len(block)
    16
    >>> block[0:2]
    b'\x00\x02'
    >>> block[-6:]
    b'\x00hello'

    """

    max_msglength = target_length - 11
    msglength = len(message)

    if msglength > max_msglength:
        raise OverflowError('%i bytes needed for message, but there is only'
                            ' space for %i' % (msglength, max_msglength))

    # Get random padding
    padding = b''
    padding_length = target_length - msglength - 3

    # We remove 0-bytes, so we'll end up with less padding than we've asked for,
    # so keep adding data until we're at the correct length.
    while len(padding) < padding_length:
        needed_bytes = padding_length - len(padding)

        # Always read at least 8 bytes more than we need, and trim off the rest
        # after removing the 0-bytes. This increases the chance of getting
        # enough bytes, especially when needed_bytes is small
        new_padding = os.urandom(needed_bytes + 5)
        new_padding = new_padding.replace(b'\x00', b'')
        padding = padding + new_padding[:needed_bytes]

    assert len(padding) == padding_length

    return b''.join([b'\x00\x02',
                     padding,
                     b'\x00',
                     message])


def encrypt(message: bytes, pub_key: key.PublicKey):
    keylength = common.byte_size(pub_key.n)
    padded = _pad_for_encryption(message, keylength)
    payload = transform.bytes2int(padded)
    encrypted = core.encrypt_int(payload, pub_key.e, pub_key.n)
    block = transform.int2bytes(encrypted, keylength)
    return block


def decrypt(crypto: bytes, priv_key: key.PrivateKey) -> bytes:
    blocksize = common.byte_size(priv_key.n)
    encrypted = transform.bytes2int(crypto)
    decrypted = priv_key.blinded_decrypt(encrypted)
    cleartext = transform.int2bytes(decrypted, blocksize)

    # If we can't find the cleartext marker, decryption failed.
    if cleartext[0:2] != b'\x00\x02':
        raise DecryptionError('Decryption failed')

    # Find the 00 separator between the padding and the message
    try:
        sep_idx = cleartext.index(b'\x00', 2)
    except ValueError:
        raise DecryptionError('Decryption failed')

    return cleartext[sep_idx + 1:]


__all__ = ['encrypt', 'decrypt', 'DecryptionError']
