#include "des.h"
#include "utils.h"
#include "constants.h"


vector<bool> des::extend_key(const vector<bool>& key_bits)
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


vector<vector<bool>> des::generate_keys(const string& key) const
{
	const auto size = 56;

	const vector<char> bytes(key.begin(), key.end());
	const auto bits = get_bits(bytes);
	const vector<bool> key_bits(bits.begin(), bits.begin() + size);

	auto key_ext = extend_key(key_bits);
	key_ext = permutate(key_ext, kp1);

	vector<bool> c(key_ext.begin(), key_ext.begin() + size / 2);
	vector<bool> d(key_ext.begin() + size / 2, key_ext.end());
	
	vector<vector<bool>> keys;

	for (auto i = 0; i < 16; i++)
	{
		rotate(c.begin(), c.begin() + rt[i], c.end());
		rotate(d.begin(), d.begin() + rt[i], d.end());

		vector<bool> cur_key;
		cur_key.insert(cur_key.end(), c.begin(), c.end());
		cur_key.insert(cur_key.end(), d.begin(), d.end());

		cur_key = permutate(cur_key, kp2);

		keys.push_back(cur_key);
	}

	return keys;
}

vector<bool> des::feistel(const vector<bool>& r, const vector<bool>& key) const
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

vector<bool> des::encrypt_block(const vector<bool>& block, const vector<vector<bool>>& keys, const bool& decrypt) const
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

	res.insert(res.end(), r.begin(), r.end());
	res.insert(res.end(), l.begin(), l.end());

	res = permutate(res, fp);

	return res;
}

des::des()
{
	keys_ = generate_keys(default_key_);
}

string des::encrypt(const string& message, const bool& decrypt) const
{
	const vector<char> bytes(message.begin(), message.end());
	const auto bits = get_bits(bytes);
	auto blocks = get_blocks(bits);

	vector<vector<bool>> encrypted_blocks;
	encrypted_blocks.reserve(blocks.size());
	for (const auto& block : blocks) encrypted_blocks.push_back(encrypt_block(block, keys_, decrypt));

	return restore_message(encrypted_blocks);
}

des::des(const string& base_key)
{
	keys_ = generate_keys(base_key);
}

string des::encrypt(const string& message) const
{
	return encrypt(message, false);
}

string des::decrypt(const string& message) const
{
	return encrypt(message, true);
}
