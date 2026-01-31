#ifndef GENERATORS_H
#define GENERATORS_H

#include <iostream>
#include <fstream>
#include <random>
#include <array>
#include <vector>
#include <algorithm>
#include <functional>
#include <limits>

template <typename T, typename = std::is_integral<T>>
void gen_random_numbers(int64_t num, std::ostream& out) {
	std::random_device rd;
	std::array<int32_t, 624> numbers;
	std::generate(numbers.begin(), numbers.end(), std::ref(rd));
	std::seed_seq sseq(numbers.begin(), numbers.end());
	std::mt19937 gen(sseq);
	std::uniform_int_distribution<T> dist(0, std::numeric_limits<T>::max());
	for (int64_t i = 0; i < num; ++i)
		out << dist(gen) << std::endl;
}

template <typename T, typename = std::is_integral<T>>
void gen_random_binary_numbers(int64_t num, std::ofstream& out) {
	std::random_device rd;
	std::array<int32_t, 624> numbers;
	std::generate(numbers.begin(), numbers.end(), std::ref(rd));
	std::seed_seq sseq(numbers.begin(), numbers.end());
	std::mt19937 gen(sseq);
	std::uniform_int_distribution<T> dist(0, std::numeric_limits<T>::max());
	constexpr int data_size = 65'000;
	std::array<T, data_size> data;
	int64_t j = 0;
	while (j < num) {
		int size = num - j > data_size ? data_size : num - j;
		for (int i = 0; i < size; ++i)
			data[i] = dist(gen);
		out.write(reinterpret_cast<char*>(data.data()), size * sizeof(T));
		j += size;
	}
}

#endif // GENERATORS_H
