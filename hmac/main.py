from hmac import HMAC
import base64
import time


def send_message(message):
    for i in range(3, 0, -1):
        print('Sending message:', i)
        time.sleep(0.5)
    
    print('Message received')
    return message


def main():
    key = 'secretkey'

    msg = input('Enter message: ')
    hmac = HMAC(key.encode('utf-8'), msg.encode('utf-8'))

    hashed = base64.b64encode(hmac.digest())
    print('Hashed:', hashed)

    got = send_message(msg)

    check_hmac = HMAC(key.encode('utf-8'), got.encode('utf-8'))
    got_hashed = base64.b64encode(check_hmac.digest())

    print('Is not corruped:', got_hashed == hashed)


if __name__ == "__main__":
    main()
