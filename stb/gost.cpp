#include "gost.h"
#include "utils.h"
#include "constants.h"


vector<bool> gost::h_permutation(vector<bool> u) const
{
	const auto y = (u[0] << 3) + (u[1] << 2) + (u[2] << 1) + u[3];
	const auto x = (u[4] << 3) + (u[5] << 2) + (u[6] << 1) + u[7];

	return get_bits(H[y][x], 8);
}

vector<bool> gost::g(const vector<bool>& u, int r) const
{
	const auto block_size = 8;
	const auto len = u.size();
	
	vector<bool> u_new;
	u_new.reserve(len);

	for (auto i = 0; i < 4; i++)
	{
		vector<bool> cur_u(u.begin() + block_size * i, u.begin() + block_size * (i + 1));
		cur_u = h_permutation(cur_u);

		u_new.insert(u_new.end(), cur_u.begin(), cur_u.end());
	}

	rotate(u_new.begin(), u_new.begin() + r, u_new.end());
	
	return u_new;
}

vector<vector<bool>> gost::generate_keys(const string& key) const
{
	const auto size = 256;
	const auto block_size = 32;

	const vector<char> bytes(key.begin(), key.end());
	const auto bits = get_bits(bytes);
	const vector<bool> key_bits(bits.begin(), bits.begin() + size);

	vector<vector<bool>> keys;

	for (auto i = 0; i < 7; i++)
		for (auto j = 0; j < 8; j++)
			keys.emplace_back(key_bits.begin() + block_size * j, key_bits.begin() + block_size * (j + 1));

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
	const auto size = block.size();
	const auto part_size = 32;
	
	vector<bool> res;
	
	vector<bool> a(block.begin(), block.begin() + part_size);
	vector<bool> b(block.begin() + part_size, block.begin() + part_size * 2);
	vector<bool> c(block.begin() + part_size * 2, block.begin() + part_size * 3);
	vector<bool> d(block.begin() + part_size * 3, block.begin() + part_size * 4);
	vector<bool> e;

	for (auto i = 0; i < 8; i++)
	{
		b = xor_v(b, g(plus_vec(a, keys[7 * i], 32), 5));
		c = xor_v(c, g(plus_vec(d, keys[7 * i + 1], 32), 21));
		a = minus_vec(a, g(plus_vec(b, keys[7 * i + 2], 32), 13), 32);
	}

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
