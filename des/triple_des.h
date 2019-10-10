#pragma once
#include "des.h"

class triple_des
{
private:
	des des1_;
	des des2_;
public:
	triple_des(const std::string& key1, const std::string& key2);

	std::string encrypt(const std::string& message) const;
	std::string decrypt(const std::string& message) const;
};

