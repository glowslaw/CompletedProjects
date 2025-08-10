#include <iostream>
#include <vector>
#include <stdlib.h>

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

std::vector<int> generate_random_numbers(int t)
{
	std::vector<int> random_numbers;
	for (int i = 0; i < t; i++)
		random_numbers.push_back(rand());
	return random_numbers;
}

long long pow(int base, int exponent, int mod)
{
	long long power = 1;
	for (int i = 0; i < exponent; i++)
		power = (power * base) % mod;
	return power;
}

std::vector<std::vector<int>> generate_vec_s(std::vector <int> vec_a, int s, int t, int p, int n)
{
	std::vector<std::vector<int>> vec_s;
	for (int i = 1; i <= n; i++)
	{
		int sum = 0;
		for (int j = 1; j <= t - 1; j++)
		{
			sum = sum + vec_a[j - 1] * pow(i,j,p);
		}
		std::vector<int> pom = { i,(s + sum) % p };
		vec_s.push_back(pom);
	}
	return vec_s;
}

int generate_secret(std::vector<std::vector<int>> vec_s, int p)
{
	int sum = 0;
	for (int k = 1; k <= vec_s.size(); k++)
	{
		double product = 1;
		int i = vec_s[k-1][0];
		int s_i = vec_s[k-1][1];
		for (int m = 1; m <= vec_s.size(); m++)
		{
			int j = vec_s[m - 1][0];
			if (i == j)
				continue;
			product = (product * (-j) / (i - j));
		}
		sum = (sum + s_i * int(product)) % p;
	}
	return sum;
}

std::vector<std::vector<int>> generate_incomplete_vec_s(std::vector<std::vector<int>> vec_s, int t)
{
	std::vector<std::vector<int>> vec_pom;
	for (int i = 0; i < t; i++)
	{
		vec_pom.push_back(vec_s[i]);
	}
	return vec_pom;
}

int main()
{
	srand(time(NULL));

	std::vector <int> primes = find_4digit_primes();

	int p = 1523;
	int s = 954;
	int n = 4;
	int t = 3;
	std::vector <int> vec_a = generate_random_numbers(t - 1);
	std::vector<std::vector<int>> vec_s = generate_vec_s(vec_a, s, t, p, n);

	std::cout << "p: " << p << std::endl;
	std::cout << "s: " << s << std::endl;
	std::cout << "n: " << n << std::endl;
	std::cout << "t: " << t << std::endl;
	std::cout << std::endl;
	for (int i = 0; i < vec_a.size(); i++)
		std::cout << "vec_a[" << i << "]: " << vec_a[i] << std::endl;
	for (int i = 0; i < vec_s.size(); i++)
		std::cout << "vec_s[" << i << "][0]: " << vec_s[i][0] << "   vec_s[" << i << "][1]: " << vec_s[i][1] << std::endl;

	std::vector<std::vector<int>> vec_pom = generate_incomplete_vec_s(vec_s, t);
	for (int i = 0; i < t; i++)
		std::cout << "vec_pom[" << i << "][0]: " << vec_pom[i][0] << "   vec_pom[" << i << "][1]: " << vec_pom[i][1] << std::endl;

	std::cout << std::endl;
	int new_s = generate_secret(vec_pom, p);
	std::cout << "new_s: " << new_s << std::endl;
}