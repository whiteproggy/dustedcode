#include <iostream>

#include <fstream>
#include <string>
#include <vector>

#include "generators.h"
#include "times.h"
#include "files.h"
#include "sort.h"
#include "outsort.h"

int main()
{
	std::cout << "Generate random data" << std::endl;

	const std::string fname("input.bin");
	std::ofstream file(fname, std::ios::out | std::ios::binary);
	gen_random_binary_numbers<uint32_t>(1'000ll, file);
	file.close();

	auto beg_time = std::chrono::steady_clock::now();

	/////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl << "Straight merge" << std::endl;

	int A[] = {75, 32, 67, 1, 5, 3, 89, 75, 0, 0, 0, 0, 0, 0, 0, 0};
	straight_merge(A, 8);

	for (int i = 0; i < 8; ++i)
		std::cout << A[i] << ' ';
	std::cout << std::endl;

	/////////////////////////////////////////////////////////////////////////////

	std::cout << std::endl << "Natural merge!" << std::endl;

	std::string outname = natural_merge<int>(fname);

	binary_file_to_string<int>(outname, std::cout);

	auto end_time = std::chrono::steady_clock::now();
	auto diff = end_time - beg_time;
	print_duration(std::chrono::duration_cast<std::chrono::microseconds>(diff), "Time: ", std::cout);

	return 0;
}
