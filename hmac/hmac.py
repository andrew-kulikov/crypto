from sha256 import sha256


trans_5C = bytes((x ^ 0x5C) for x in range(256))
trans_36 = bytes((x ^ 0x36) for x in range(256))

class HMAC:
    blocksize = 64

    def __init__(self, key, msg=None):
        self.outer = sha256('')
        self.inner = sha256('')

        self.digest_size = self.inner.digest_size
        self.block_size = self.inner.block_size

        if len(key) > self.block_size:
            key = sha256(key).digest()

        key = key.ljust(self.block_size, b'\0')

        print('key:', key)

        self.outer.update(key.translate(trans_5C))
        self.inner.update(key.translate(trans_36))

        self.update(msg)
        
    def update(self, msg):
        self.inner.update(msg)

    def _current(self):
        h = self.outer.copy()
        h.update(self.inner.digest())
        return h

    def digest(self):
        h = self._current()
        return h.digest()

    def hexdigest(self):
        h = self._current()
        return h.hexdigest()

