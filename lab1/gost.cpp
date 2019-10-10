#include "gost.h"
#include "utils.h"
#include "constants.h"


vector<bool> gost::extend_key(const vector<bool>& key_bits)
{
	const int len = key_bits.size();
	vector<bool> key_ext;

	for (auto i = 0, cur = 0; i < len; i++)
	{
		cur ^= key_bits[i];
		key_ext.push_back(key_bits[i]);

		if ((i + 1) % 7 == 0)
		{
			key_ext.push_back(!cur);
			cur = 0;
		}
	}

	return key_ext;
}


vector<vector<bool>> gost::generate_keys(const string& key) const
{
	const auto size = 256;
	const auto block_size = 32;

	const vector<char> bytes(key.begin(), key.end());
	const auto bits = get_bits(bytes);
	const vector<vector<bool>> key_bits(bits.begin(), bits.begin() + size);

	vector<vector<bool>> keys;

	for (auto i = 0; i < 7; i++)
		for (auto j = 0; j < 8; j++)
			keys.emplace_back(key_bits.begin() + j * block_size, key_bits.begin() + (j + 1) * block_size);

	return keys;
}

vector<bool> gost::feistel(const vector<bool>& r, const vector<bool>& key) const
{
	vector<bool> res;

	auto r_new = permutate(r, et);
	r_new = xor_v(r_new, key);

	const auto len = r_new.size();
	res.reserve(r.size());
	for (unsigned int i = 0, j = 0; i < len; i += 6, j++)
	{
		vector<bool> block(r_new.begin() + i, r_new.begin() + (i + 6));
		const auto y = (block[0] << 1) + block[5];
		const auto x = (block[1] << 3) + (block[2] << 2) + (block[3] << 1) + block[4];

		auto s_box_val_bits = get_bits(s_box[j][y][x], 4);
		res.insert(res.end(), s_box_val_bits.begin(), s_box_val_bits.end());
	}

	res = permutate(res, p);

	return res;
}

vector<bool> gost::encrypt_block(const vector<bool>& block, const vector<vector<bool>>& keys, const bool& decrypt) const
{
	vector<bool> res;
	auto src = permutate(block, ip);
	const auto size = src.size();

	vector<bool> l(src.begin(), src.begin() + size / 2);
	vector<bool> r(src.begin() + size / 2, src.end());

	auto start = 0, end = 16, inc = 1;
	if (decrypt) start = 15, end = -1, inc = -1;

	while (start != end)
	{
		auto new_l = r;
		auto new_r = feistel(r, keys[start]);

		new_r = xor_v(new_r, l);

		r = vector<bool>(new_r);
		l = vector<bool>(new_l);

		start += inc;
	}

	res.reserve(size);
	res.insert(res.end(), r.begin(), r.end());
	res.insert(res.end(), l.begin(), l.end());

	res = permutate(res, fp);

	return res;
}

gost::gost()
{
	keys_ = generate_keys(default_key_);
}

string gost::encrypt(const string& message, const bool& decrypt) const
{
	const vector<char> bytes(message.begin(), message.end());
	const auto bits = get_bits(bytes);
	auto blocks = get_blocks(bits);

	vector<vector<bool>> encrypted_blocks;
	encrypted_blocks.reserve(blocks.size());
	for (const auto& block : blocks) encrypted_blocks.push_back(encrypt_block(block, keys_, decrypt));

	return restore_message(encrypted_blocks);
}

gost::gost(const string& base_key)
{
	keys_ = generate_keys(base_key);
}

string gost::encrypt(const string& message) const
{
	return encrypt(message, false);
}

string gost::decrypt(const string& message) const
{
	return encrypt(message, true);
}
