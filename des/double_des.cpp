#include "double_des.h"

double_des::double_des(const std::string& key1, const std::string& key2) : des1_(key1), des2_(key2)
{
}

string double_des::encrypt(const std::string& message) const
{
	auto encrypted = des1_.encrypt(message);
	encrypted = des2_.encrypt(encrypted);

	return encrypted;
}

string double_des::decrypt(const std::string& message) const
{
	auto decrypted = des2_.decrypt(message);
	decrypted = des1_.decrypt(decrypted);

	return decrypted;
}
