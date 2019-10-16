#pragma once

#include <vector>
#include <string>
#include <ios>

typedef long long ll;

using namespace std;


vector<bool> rev(const vector<bool>& v);

vector<bool> rev_bytes(const vector<bool>& v);

string hex_to_str(string hex);

void print_hex(vector<bool> bits);

string to_hex(int x);

ll to_int(const vector<bool>& bits);

void swap_vec(vector<bool>& v1, vector<bool>& v2);

vector<bool> permutate(const vector<bool>& block, const vector<int>& table);

vector<bool> xor_v(vector<bool> a, vector<bool> b);

vector<bool> get_bits(const vector<char>& text);

vector<bool> get_bits(ll val, int size);

vector<bool> plus_vec(const vector<bool>& a, const vector<bool>& b, int mod);

vector<bool> minus_vec(const vector<bool>& a, const vector<bool>& b, int mod);

vector<vector<bool>> get_blocks(vector<bool> bits, unsigned int block_size);

char restore_byte(const vector<bool>& bits);

bool is_zero_vector(const vector<bool>& v);

string restore_block(const vector<bool>& bits);

string restore_message(const vector<vector<bool>>& blocks);

void print(const vector<bool>& block);

void print(const vector<vector<bool>>& message);
