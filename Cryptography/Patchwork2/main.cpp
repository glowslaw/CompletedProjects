#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <bitset>
#include <unordered_set>

int size_of_key = 2000000;

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

std::vector<int> generateRandomDistinctNumbers(int n, int k, int minVal = 0, int maxVal = 1000000) 
{
	std::srand(k);
	std::unordered_set<int> uniqueNumbers;

	while (uniqueNumbers.size() < n)
	{
		int randomNum = std::rand() % (maxVal - minVal + 1) + minVal;
		uniqueNumbers.insert(randomNum);
	}

	std::vector<int> randomNumbers(uniqueNumbers.begin(), uniqueNumbers.end());

	return randomNumbers;
}

int countSumOfDifferencesBetweenPairs(std::vector<std::vector<unsigned char>> image, std::vector<int> set_A, std::vector<int> set_B)
{
	int sum = 0;
	for (int i = 0; i < set_A.size(); i++)
	{
		int a = image[set_A[i]][0] + image[set_A[i]][1] + image[set_A[i]][2];
		int b = image[set_B[i]][0] + image[set_B[i]][1] + image[set_B[i]][2];
		sum += a - b;
	}
	return sum;
}

int main()
{
	std::vector <int> primes = find_3mod4_primes(find_4digit_primes());

	int p = primes[0];
	int q = primes[1];
	int n = p * q;
	int x0 = find_x0(n);
	int shift = 10;
	int number_of_pairs = 1000;
	int k = 1;
	std::vector<int> ones_and_zeroes = Generate_Ones_and_Zeroes(x0, n);

	std::cout << "p: " << p << std::endl;
	std::cout << "q: " << q << std::endl;
	std::cout << "n: " << n << std::endl;
	std::cout << "x0: " << x0 << std::endl;
	std::cout << "shift: " << shift << std::endl;
	std::cout << "number_of_pairs: " << number_of_pairs << std::endl;
	std::cout << "k: " << k << std::endl;

	std::cout << std::endl;
	std::cout << std::endl;

	std::ifstream ifs;
	ifs.open("Data2.bmp", std::ios::binary);

	if (!ifs)
	{
		std::cout << "Error in opening file" << std::endl;
		system("pause");
		return 0;
	}

	ifs.seekg(2);

	int file_size;
	ifs.read((char*)&file_size, sizeof(int));

	std::cout << "Bitmap size: " << file_size << std::endl;

	ifs.seekg(10);
	int beg;
	ifs.read((char*)&beg, sizeof(int));

	std::cout << "Beggining of image: " << beg << std::endl;

	ifs.seekg(18);
	int columns;
	ifs.read((char*)&columns, sizeof(int));

	std::cout << "Column number: " << columns << std::endl;

	ifs.seekg(22);
	int rows;
	ifs.read((char*)&rows, sizeof(int));

	std::cout << "Row number: " << rows << std::endl;

	ifs.seekg(beg);

	unsigned char R, G, B;
	int when_newline = columns * 3;
	for (int i = 0; i < file_size - beg - rows; i += 3)
	{
		if (i % when_newline == 0 && i != 0)
		{
			ifs.read((char*)&B, sizeof(unsigned char));
		}

		ifs.read((char*)&B, sizeof(unsigned char));
		ifs.read((char*)&G, sizeof(unsigned char));
		ifs.read((char*)&R, sizeof(unsigned char));

	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;



	std::ifstream ifs2;
	ifs2.open("badger.bmp", std::ios::binary);

	if (!ifs2)
	{
		std::cout << "Error in opening file" << std::endl;
		system("pause");
		return 0;
	}

	std::ofstream ofs;
	ofs.open("Result.bmp", std::ios::binary);
	unsigned char S;
	int Entry;

	for (int i = 0; i < beg; i++)
	{
		ifs2.read((char*)&S, sizeof(char));
		ofs.write((char*)&S, sizeof(char));
	}

	std::vector<std::vector<unsigned char>> image;
	std::vector<int> random_numbers = generateRandomDistinctNumbers(2 * number_of_pairs, k, 0, rows * columns);
	std::vector<int> set_A(random_numbers.begin(), random_numbers.begin() + random_numbers.size() / 2);
	std::vector<int> set_B(random_numbers.begin() + random_numbers.size() / 2, random_numbers.begin() + random_numbers.size());

	for (int i = 0; i < file_size - beg; i = i + 3)
	{
		ifs2.read((char*)&B, sizeof(unsigned char));
		ifs2.read((char*)&G, sizeof(unsigned char));
		ifs2.read((char*)&R, sizeof(unsigned char));

		std::vector<unsigned char> pixel;
		pixel.push_back(B);
		pixel.push_back(G);
		pixel.push_back(R);

		image.push_back(pixel);
	}

	std::cout << "BeforeShifting" << std::endl;
	std::cout << "SumOfDifferencesBetweenPairs: " << countSumOfDifferencesBetweenPairs(image, set_A, set_B) << std::endl;
	std::cout << "AverageOfDifferencesBetweenPairs: " << countSumOfDifferencesBetweenPairs(image, set_A, set_B) / 1000 << std::endl;
	std::cout << std::endl;

	int overflow_counter = 0;
	for (int i = 0; i < set_A.size(); i++)
	{
		if (image[set_A[i]][0] + shift <= 255)
		{
			image[set_A[i]][0] += shift;
			image[set_A[i]][1] += shift;
			image[set_A[i]][2] += shift;
		}
		else
			overflow_counter++;
	}
	for (int i = 0; i < set_B.size(); i++)
	{
		if (image[set_B[i]][0] - shift >= 0)
		{
			image[set_B[i]][0] -= shift;
			image[set_B[i]][1] -= shift;
			image[set_B[i]][2] -= shift;
		}
		else
			overflow_counter++;
	}
	std::cout << "overflow_counter: " << overflow_counter << std::endl;
	std::cout << std::endl;

	for (int i = 0; i < file_size - beg; i = i + 3)
	{
		ofs.write((char*)&image[int(i / 3)][0], sizeof(unsigned char));
		ofs.write((char*)&image[int(i / 3)][1], sizeof(unsigned char));
		ofs.write((char*)&image[int(i / 3)][2], sizeof(unsigned char));
	}

	std::cout << "AfterShifting" << std::endl;
	std::cout << "SumOfDifferencesBetweenPairs: " << countSumOfDifferencesBetweenPairs(image, set_A, set_B) << std::endl;
	std::cout << "AverageOfDifferencesBetweenPairs: " << countSumOfDifferencesBetweenPairs(image, set_A, set_B) / 1000 << std::endl;
	std::cout << std::endl;

	std::ifstream ifs3;
	ifs3.open("Result.bmp", std::ios::binary);

	if (!ifs3)
	{
		std::cout << "Error in opening file" << std::endl;
		system("pause");
		return 0;
	}

	for (int i = 0; i < beg; i++)
	{
		ifs3.read((char*)&S, sizeof(char));
	}

	std::vector<std::vector<unsigned char>> image2;
	std::vector<int> random_numbers2 = generateRandomDistinctNumbers(2 * number_of_pairs, k, 0, rows * columns);
	std::vector<int> set_A2(random_numbers2.begin(), random_numbers2.begin() + random_numbers2.size() / 2);
	std::vector<int> set_B2(random_numbers2.begin() + random_numbers2.size() / 2, random_numbers2.begin() + random_numbers2.size());

	for (int i = 0; i < file_size - beg; i = i + 3)
	{
		ifs3.read((char*)&B, sizeof(unsigned char));
		ifs3.read((char*)&G, sizeof(unsigned char));
		ifs3.read((char*)&R, sizeof(unsigned char));

		std::vector<unsigned char> pixel;
		pixel.push_back(B);
		pixel.push_back(G);
		pixel.push_back(R);

		image2.push_back(pixel);
	}

	int overflow_counter2 = 0;
	for (int i = 0; i < set_B.size(); i++)
	{
		if (image2[set_B[i]][0] + shift <= 255)
		{
			image2[set_B[i]][0] += shift;
			image2[set_B[i]][1] += shift;
			image2[set_B[i]][2] += shift;
		}
		else
			overflow_counter2++;
	}
	for (int i = 0; i < set_A.size(); i++)
	{
		if (image2[set_A[i]][0] - shift >= 0)
		{
			image2[set_A[i]][0] -= shift;
			image2[set_A[i]][1] -= shift;
			image2[set_A[i]][2] -= shift;
		}
		else
			overflow_counter2++;
	}
	std::cout << "overflow_counter2: " << overflow_counter2 << std::endl;
	std::cout << std::endl;

	std::cout << "AfterShiftingBack" << std::endl;
	std::cout << "SumOfDifferencesBetweenPairs: " << countSumOfDifferencesBetweenPairs(image2, set_A2, set_B2) << std::endl;
	std::cout << "AverageOfDifferencesBetweenPairs: " << countSumOfDifferencesBetweenPairs(image2, set_A2, set_B2)/1000 << std::endl;
	std::cout << std::endl;
}