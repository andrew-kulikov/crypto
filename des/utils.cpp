#include <algorithm>
#include <vector>
#include <string>
#include "utils.h"
#include <iostream>

using namespace std;

int to_int(const vector<bool>& bits)
{
	unsigned int res = 0;
	const unsigned int len = bits.size();

	for (unsigned int i = 0; i < len; i++) res += bits[i] << (len - i - 1);

	return res;
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
		vector<bool> tmp;
		tmp.reserve(8);
		for (auto i = 0; i < 8; i++)
			tmp.push_back((c >> i) & 1);

		reverse(tmp.begin(), tmp.end());
		res.insert(res.end(), tmp.begin(), tmp.end());
	}

	return res;
}

vector<bool> get_bits(const int val, const int size)
{
	vector<bool> res;

	res.reserve(size);
	for (auto i = 0; i < size; i++) res.push_back((val >> i) & 1);
	reverse(res.begin(), res.end());

	return res;
}

vector<vector<bool>> get_blocks(vector<bool> bits)
{
	vector<vector<bool>> res;
	vector<bool> batch;
	const int n = bits.size();
	for (auto i = 0; i < n; i++)
	{
		batch.push_back(bits[i]);

		if ((i + 1) % 64 == 0)
		{
			res.push_back(batch);
			batch.clear();
		}
	}

	if (!batch.empty())
	{
		while (batch.size() < 64) batch.push_back(false);
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