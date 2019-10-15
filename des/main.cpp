#include <iostream>
#include "des.h"
#include "double_des.h"
#include "triple_des.h"


using namespace std;


int main()
{
	string message, key1, key2;

	/*cout << "Enter message: ";
	cin >> message;

	cout << "Enter key1: ";
	cin >> key1;

	cout << "Enter key2: ";
	cin >> key2;*/

	const des des("12345678");

	/*const double_des d_des(key1, key2);
	const triple_des t_des(key1, key2);*/

	const auto encrypted_message = des.encrypt("BSUIR135");
	const auto decrypted_message = des.decrypt(encrypted_message);

	/*
	const auto encrypted_message_d = d_des.encrypt(message);
	const auto decrypted_message_d = d_des.decrypt(encrypted_message_d);

	const auto encrypted_message_t = t_des.encrypt(message);
	const auto decrypted_message_t = t_des.decrypt(encrypted_message_t);*/

	cout << "--------------------------------------------" << endl;
	cout << "DES encrypted message: " << encrypted_message << endl;
	cout << "DES decrypted message: " << decrypted_message << endl;
	/*cout << "-----------------------------------------------" << endl;
	cout << "2DES encrypted message: " << encrypted_message_d << endl;
	cout << "2DES decrypted message: " << decrypted_message_d << endl;
	cout << "-----------------------------------------------" << endl;
	cout << "3DES encrypted message: " << encrypted_message_t << endl;
	cout << "3DES decrypted message: " << decrypted_message_t << endl;
	cout << "-----------------------------------------------" << endl;*/

	return 0;
}
