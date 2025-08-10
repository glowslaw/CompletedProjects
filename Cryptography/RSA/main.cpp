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

int get_phi_n(int p, int q)
{
	return (p - 1) * (q - 1);
}

int NWD(int a, int b)
{
	int c;
	while (b != 0)
	{
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}

int get_e(int phi_n)
{
	for (int i = 1000; i < 9999; i++)
		if (NWD(i, phi_n) == 1)
			return i;
	return -1;
}

int inverse(int e, int phi_n)
{
	for (int i = 1; i < phi_n; i++)
	{
		if (e * i % phi_n == 1)
			return i;
	}
	return -1;
}

long long pow(int base, int exponent, int mod)
{
	long long power = 1;
	for (int i = 0; i < exponent; i++)
		power = (power * base) % mod;
	return power;
}

std::vector<long long> cipher(std::string m, int e, int n)
{
	std::vector<long long> c;
	c.resize(m.size());
	for (int i = 0; i < m.size(); i++)
		c[i] = pow(m[i], e, n);
	return c;
}

std::string decipher(std::vector<long long> c, int d, int n)
{
	std::string m;
	m.resize(c.size());
	for (int i = 0; i < m.size(); i++)
		m[i] = pow(c[i], d, n);
	return m;
}

int main()
{
	std::vector <int> primes = find_4digit_primes();

	int p = primes[0];
	int q = primes[1];
	int n = p * q;
	int phi_n = get_phi_n(p, q);
	int e = get_e(phi_n);
	int d = inverse(e, phi_n);
	std::string m = "Ala ma anakonde. Bartek ma barana. Czarek ma cent";
	std::vector<long long> c = cipher(m, e, n);
	std::string dec = decipher(c, d, n);

	std::cout << "p: " << p << std::endl;
	std::cout << "q: " << q << std::endl;
	std::cout << "n: " << n << std::endl;
	std::cout << "phi_n: " << phi_n << std::endl;
	std::cout << "e: " << e << std::endl;
	std::cout << "d: " << d << std::endl;
	std::cout << std::endl;

	std::cout << "m: " << m << std::endl;
	std::cout << std::endl;

	std::cout << "c: " << std::endl;
	for (int i = 0; i < c.size(); i++)
		std::cout << c[i] << " ";
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "dec: " << dec << std::endl;
}