#include <iostream>
#include <vector>

bool is_prime(int number)
{
	if (number == 2)
		return true;
	if (number == 0 || number == 1 || number % 2 == 0)
		return false;
	for (int i = 3; i * i <= number; i = i + 2)
		if (number % i == 0)
			return false;
	return true;
}

std::vector<int> find_4digit_primes()
{
	std::vector<int> primes;
	for (int i = 1000; i < 9999; i++)
		if (is_prime(i) == true)
			primes.push_back(i);
	return primes;
}

long long pow(int base, int exponent, int mod)
{
	long long power = 1;
	for (int i = 0; i < exponent; i++)
		power = (power * base) % mod;
	return power;
}

int primitive_root_of_prime(int n)
{
	for (int r = 1; r <= n - 1; r++)
	{
		std::vector<int> powers;
		powers.resize(n - 1);
		for (int x = 0; x <= n - 2; x++)
		{
			int p = pow(r, x, n);
			if (p == 0)
				return -2;

			powers[p - 1]++;
		}

		bool root_found = true;
		for (int i = 0; i < powers.size(); i++)
		{
			if (powers[i] != 1)
				root_found = false;
		}
		if (root_found == true)
			return r;
	}
	return -1;
}

int main()
{
	std::vector <int> primes = find_4digit_primes();
	int n = primes[primes.size() - 1];
	int g = primitive_root_of_prime(n);
	int x = 7279;
	int X = pow(g, x, n);
	int y = 1893;
	int Y = pow(g, y, n);
	int kA = pow(Y, x, n);
	int kB = pow(X, y, n);

	std::cout << "n: " << n << std::endl;
	std::cout << "g: " << g << std::endl;
	std::cout << "x: " << x << std::endl;
	std::cout << "X: " << X << std::endl;
	std::cout << "y: " << y << std::endl;
	std::cout << "Y: " << Y << std::endl;
	std::cout << "kA: " << kA << std::endl;
	std::cout << "kB: " << kB << std::endl;
}