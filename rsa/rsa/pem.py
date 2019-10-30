import base64
import typing

# Should either be ASCII strings or bytes.
FlexiText = typing.Union[str, bytes]


def _markers(pem_marker: FlexiText) -> typing.Tuple[bytes, bytes]:
    if not isinstance(pem_marker, bytes):
        pem_marker = pem_marker.encode('ascii')

    return (b'-----BEGIN ' + pem_marker + b'-----',
            b'-----END ' + pem_marker + b'-----')


def _pem_lines(contents: bytes, pem_start: bytes, pem_end: bytes) -> typing.Iterator[bytes]:
    """Generator over PEM lines between pem_start and pem_end."""

    in_pem_part = False
    seen_pem_start = False

    for line in contents.splitlines():
        line = line.strip()

        # Skip empty lines
        if not line:
            continue

        # Handle start marker
        if line == pem_start:
            if in_pem_part:
                raise ValueError('Seen start marker "%s" twice' % pem_start)

            in_pem_part = True
            seen_pem_start = True
            continue

        # Skip stuff before first marker
        if not in_pem_part:
            continue

        # Handle end marker
        if in_pem_part and line == pem_end:
            in_pem_part = False
            break

        # Load fields
        if b':' in line:
            continue

        yield line

    # Do some sanity checks
    if not seen_pem_start:
        raise ValueError('No PEM start marker "%s" found' % pem_start)

    if in_pem_part:
        raise ValueError('No PEM end marker "%s" found' % pem_end)


def load_pem(contents: FlexiText, pem_marker: FlexiText) -> bytes:
    # We want bytes, not text. If it's text, it can be converted to ASCII bytes.
    if not isinstance(contents, bytes):
        contents = contents.encode('ascii')

    (pem_start, pem_end) = _markers(pem_marker)
    pem_lines = [line for line in _pem_lines(contents, pem_start, pem_end)]

    # Base64-decode the contents
    pem = b''.join(pem_lines)
    return base64.standard_b64decode(pem)


def save_pem(contents: bytes, pem_marker: FlexiText) -> bytes:
    (pem_start, pem_end) = _markers(pem_marker)

    b64 = base64.standard_b64encode(contents).replace(b'\n', b'')
    pem_lines = [pem_start]

    for block_start in range(0, len(b64), 64):
        block = b64[block_start:block_start + 64]
        pem_lines.append(block)

    pem_lines.append(pem_end)
    pem_lines.append(b'')

    return b'\n'.join(pem_lines)
