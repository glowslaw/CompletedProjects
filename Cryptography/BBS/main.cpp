#include <iostream>

#include <vector>

int size_of_key = 20000;

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

std::vector<int> find_3mod4_primes(std::vector<int> primes)
{
	std::vector<int> special_primes;
	for (int i = 0; i < primes.size(); i++)
	{
		if (primes[i] % 4 == 3)
			special_primes.push_back(primes[i]);
	}
	return special_primes;
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

int find_x0(int n)
{
	for (int i = 1000; i < 9999; i++)
		if (NWD(i, n) == 1)
			return i;
	return -1;
}

std::vector <int> Generate_Ones_and_Zeroes(int x0, int n)
{
	std::vector<int> ones_and_zeroes;
	int x = x0;
	for (int i = 0; i < size_of_key; i++)
	{
		if (x % 2 == 0)
			ones_and_zeroes.push_back(0);
		else
			ones_and_zeroes.push_back(1);
		x = x * x % n;
	}
	return ones_and_zeroes;
}

int number_of_ones(std::vector<int> ones_and_zeroes)
{
	int number = 0;
	for (int i = 0; i < ones_and_zeroes.size(); i++)
		if(ones_and_zeroes[i] == 1)
			number++;
	return number;
}

std::vector<int> check_series_of_ones(std::vector<int> ones_and_zeroes)
{
	std::vector<int> series = {0,0,0,0,0,0};
	int length = 0;
	for (int i = 0; i < ones_and_zeroes.size(); i++)
	{
		if (ones_and_zeroes[i] == 1)
		{
			length++;
			if (i == ones_and_zeroes.size() - 1)
			{
				if (length >= 6)
					series[5]++;
				else
					series[length - 1]++;
				length = 0;
			}
		}
		else
		{
			if (length >= 6)
				series[5]++;
			else if(length >= 1)
				series[length - 1]++;
			length = 0;
		}
	}
	return series;
}

std::vector<int> check_series_of_zeroes(std::vector<int> ones_and_zeroes)
{
	std::vector<int> series = { 0,0,0,0,0,0 };
	int length = 0;
	for (int i = 0; i < ones_and_zeroes.size(); i++)
	{
		if (ones_and_zeroes[i] == 0)
		{
			length++;
			if (i == ones_and_zeroes.size() - 1)
			{
				if (length >= 6)
					series[5]++;
				else
					series[length - 1]++;
				length = 0;
			}
		}
		else
		{
			if (length >= 6)
				series[5]++;
			else if (length >= 1)
				series[length - 1]++;
			length = 0;
		}
	}
	return series;
}

int longest_series_of_ones(std::vector<int> ones_and_zeroes)
{
	int biggest_length = 0;
	int length = 0;
	for (int i = 0; i < ones_and_zeroes.size(); i++)
	{
		if (ones_and_zeroes[i] == 1)
		{
			length++;
			if (i == ones_and_zeroes.size() - 1)
			{
				if (length > biggest_length)
					biggest_length = length;
				length = 0;
			}
		}
		else
		{
			if (length > biggest_length)
				biggest_length = length;
			length = 0;
		}
	}
	return biggest_length;
}

int longest_series_of_zeroes(std::vector<int> ones_and_zeroes)
{
	int biggest_length = 0;
	int length = 0;
	for (int i = 0; i < ones_and_zeroes.size(); i++)
	{
		if (ones_and_zeroes[i] == 0)
		{
			length++;
			if (i == ones_and_zeroes.size() - 1)
			{
				if (length > biggest_length)
					biggest_length = length;
				length = 0;
			}
		}
		else
		{
			if (length > biggest_length)
				biggest_length = length;
			length = 0;
		}
	}
	return biggest_length;
}

std::vector<int> count_segments(std::vector<int> ones_and_zeroes)
{
	std::vector<int> segments = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	for (int i = 0; i < ones_and_zeroes.size(); i = i + 4)
	{
		int index = 8 * ones_and_zeroes[i] + 4 * ones_and_zeroes[i+1] + 2 * ones_and_zeroes[i+2] + ones_and_zeroes[i+3];
		segments[index]++;
	}
	return segments;
}

float count_chi_squared(std::vector<int> segments)
{
	float chi_squared = 0;
	float expected_value = size_of_key / 4 / 16;
	for (int i = 0; i < segments.size(); i++)
	{
		float observed_value = segments[i];
		chi_squared = chi_squared + (observed_value - expected_value) * (observed_value - expected_value) / expected_value;
	}
	return chi_squared;
}

int main()
{
	std::vector <int> primes = find_3mod4_primes(find_4digit_primes());

	int p = primes[0];
	int q = primes[1];
	int n = p * q;
	int x0 = find_x0(n);
	std::vector<int> ones_and_zeroes = Generate_Ones_and_Zeroes(x0, n);
	std::vector<int> series_of_ones = check_series_of_ones(ones_and_zeroes);
	std::vector<int> series_of_zeroes = check_series_of_zeroes(ones_and_zeroes);
	std::vector<int> segments = count_segments(ones_and_zeroes);
	float chi_squared = count_chi_squared(segments);

	std::cout << "p: " << p << std::endl;
	std::cout << "q: " << q << std::endl;
	std::cout << "n: " << n << std::endl;
	std::cout << "x0: " << x0 << std::endl;
	std::cout << "number of ones: " << number_of_ones(ones_and_zeroes) << std::endl;
	std::cout << "number of zeroes: " << size_of_key - number_of_ones(ones_and_zeroes) << std::endl;

	std::cout<<std::endl;
	std::cout << "series of ones: " << std::endl;
	for (int i = 0; i < series_of_ones.size(); i++)
		std::cout << series_of_ones[i] << std::endl;

	std::cout << std::endl;
	std::cout << "series of zeroes: " << std::endl;
	for (int i = 0; i < series_of_zeroes.size(); i++)
		std::cout << series_of_zeroes[i] << std::endl;

	std::cout << std::endl;
	std::cout << "longest series of ones: " << longest_series_of_ones(ones_and_zeroes) << std::endl;
	std::cout << "longest series of zeroes: " << longest_series_of_zeroes(ones_and_zeroes) << std::endl;

	std::cout << std::endl;
	std::cout << "segments: " << std::endl;
	for (int i = 0; i < segments.size(); i++)
		std::cout << segments[i] << std::endl;

	std::cout << std::endl;
	std::cout << "chi_squared: " << chi_squared << std::endl;
}