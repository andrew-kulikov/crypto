#include <iostream>
#include  "gost.h"
#include "utils.h"

int main()
{
	gost g(hex_to_str("E9DEE72C8F0C0FA62DDB49F46F73964706075316ED247A3739CBA38303A98BF6"));

	const auto src = hex_to_str("B194BAC80A08F53B366D008E584A5DE41");
	const auto encrypted = g.encrypt(src);
	const auto decrypted = g.decrypt(encrypted);

	cout << "source: " << src << endl;
	cout << "encrypted: " << encrypted << endl;
	cout << "decrypted: " << decrypted << endl;

	cout << "source: ";
	const vector<char> bytes(src.begin(), src.end());
	print_hex(get_bits(bytes));
	cout << "encrypted: ";
	const vector<char> bytes1(encrypted.begin(), encrypted.end());
	print_hex(get_bits(bytes1));
	cout << "decrypted: ";
	const vector<char> bytes2(decrypted.begin(), decrypted.end());
	print_hex(get_bits(bytes2));
}
