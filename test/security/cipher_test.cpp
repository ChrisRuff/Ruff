#include "doctest/doctest.h"

#include <string>
#include <iostream>

#include <ruff/security/simple_ciphers.hpp>
TEST_SUITE("Ciphers")
{
	TEST_CASE("Substitution")
	{
		using namespace ruff::security;
		std::string msg = "hello world";
		std::unordered_map<char, char> key{
			{ ' ', ' ' },
			{ 'a', 'b' },
			{ 'b', 'c' },
			{ 'c', 'd' },
			{ 'd', 'e' },
			{ 'e', 'f' },
			{ 'f', 'g' },
			{ 'g', 'h' },
			{ 'h', 'i' },
			{ 'i', 'j' },
			{ 'j', 'k' },
			{ 'k', 'l' },
			{ 'l', 'm' },
			{ 'm', 'n' },
			{ 'n', 'o' },
			{ 'o', 'p' },
			{ 'p', 'q' },
			{ 'q', 'r' },
			{ 'r', 's' },
			{ 's', 't' },
			{ 't', 'u' },
			{ 'u', 'v' },
			{ 'v', 'w' },
			{ 'w', 'x' },
			{ 'x', 'y' },
			{ 'y', 'z' },
			{ 'z', 'a' },
		};
		auto cipher = SubstitutionEncrypt("hello world", key);
		auto decrypted = SubstitutionDecrypt(cipher, key);

		CHECK(msg == decrypted);
	};
	TEST_CASE("Transposition 1")
	{
		using namespace ruff::security;
		std::string msg = "hello world";
		auto cipher = TranspositionEncrypt(msg, 5, { 1, 2, 3, 0, 4 });
		auto decrypted = TranspositionDecrypt(cipher, 5, { 1, 2, 3, 0, 4 });

		CHECK(msg == decrypted);
	}
	TEST_CASE("Transposition 2")
	{
		using namespace ruff::security;
		std::string msg = "hello world";
		auto cipher = TranspositionEncrypt(msg, 3, { 0, 2, 1 });
		auto decrypted = TranspositionDecrypt(cipher, 3, { 0, 2, 1 });

		CHECK(msg == decrypted);
	}
};
