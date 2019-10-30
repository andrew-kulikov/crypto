import multiprocessing as mp

import rsa.prime
import rsa.randnum


def _find_prime(nbits: int, pipe) -> None:
    while True:
        integer = rsa.randnum.read_random_odd_int(nbits)

        if rsa.prime.is_prime(integer):
            pipe.send(integer)
            return


def getprime(nbits: int, poolsize: int) -> int:
    (pipe_recv, pipe_send) = mp.Pipe(duplex=False)

    # Create processes
    try:
        procs = [mp.Process(target=_find_prime, args=(nbits, pipe_send))
                 for _ in range(poolsize)]
        # Start processes
        for p in procs:
            p.start()

        result = pipe_recv.recv()
    finally:
        pipe_recv.close()
        pipe_send.close()

    # Terminate processes
    for p in procs:
        p.terminate()

    return result


__all__ = ['getprime']
