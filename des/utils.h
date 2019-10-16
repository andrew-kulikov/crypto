#pragma once

#include <vector>
#include <string>

using namespace std;

int to_int(const vector<bool>& bits);

vector<bool> permutate(const vector<bool>& block, const vector<int>& table);

vector<bool> xor_v(vector<bool> a, vector<bool> b);

vector<bool> get_bits(const vector<char>& text);

vector<bool> get_bits(int val, int size);

vector<vector<bool>> get_blocks(vector<bool> bits, unsigned int block_size);

char restore_byte(const vector<bool>& bits);

bool is_zero_vector(const vector<bool>& v);

string restore_block(const vector<bool>& bits);

string restore_message(const vector<vector<bool>>& blocks);

void print(const vector<bool>& block);

void print(const vector<vector<bool>>& message);
