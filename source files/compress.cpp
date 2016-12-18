//--------------------------------------------------------
// You can only use these headers

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "treap_bst.h"

//--------------------------------------------------------

// implement your compress program here

int main(int argc, char *argv[])
{
	TreapBST<uint32_t, std::string> dict;
	TreapBST<std::string, uint32_t> revDict;
	if (argc == 3)
	{
		char* fileIn = argv[1];
		std::ifstream f1(fileIn);
		if (!f1.good())
		{
			std::cerr << "No input file";
			return EXIT_FAILURE;
			//if the input file doesn't exist, exits the program
		}
		char* fileOut = argv[2];
		std::string val;
		std::string word;
		uint8_t endVal = 0;
		uint32_t finalVal = 0;
		std::ofstream outFile(fileOut, std::ios::binary);
		bool found = false;
		bool first = true;
		uint32_t currKey = 1;
		uint32_t i;
		int added;

		while (std::getline(f1, word, ' '))
		{
			if (first)
			{
				dict.insert(currKey, word);
				outFile.write(reinterpret_cast<const char*>(&currKey), sizeof(currKey));
				outFile.write(word.c_str(), word.length());
				outFile.write(reinterpret_cast<const char*>(&endVal), sizeof(endVal));
				first = false;
				added = 1;
			}
			
			for (i = added; i > 0; i--)
			{
				val = dict.search(i, found);
				if (word == val)
					break;
			}

			if (word != val)
			{
				added++;
				dict.insert(added, word);
				outFile.write(reinterpret_cast<const char*>(&added), sizeof(added));
				outFile.write(word.c_str(), word.length());
				outFile.write(reinterpret_cast<const char*>(&endVal), sizeof(endVal));				
			}
			currKey++;
		}
		outFile.write(reinterpret_cast<const char*>(&finalVal), sizeof(finalVal));

		f1.close();
		f1.open(fileIn);
		currKey = 1;
		val = "";

		while (std::getline(f1, word, ' '))
		{
			for(i = 0; i < currKey; i++)
			{
				val = dict.search(i, found);
				if (word == val)
					break;
			}
			outFile.write(reinterpret_cast<const char*>(&i), sizeof(i));
			
			if(word != val)
				currKey++;
		}
		return EXIT_SUCCESS;
	}
	else if (argc == 4)
	{
		std::string free = argv[1];
		if (free == "-d")
		{
			char* fileIn = argv[2];
			char* fileOut = argv[3];

			std::ifstream f1(argv[2], std::ios::binary);
			if (!f1.good())
			{
				std::cerr << "No input file";
				return EXIT_FAILURE;
				//if the input file doesn't exist, exits the program
			}

			uint32_t nums = 1;

			uint32_t key;
			uint8_t endVal = 1;
			std::string value;
			while (nums != 0)
			{
				f1.read(reinterpret_cast<char*>(&nums), sizeof(nums));
				value = "";
				endVal = 1;
				key = nums;
				while (endVal != 0)
				{
					f1.read(reinterpret_cast<char*>(&endVal), sizeof(endVal));
					value.push_back(endVal);
				}
				dict.insert(key, value);
			}

			uint32_t order = 0;
			bool found;
			std::ofstream outFile(fileOut);
			std::string val;
			
			val = dict.search(1, found);
			if (found)
			outFile.write(val.c_str(), val.length());

			while (f1.read(reinterpret_cast<char*>(&nums), sizeof(nums)))
			{
				int numel = nums >> 16;
				val = dict.search(numel, found);
				if(found)
					outFile.write(val.c_str(), val.length());
			}
		}
		else
		{
			std::cerr << "No matching command";
			return EXIT_FAILURE;
		}
	}
	else
	{
		std::cerr << "No matching command";
		return EXIT_FAILURE;
	}
}