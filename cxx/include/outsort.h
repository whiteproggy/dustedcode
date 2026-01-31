#ifndef OUTSORT_H
#define OUTSORT_H

#include <string>
#include <vector>

#include "files.h"

using std::size_t;
using std::string;
using std::vector;
using std::to_string;

template <typename T>
string natural_merge(const string& srcname) {
	std::string dstname{"outdata.bin"};
	File<T>* file2 = nullptr;
	File<T>* src = new File<T>();
	src->open(srcname);
	Rider<T> rider2;
	run_set(rider2, src);

	int L = 0;
	do {
		File<T>* file0 = new File<T>();
		file0->create("data0.bin");
		Rider<T> rider0;
		file_set(rider0, file0, 0);

		File<T>* file1 = new File<T>();
		file1->create("data1.bin");
		Rider<T> rider1;
		file_set(rider1, file1,  0);

		// distrebute from rider2 to rider0 and rider1
		do {
			copy_run(rider2, rider0);
			if (!rider2.eof)
				copy_run(rider2, rider1);
		} while (!rider2.eof);
		delete file0;
		delete file1;

		// merge from rider0 and rider1 to rider2
		file0 = new File<T>();
		file0->open("data0.bin");
		file_set(rider0, file0, 0);
		run_set(rider0, file0);

		file1 = new File<T>();
		file1->open("data1.bin");
		file_set(rider1,file1,  0);
		run_set(rider1, file1);

		file2 = new File<T>();
		file2->create(dstname);
		file_set(rider2, file2, 0);
		L = 0;
		do {
			do {
				if (rider0.first < rider1.first) {
					copy_value(rider0, rider2);
					if (rider0.eor)
						copy_run(rider1, rider2);
				} else {
					copy_value(rider1, rider2);
					if (rider1.eor)
						copy_run(rider0, rider2);
				}
			} while (!(rider0.eor || rider1.eor));
			L++;
		} while (!(rider0.eof || rider1.eof));
		while (!rider0.eof)
			copy_run(rider0, rider2);
		while (!rider1.eof)
			copy_run(rider1, rider2);

		delete file0;
		delete file1;
		delete file2;
		file2 = new File<T>();
		file2->open(dstname);
		file_set(rider2, file2,  0);
		run_set(rider2, file2);
	} while (L != 1);
	delete src;
	delete file2;
	return dstname;
}

template <typename T>
void balanced_merge() {

}

template <typename T> // T is Integral
string polyphase_merge(const string& src_name, size_t num)
{
	std::vector<int> data(num, 1), diff(num, 1);
	int level = 0, cur = 0;

	auto select = [&data, &diff, &level, &cur, &num]() {
		if (diff[cur] < diff[cur + 1])
			++cur;
		else {
			if (diff[cur] == 0) {
				++level;
				auto z = data[0];
				for (size_t i = 0; i < num - 1; ++i) {
					diff[i] = z + data[i+1] - data[i];
					data[i] = z + data[i+1];
				}
			}
			cur = 0;
		}
		diff[cur]--;
	};

	///////////////////////////////////////////////////////////////

	Rider<T> rider;
	rider.open_file(src_name);

	vector<Rider<T>> riders(num, Rider<T>());
	vector<File<T>*> files(num, nullptr);
	for (size_t i = 0; i < num - 1; ++i) {
		files[i] = new File<T>("data" + to_string(i) + ".bin");
		files[i]->add_to_rider(riders[i], 0);
	}

	// distribute initial runs
	level = 1;
	data[num-1] = diff[num-1] = 0;
	cur = 0;
	while (!rider.eof && cur < num - 1) {
		select();
		copy_run(rider, riders[cur]);
	}

	while (!rider.eof) {
		select(); // riders[cur].first = last item written on files[cur]
		if (riders[cur].first <= rider.first) {
			copy_run(rider, riders[cur]);
			if (rider.eof)
				++diff[cur];
			else
				copy_run(rider, riders[cur]);
		} else
			copy_run(rider, riders[cur]);
	}

	vector<size_t> index(num, 0), index2(num, 0);
	for (size_t i = 0; i < num - 1; ++i) {
		index[i] = i;
		riders[i].set_file(files[i], 0);
	}
	index[num - 1] = num - 1;

	do { // merge from index[0] ... index[N-2] to index[N-1]
		T z = data[num - 2];
		diff[num - 1] = 0;
		files[index[num - 1]] = new File<T>("data" + to_string(num - 1) + ".bin");
		files[index[num - 1]]->add_to_rider(riders[index[num - 1]], 0);
		do { // merge one run
			size_t k = 0;
			for (size_t i = 0; i < num - 1; ++i)
				if (diff[i] > 0)
					--diff[i];
				else
					index2[k++] = index[i];
			if (k == 0)
				++diff[num - 1];
			else { // merge one real run from index[0] ... index[k-1] to index[N-1]
				while (k > 0) {
					T mx = 0;
					T min_val = riders[index2[0]].first;
				}
			}
		} while (z != 0);

	} while (level > 0);

	return {};
}

#endif // OUTSORT_H




































