#include "euler/euler.hpp"

namespace ruff
{
namespace euler
{
	static std::vector primes = SieveOfAtkin(50000);

	std::vector<int> findPrimeFactors(long num)
	{
		std::vector<int> factors;
		for(const int prime : primes)
		{
			if(num % prime == 0)
			{
				num /= prime;
				factors.push_back(prime);
			}
		}
		return factors;
	}

	// Sieve of Atkin adapted from:
	// https://www.geeksforgeeks.org/sieve-of-atkin/
	std::vector<unsigned long> SieveOfAtkin(unsigned long limit)
	{
		std::vector<unsigned long> primes;

		// 2 and 3 are known to be prime
		if(limit > 2)
			primes.push_back(2);
		if(limit > 3)
			primes.push_back(3);

		// Initialise the sieve array with false values
		bool sieve[limit];
		for(unsigned long i = 0; i < limit; i++)
			sieve[i] = false;

		/* Mark siev[n] is true if one  
				 of the following is true: 
			a) n = (4*x*x)+(y*y) has odd number of  
				 solutions, i.e., there exist 
				 odd number of distinct pairs (x, y)  
				 that satisfy the equation and 
					n % 12 = 1 or n % 12 = 5. 
			b) n = (3*x*x)+(y*y) has odd number of  
				 solutions and n % 12 = 7 
			c) n = (3*x*x)-(y*y) has odd number of  
				 solutions, x > y and n % 12 = 11 */
		for(unsigned long x = 1; x * x < limit; x++)
		{
			for(unsigned long y = 1; y * y < limit; y++)
			{
				// Main part of Sieve of Atkin
				unsigned long n = (4 * x * x) + (y * y);
				if(n <= limit && (n % 12 == 1 || n % 12 == 5))
				{
					sieve[n] ^= true;
				}

				n = (3 * x * x) + (y * y);
				if(n <= limit && n % 12 == 7)
				{
					sieve[n] ^= true;
				}

				n = (3 * x * x) - (y * y);
				if(x > y && n <= limit && n % 12 == 11)
				{
					sieve[n] ^= true;
				}
			}
		}

		// Mark all multiples of squares as non-prime
		for(unsigned long r = 5; r * r < limit; r++)
		{
			if(sieve[r])
			{
				for(unsigned long i = r * r; i < limit; i += r * r)
				{
					sieve[i] = false;
				}
			}
		}

		// Print primes using sieve[]
		for(unsigned long a = 5; a < limit; a++)
		{
			if(sieve[a])
			{
				primes.push_back(a);
			}
		}
		return primes;
	}
}// namespace euler
}// namespace ruff
