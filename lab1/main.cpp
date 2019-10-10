#include <iostream>
#include "des.h"
#include "double_des.h"
#include "triple_des.h"


using namespace std;


int main()
{
	string message, key1, key2;

	cout << "Enter message: ";
	cin >> message;

	cout << "Enter key1: ";
	cin >> key1;

	cout << "Enter key2: ";
	cin >> key2;

	const triple_des des(key1, key2);

	const auto encrypted_message = des.encrypt(message);
	const auto decrypted_message = des.decrypt(encrypted_message);

	cout << "source message: " << message << endl;
	cout << "encrypted message: " << encrypted_message << endl;
	cout << "decrypted message: " << decrypted_message << endl;

	return 0;
}
