#include <algorithm>
#include <vector>
#include <string>
#include "utils.h"
#include <iostream>
#include <sstream>

using namespace std;



vector<bool> rev(const vector<bool>& v)
{
	auto res = v;
	reverse(res.begin(), res.end());
	return res;
}

vector<bool> rev_bytes(const vector<bool>& v)
{
	vector<bool> res;
	const auto len = v.size();
	
	for (unsigned int i = 0; i < len; i += 8)
	{
		vector<bool> cur(v.begin() + i, v.begin() + i + 8);
		res.insert(res.begin(), cur.begin(), cur.end());
	}

	return res;
}

string hex_to_str(string hex)
{
	const auto final_len = hex.size() / 2;
	string res;
	for (size_t i = 0, j = 0; j < final_len; i += 2, j++) {
		const auto c = char((hex[i] % 32 + 9) % 25 * 16 + (hex[i + 1] % 32 + 9) % 25);
		res += c;
	}

	return res;
}

void print_hex(vector<bool> bits)
{
	const auto len = bits.size();
	for (unsigned int i = 0; i < len; i += 4)
	{
		vector<bool> cur(bits.begin() + i, bits.begin() + i + 4);
		cout << to_hex(to_int(cur));
	}
	cout << endl;
}

string to_hex(const int x)
{
	stringstream stream;
	stream << std::hex << x;

	return stream.str();
}

ll to_int(const vector<bool>& bits)
{
	ll res = 0;
	const auto len = bits.size();

	for (unsigned int i = 0; i < len; i++) res += ll(bits[i]) << (len - i - 1);

	return res;
}

vector<bool> plus_vec(const vector<bool>& a, const vector<bool>& b, const int mod)
{
	const auto size = a.size();
	
	const auto a_int = to_int(rev_bytes(a));
	const auto b_int = to_int(rev_bytes(b));
	const auto res_int = (a_int + b_int) % (1LL << mod);

	//cout << a_int << " " << b_int << endl;
	//cout << to_int(rev_bytes(a)) << " " << to_int(rev_bytes(b)) << endl;

	const auto res = get_bits(res_int, size);
	
	return rev_bytes(res);
}

vector<bool> minus_vec(const vector<bool>& a, const vector<bool>& b, const int mod)
{
	const auto size = a.size();

	const auto a_int = to_int(rev_bytes(a));
	const auto b_int = to_int(rev_bytes(b));
	const auto res_int = (a_int - b_int) % (1LL << mod);

	const auto res = get_bits(res_int, size);
	
	return rev_bytes(res);
}

vector<bool> permutate(const vector<bool>& block, const vector<int>& table)
{
	const auto len = table.size();
	vector<bool> res;

	res.reserve(len);
	for (unsigned int i = 0; i < len; i++) res.push_back(block[table[i]]);

	return res;
}

vector<bool> xor_v(vector<bool> a, vector<bool> b)
{
	const auto len = a.size();
	vector<bool> res;

	for (unsigned int i = 0; i < len; i++) res.push_back(a[i] ^ b[i]);

	return res;
}

vector<bool> get_bits(const vector<char>& text)
{
	vector<bool> res;
	for (auto c : text)
	{
		const int cur_c = c < 0 ? 256 + c : c;
		vector<bool> tmp;
		tmp.reserve(8);
		for (auto i = 0; i < 8; i++)
			tmp.push_back((cur_c >> i) & 1);

		reverse(tmp.begin(), tmp.end());
		res.insert(res.end(), tmp.begin(), tmp.end());
	}

	return res;
}

void swap_vec(vector<bool>& v1, vector<bool>& v2)
{
	const auto temp = v1;
	v1 = v2;
	v2 = temp;
}

vector<bool> get_bits(const ll val, const int size)
{
	vector<bool> res;

	res.reserve(size);
	for (auto i = 0; i < size; i++) res.push_back((val >> i) & 1);
	reverse(res.begin(), res.end());

	return res;
}

vector<vector<bool>> get_blocks(vector<bool> bits, const unsigned int block_size)
{
	vector<vector<bool>> res;
	vector<bool> batch;
	const int n = bits.size();
	for (auto i = 0; i < n; i++)
	{
		batch.push_back(bits[i]);

		if ((i + 1) % block_size == 0)
		{
			res.push_back(batch);
			batch.clear();
		}
	}

	if (!batch.empty())
	{
		while (batch.size() < block_size) batch.push_back(false);
		res.push_back(batch);
	}

	return res;
}

char restore_byte(const vector<bool>& bits)
{
	auto c = 0;
	const auto len = bits.size();

	for (unsigned int i = 0; i < len; i++) c += bits[i] << (len - i - 1);

	return static_cast<char>(c);
}

bool is_zero_vector(const vector<bool>& v)
{
	for (auto x : v) if (x) return false;
	return true;
}

string restore_block(const vector<bool>& bits)
{
	string res;
	const auto len = bits.size();

	for (unsigned int i = 0; i < len; i += 8)
	{
		vector<bool> byte(bits.begin() + i, bits.begin() + (i + 8));

		if (is_zero_vector(byte)) continue;
		res += restore_byte(byte);
	}

	return res;
}

string restore_message(const vector<vector<bool>>& blocks)
{
	string res;
	for (const auto& block : blocks) res += restore_block(block);

	return res;
}

void print(const vector<bool>& block)
{
	for (auto c : block) cout << c;
	cout << endl;
}

void print(const vector<vector<bool>>& message)
{
	for (const auto& block : message) print(block);
}