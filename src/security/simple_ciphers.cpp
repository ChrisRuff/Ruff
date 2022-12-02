#include <ruff/security/simple_ciphers.hpp>

namespace ruff::security
{
std::string SubstitutionEncrypt(std::string msg, const std::unordered_map<char, char>& key)
{
	for(char& c : msg)
	{
		c = key.at(tolower(c));
	}
	return msg;
};
std::string SubstitutionDecrypt(std::string cipher, const std::unordered_map<char, char>& key)
{
	for(char& c : cipher)
	{
		char letter = tolower(c);
		for(auto it = key.begin(); it != key.end(); ++it)
		{
			if(it->second == letter) c = it->first;
		}
	}
	return cipher;
}

std::string TranspositionEncrypt(std::string msg, size_t ncols, std::vector<size_t> key)
{
	std::vector<std::string> cols(ncols);
	for(size_t i = 0; i < msg.size(); ++i)
	{
		cols[i % ncols].push_back(msg.at(i));
	}
	std::string ciphertext;
	for(size_t i = 0; i < key.size(); ++i)
	{
		if(key[i] > cols.size()) throw "Key is out of column bounds";
		ciphertext.append(cols[key[i]]);
	}
	return ciphertext;
}

std::string TranspositionDecrypt(std::string cipher, size_t ncols, std::vector<size_t> key)
{
	std::vector<std::string> cols(ncols);
	size_t current_key = 0;
	size_t col = key[current_key++];
	for(size_t i = 0, count = 0; i < cipher.size(); ++i)
	{
		cols[col].push_back(cipher.at(i));
		bool extra = col < cipher.size() % ncols;
		count++;
		if(count >= (cipher.size() / ncols) + extra)
		{
			col = key[current_key++];
			count = 0;
		}
	}
	std::string msg;
	col = 0;
	size_t row = 0;
	for(size_t i = 0; i < cipher.size(); ++i)
	{
		if(col >= ncols)
		{
			row++;
			col = 0;
		}
		msg.push_back(cols[col++].at(row));
	}
	return msg;
}
}// namespace ruff::security
