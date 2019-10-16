#include "gost.h"
#include "utils.h"
#include "constants.h"
#include <iostream>

vector<bool> gost::h_permutation(vector<bool> u) const
{
	const auto y = (u[0] << 3) + (u[1] << 2) + (u[2] << 1) + u[3];
	const auto x = (u[4] << 3) + (u[5] << 2) + (u[6] << 1) + u[7];

	return get_bits(H[y][x], 8);
}

vector<bool> gost::g(const vector<bool>& u, const int r) const
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

	u_new = rev_bytes(u_new);
	rotate(u_new.begin(), u_new.begin() + r, u_new.end());
	u_new = rev_bytes(u_new);

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


vector<bool> gost::encrypt_block(const vector<bool>& block, const vector<vector<bool>>& keys) const
{
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

		const auto i_vec = rev_bytes(get_bits(ll(i + 1) % (1LL << 32), 32));
		e = g(plus_vec(b, plus_vec(c, keys[7 * i + 3], 32), 32), 21);
		e = xor_v(e, i_vec);

		b = plus_vec(b, e, 32);
		c = minus_vec(c, e, 32);

		d = plus_vec(d, g(plus_vec(c, keys[7 * i + 4], 32), 13), 32);
		b = xor_v(b, g(plus_vec(a, keys[7 * i + 5], 32), 21));
		c = xor_v(c, g(plus_vec(d, keys[7 * i + 6], 32), 5));

		swap_vec(a, b);
		swap_vec(c, d);
		swap_vec(b, c);
	}

	res.insert(res.end(), b.begin(), b.end());
	res.insert(res.end(), d.begin(), d.end());
	res.insert(res.end(), a.begin(), a.end());
	res.insert(res.end(), c.begin(), c.end());
	
	return res;
}

vector<bool> gost::decrypt_block(const vector<bool>& block, const vector<vector<bool>>& keys) const
{
	const auto part_size = 32;

	vector<bool> res;

	vector<bool> a(block.begin(), block.begin() + part_size);
	vector<bool> b(block.begin() + part_size, block.begin() + part_size * 2);
	vector<bool> c(block.begin() + part_size * 2, block.begin() + part_size * 3);
	vector<bool> d(block.begin() + part_size * 3, block.begin() + part_size * 4);
	vector<bool> e;

	for (auto i = 8; i > 0; i--)
	{
		b = xor_v(b, g(plus_vec(a, keys[7 * i - 1], 32), 5));
		c = xor_v(c, g(plus_vec(d, keys[7 * i - 2], 32), 21));
		a = minus_vec(a, g(plus_vec(b, keys[7 * i - 3], 32), 13), 32);

		const auto i_vec = rev_bytes(get_bits(ll(i) % (1LL << 32), 32));
		e = g(plus_vec(b, plus_vec(c, keys[7 * i - 4], 32), 32), 21);
		e = xor_v(e, i_vec);

		b = plus_vec(b, e, 32);
		c = minus_vec(c, e, 32);

		d = plus_vec(d, g(plus_vec(c, keys[7 * i - 5], 32), 13), 32);
		b = xor_v(b, g(plus_vec(a, keys[7 * i - 6], 32), 21));
		c = xor_v(c, g(plus_vec(d, keys[7 * i - 7], 32), 5));

		swap_vec(a, b);
		swap_vec(c, d);
		swap_vec(a, d);
	}

	res.insert(res.end(), c.begin(), c.end());
	res.insert(res.end(), a.begin(), a.end());
	res.insert(res.end(), d.begin(), d.end());
	res.insert(res.end(), b.begin(), b.end());
	
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
	auto blocks = get_blocks(bits, 128);
	
	vector<vector<bool>> encrypted_blocks;
	encrypted_blocks.reserve(blocks.size());
	for (const auto& block : blocks) encrypted_blocks.push_back(
		decrypt ? decrypt_block(block, keys_) : encrypt_block(block, keys_));

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
