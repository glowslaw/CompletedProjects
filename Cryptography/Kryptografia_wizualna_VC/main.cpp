#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>

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

int main()
{
	std::vector <int> primes = find_3mod4_primes(find_4digit_primes());

	int p = primes[0];
	int q = primes[1];
	int n = p * q;
	int x0 = find_x0(n);
	std::vector<int> ones_and_zeroes = Generate_Ones_and_Zeroes(x0, n);

	std::cout << "p: " << p << std::endl;
	std::cout << "q: " << q << std::endl;
	std::cout << "n: " << n << std::endl;
	std::cout << "x0: " << x0 << std::endl;

	std::cout << std::endl;
	std::cout << std::endl;

	std::ifstream ifs;
	ifs.open("Secret.bmp", std::ios::binary);

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
			ifs.read((char*)&B, sizeof(unsigned char));

		ifs.read((char*)&B, sizeof(unsigned char));
		ifs.read((char*)&G, sizeof(unsigned char));
		ifs.read((char*)&R, sizeof(unsigned char));
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;



	std::ofstream ofs;
	ofs.open("Result.bmp", std::ios::binary);
	std::ofstream ofs2;
	ofs2.open("Result2.bmp", std::ios::binary);
	unsigned char S;
	int Entry;

	std::ifstream ifs2;
	ifs2.open("HugeRed.bmp", std::ios::binary);

	ifs2.seekg(0);

	if (!ifs2)
	{
		std::cout << "Error in opening file" << std::endl;
		system("pause");
		return 0;
	}

	for (int i = 0; i < beg; i++)
	{
		ifs2.read((char*)&S, sizeof(char));
		ofs.write((char*)&S, sizeof(char));
		ofs2.write((char*)&S, sizeof(char));
	}

	int ones_and_zeroes_counter = 0;

	ifs.seekg(beg);
	for (int i = 0; i < file_size - beg; i = i + 3)
	{
		ifs.read((char*)&B, sizeof(unsigned char));
		ifs.read((char*)&G, sizeof(unsigned char));
		ifs.read((char*)&R, sizeof(unsigned char));

		if (int(B) == 255 && int(G) == 255 && int(R) == 255)
		{
			if (ones_and_zeroes[ones_and_zeroes_counter] == 0)
			{
				int Color = 0;
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));

				Color = 255;
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));

				Color = 0;
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));

				Color = 255;
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
			}
			else
			{
				int Color = 255;
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));

				Color = 0;
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));

				Color = 255;
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));

				Color = 0;
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
			}
		}
		else
		{
			if (ones_and_zeroes[ones_and_zeroes_counter] == 0)
			{
				int Color = 0;
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));

				Color = 255;
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));

				Color = 255;
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));

				Color = 0;
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
			}
			else
			{
				int Color = 255;
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));

				Color = 0;
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));
				ofs.write((char*)&Color, sizeof(unsigned char));

				Color = 0;
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));

				Color = 255;
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
				ofs2.write((char*)&Color, sizeof(unsigned char));
			}
		}
		ones_and_zeroes_counter++;
	}

	std::ifstream slide1;
	slide1.open("Result.bmp", std::ios::binary);
	std::ifstream slide2;
	slide2.open("Result2.bmp", std::ios::binary);
	std::ofstream dec;
	dec.open("Deciphered.bmp", std::ios::binary);

	unsigned char S111, S112, S113, S121, S122, S123, S211, S212, S213, S221, S222, S223;

	if (!slide1)
	{
		std::cout << "Error in opening file S1" << std::endl;
		system("pause");
		return 0;
	}
	if (!slide2)
	{
		std::cout << "Error in opening file S2" << std::endl;
		system("pause");
		return 0;
	}

	ifs.seekg(0);
	for (int i = 0; i < beg; i++)
	{
		ifs.read((char*)&S, sizeof(char));
		dec.write((char*)&S, sizeof(unsigned char));
	}

	slide1.seekg(beg);
	slide2.seekg(beg);
	for (int i = 0; i < file_size - beg; i = i + 3)
	{
		if (i % when_newline == 0 && i != 0)
			ifs.read((char*)&S, sizeof(unsigned char));

		slide1.read((char*)&S111, sizeof(char));
		slide1.read((char*)&S112, sizeof(char));
		slide1.read((char*)&S113, sizeof(char));
		slide1.read((char*)&S121, sizeof(char));
		slide1.read((char*)&S122, sizeof(char));
		slide1.read((char*)&S123, sizeof(char));

		slide2.read((char*)&S211, sizeof(char));
		slide2.read((char*)&S212, sizeof(char));
		slide2.read((char*)&S213, sizeof(char));
		slide2.read((char*)&S221, sizeof(char));
		slide2.read((char*)&S222, sizeof(char));
		slide2.read((char*)&S223, sizeof(char));

		bool P11_is_black, P12_is_black, P21_is_black, P22_is_black;

		if (int(S111) == 0 && int(S112) == 0 && int(S113) == 0)
			P11_is_black = true;
		else
			P11_is_black = false;
		if (int(S121) == 0 && int(S122) == 0 && int(S123) == 0)
			P12_is_black = true;
		else
			P12_is_black = false;
		if (int(S211) == 0 && int(S212) == 0 && int(S213) == 0)
			P21_is_black = true;
		else
			P21_is_black = false;
		if (int(S221) == 0 && int(S222) == 0 && int(S223) == 0)
			P22_is_black = true;
		else
			P22_is_black = false;


		int D;
		if ((P11_is_black == true && P22_is_black == true) || (P12_is_black == true && P21_is_black == true))
			D = 0;
		else
			D = 255;

		dec.write((char*)&D, sizeof(unsigned char));
		dec.write((char*)&D, sizeof(unsigned char));
		dec.write((char*)&D, sizeof(unsigned char));
	}
}