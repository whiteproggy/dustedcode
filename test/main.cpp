#include <iostream>

#include <fstream>
#include <string>
#include <vector>

//#include "sort.h"

int divide(int dividend, int divisor) {
	int q = 0;
	int m{abs(dividend)}, d{abs(divisor)};
	if (m == -2147483648)
		m -= 1;
	while (m > 0) {
		m -= d;
		if (m < 0)
			break;
		++q;
	}
	return (dividend < 0 && divisor < 0) || (dividend > 0 && divisor > 0) ? q : -q;
}

int main()
{
	std::cout << "Generate random data" << std::endl;

	std::cout << divide(-2147483648, -1) << std::endl;

	const std::string fname("input.bin");
//	std::ofstream file(fname, std::ios::out | std::ios::binary);
//	gen_random_binary_numbers<uint32_t>(1'000ll, file);
//	file.close();

//	auto beg_time = std::chrono::steady_clock::now();

	/////////////////////////////////////////////////////////////////////////////

//	std::cout << std::endl << "Straight merge" << std::endl;

//	int A[] = {75, 32, 67, 1, 5, 3, 89, 75, 0, 0, 0, 0, 0, 0, 0, 0};
//	straight_merge(A, 8);

//	for (int i = 0; i < 8; ++i)
//		std::cout << A[i] << ' ';
//	std::cout << std::endl;

	/////////////////////////////////////////////////////////////////////////////

//	std::cout << std::endl << "Natural merge!" << std::endl;

//	std::string outname = natural_merge<int>(fname);

//	binary_file_to_string<int>(outname, std::cout);

	/////////////////////////////////////////////////////////////////////////////

//	std::cout << std::endl << "Balanced merge!" << std::endl;

//	std::string outname1 = balanced_merge<int>(fname, 10);

//	std::cout << "Output file: " << outname1 << (is_sorted<int>(outname1) ? " sorted" : "not sorted") << std::endl;

	/////////////////////////////////////////////////////////////////////////////

//	std::cout << std::endl << "Polyohase merge!" << std::endl;

//	std::string outname2 = polyphase_merge<int>(fname, 10);

//	std::cout << "Output file: " << outname2 << (is_sorted<int>(outname2) ? " sorted" : "not sorted") << std::endl;


//	auto end_time = std::chrono::steady_clock::now();
//	auto diff = end_time - beg_time;
//	print_duration(std::chrono::duration_cast<std::chrono::microseconds>(diff), "Time: ", std::cout);

	return 0;
}
