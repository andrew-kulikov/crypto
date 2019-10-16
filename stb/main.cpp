#include <iostream>
#include  "gost.h"
#include "utils.h"

//task4 - blocks encrypt
void encrypt_blocks()
{
	cout << "blocks mode:" << endl;
	const auto key = hex_to_str("E9DEE72C8F0C0FA62DDB49F46F73964706075316ED247A3739CBA38303A98BF6");
	const auto message = hex_to_str("B194BAC80A08F53B366D008E584A5DE48504FA9D1BB6C7AC252E72C202FDCE0D5BE3D612");
	const auto sync = hex_to_str("BE32971343FC9A48A02A885F194B09A1");

	gost g(key);

	const auto encrypted = g.encrypt_blocks(message, sync);
	const auto decrypted = g.decrypt_blocks(encrypted, sync);

	cout << "source: ";
	const vector<char> bytes(message.begin(), message.end());
	print_hex(get_bits(bytes));
	cout << "encrypted: ";
	const vector<char> bytes1(encrypted.begin(), encrypted.end());
	print_hex(get_bits(bytes1));
	cout << "decrypted: ";
	const vector<char> bytes2(decrypted.begin(), decrypted.end());
	print_hex(get_bits(bytes2));
}

int main()
{
	//task1 - one block
	//const auto key = hex_to_str("E9DEE72C8F0C0FA62DDB49F46F73964706075316ED247A3739CBA38303A98BF6");
	//const auto message = hex_to_str("B194BAC80A08F53B366D008E584A5DE41");

	//task2 - simple encrypt
	const auto key = hex_to_str("E9DEE72C8F0C0FA62DDB49F46F73964706075316ED247A3739CBA38303A98BF6");
	const auto message = hex_to_str("B194BAC80A08F53B366D008E584A5DE48504FA9D1BB6C7AC252E72C202FDCE0D5BE3D61217B96181FE6786AD716B890B");

	//task3 - simple decrypt
	//const auto key = hex_to_str("92BD9B1CE5D141015445FBC95E4D0EF2682080AA227D642F2687F93490405511");
	//const auto message = hex_to_str("E12BDC1AE28257EC703FCCF095EE8DF1C1AB76389FE678CAF7C6F860D5BB9C4FF33C657B637C306ADD4EA7799EB23D31");

	
	//encrypt_blocks();
	//return 0;

	const gost g(key);

	const auto encrypted = g.encrypt_simple(message);
	const auto decrypted = g.decrypt_simple(encrypted);

	cout << "simple mode:" << endl;
	cout << "source: ";
	const vector<char> bytes(message.begin(), message.end());
	print_hex(get_bits(bytes));
	cout << "encrypted: ";
	const vector<char> bytes1(encrypted.begin(), encrypted.end());
	print_hex(get_bits(bytes1));
	cout << "decrypted: ";
	const vector<char> bytes2(decrypted.begin(), decrypted.end());
	print_hex(get_bits(bytes2));
}


