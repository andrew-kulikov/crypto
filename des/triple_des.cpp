#include "triple_des.h"

triple_des::triple_des(const std::string& key1, const std::string& key2) : des1_(key1), des2_(key2)
{}

std::string triple_des::encrypt(const std::string& message) const
{
	auto encrypted = des1_.encrypt(message);
	encrypted = des2_.decrypt(encrypted);
	encrypted = des1_.encrypt(encrypted);

	return encrypted;
}

std::string triple_des::decrypt(const std::string& message) const
{
	auto decrypted = des1_.decrypt(message);
	decrypted = des2_.encrypt(decrypted);
	decrypted = des1_.decrypt(decrypted);

	return decrypted;
}
