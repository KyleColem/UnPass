#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <time.h> 

std::vector<std::string> passArr;
char buff[512];

void unrar(const std::string& command, int &result) 
{
	result = std::system(command.c_str());
}

int main(int argc, char** argv)
{
	std::ifstream file;
	std::ofstream outFile;
	bool complete = false;
	unsigned int threadN=2;
	int * h1;
	
	h1 = (int*)malloc(threadN * sizeof(int));
	if (h1 == 0) { return -1; }
	for (unsigned int y = 0; y < threadN; y++) {
		h1[y] = 1;
	}

	file.open("dictionary.txt", std::ios::in);
	if (!file.is_open()) 
	{
		std::cout << "can`t open dictionary" << std::endl;
		if (h1) free(h1);
		return -1;
	}
	
	while (file.getline(buff,512))
	{
		passArr.push_back(buff);
	}
	   
	auto start = std::chrono::high_resolution_clock::now();
	std::thread* thArr[64];

	for (unsigned int i = 0; i < passArr.size() - threadN; i+= threadN) {
		
		for (unsigned int u = 0; u < threadN; u++) {
			thArr[u] = new std::thread(unrar, "UnRAR.exe E -INUL -P\"" + passArr.at(i+u) + "\" 1.rar", std::ref(h1[u]));
		}
		
		for (unsigned int k = 0; k < threadN; k++) {
			thArr[k]->join();
			if (h1[k] == 0) {
				std::cout << "Password: " << passArr.at(i) << std::endl;
				outFile.open("result.txt", std::ios::out);
				outFile << passArr.at(i) << std::endl;
				outFile.close();
				complete = true;
				if (h1) free(h1);
			}
		}

		if (complete) return 0;
		
		if ((i % 1000 == 0.)&&(i >=1000)) {
			
			std::cout << "try pass \"" << passArr.at(i) << "\"";
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = end - start;
			double passHash = i / duration.count();
			std::cout <<'\t' << passHash << " passwords/sec" << std::endl;
			
		}
		
	}

	
	if (!complete) {
		std::cout << "Password not found" << std::endl;
	}
	
}
