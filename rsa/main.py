import time
import rsa
import base64

poolsize = 8
accurate = True

def main():
    public, private = rsa.newkeys(2048, accurate=accurate, poolsize=poolsize)
    
    encrypred = rsa.encrypt(b'encrypt me pls', public)
    decrypted = rsa.decrypt(encrypred, private)
    
    print(str(public._save_pkcs1_pem(), encoding='utf-8'))
    print(str(private._save_pkcs1_pem(), encoding='utf-8'))

    print(base64.standard_b64encode(encrypred))
    print()
    print(decrypted)

if __name__ == '__main__':
    main()
