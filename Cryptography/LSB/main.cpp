#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <bitset>

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

std::string textToBinary(const std::string& text) {
	std::string binaryString = "";
	for (char c : text) {
		binaryString += std::bitset<8>(c).to_string();
	}
	return binaryString;
}

std::string binaryToText(const std::string& binary) {
	std::string text = "";
	for (size_t i = 0; i < binary.length(); i += 8) {
		std::string byte = binary.substr(i, 8);
		char c = static_cast<char>(std::bitset<8>(byte).to_ulong());
		text += c;
	}
	return text;
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


	std::string message = "Ala ma kota";
	std::string bit_message = textToBinary(message);
	for (int i = 0; i < file_size - beg; i = i + 3)
	{
		if (i % when_newline == 0 && i != 0)
		{
			ifs2.read((char*)&B, sizeof(unsigned char));
			ofs.write((char*)&B, sizeof(unsigned char));
		}

		ifs2.read((char*)&B, sizeof(unsigned char));
		ifs2.read((char*)&G, sizeof(unsigned char));
		ifs2.read((char*)&R, sizeof(unsigned char));

		if (i < bit_message.size())
		{
			if (bit_message[i] == '0' && int(B) % 2 == 1)
				B = B - 1;
			if (bit_message[i] == '1' && int(B) % 2 == 0)
				B = B + 1;
		}

		if (i + 1 < bit_message.size())
		{
			if (bit_message[i + 1] == '0' && int(G) % 2 == 1)
				G = G - 1;
			if (bit_message[i + 1] == '1' && int(G) % 2 == 0)
				G = G + 1;
		}

		if (i + 2 < bit_message.size())
		{
			if (bit_message[i + 2] == '0' && int(R) % 2 == 1)
				R = R - 1;
			if (bit_message[i + 2] == '1' && int(R) % 2 == 0)
				R = R + 1;
		}


		ofs.write((char*)&B, sizeof(unsigned char));
		ofs.write((char*)&G, sizeof(unsigned char));
		ofs.write((char*)&R, sizeof(unsigned char));
	}

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

	std::string deciphered_bit_message;
	for (int i = 0; i < file_size - beg; i = i + 3)
	{
		if (i % when_newline == 0 && i != 0)
		{
			ifs3.read((char*)&B, sizeof(unsigned char));
		}

		ifs3.read((char*)&B, sizeof(unsigned char));
		ifs3.read((char*)&G, sizeof(unsigned char));
		ifs3.read((char*)&R, sizeof(unsigned char));

		if (int(B) % 2 == 1)
			deciphered_bit_message += "1";
		else
			deciphered_bit_message += "0";

		if (int(G) % 2 == 1)
			deciphered_bit_message += "1";
		else
			deciphered_bit_message += "0";

		if (int(R) % 2 == 1)
			deciphered_bit_message += "1";
		else
			deciphered_bit_message += "0";
	}
	std::cout << deciphered_bit_message << std::endl;
	std::cout << std::endl;
	std::string deciphered_message = binaryToText(deciphered_bit_message);
	std::cout << deciphered_message << std::endl;
}