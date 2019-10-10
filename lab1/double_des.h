#pragma once
#include <string>
#include "des.h"

class double_des
{
private:
	des des1_;
	des des2_;
public:
	double_des(const std::string& key1, const std::string& key2);
	
	std::string encrypt(const std::string& message) const;
	std::string decrypt(const std::string& message) const;
};

