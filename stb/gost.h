#pragma once
#include <vector>
#include <string>

using namespace std;

class gost
{
private:
	const string default_key_ = "qwertyuiasdfghjkzxcvbnmkpoiuytre";
	vector<vector<bool>> keys_;

	vector<bool> h_permutation(vector<bool> u) const;
	vector<bool> g(const vector<bool>& u, int r) const;
	vector<vector<bool>> generate_keys(const string& key) const;
	vector<bool> encrypt_block(const vector<bool>& block, const vector<vector<bool>>& keys) const;
	vector<bool> decrypt_block(const vector<bool>& block, const vector<vector<bool>>& keys) const;
public:
	gost();
	explicit gost(const string& base_key);

	string encrypt_simple(const string& message) const;
	string decrypt_simple(const string& message) const;

	string encrypt_blocks(const string& message, const string& sync) const;
	string decrypt_blocks(const string& message, const string& sync) const;
};
