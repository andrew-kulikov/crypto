import time
import rsa
import base64

poolsize = 8
accurate = True

def main():
    public, private = rsa.newkeys(2048, accurate=accurate, poolsize=poolsize)
    
    message = 'Another one message'
    print('Исходное сообщение:', message)
    encrypred = rsa.encrypt(message.encode('utf-8'), public)
    decrypted = rsa.decrypt(encrypred, private)
    
    print(str(public._save_pkcs1_pem(), encoding='utf-8'))
    print(str(private._save_pkcs1_pem(), encoding='utf-8'))

    print(base64.standard_b64encode(encrypred))
    print()
    print('Расшифровано:', decrypted)

if __name__ == '__main__':
    main()
