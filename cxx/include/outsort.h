#ifndef OUTSORT_H
#define OUTSORT_H

#include "files.h"

template <typename T>
std::string natural_merge(const std::string& srcname) {
	std::string dstname{"outdata.bin"};
	File* file2 = nullptr;
	File* src = new File();
	src->open(srcname);
	Rider<T> rider2;
	run_set(rider2, src);

	int L = 0;
	do {
		File* file0 = new File();
		file0->create("data0.bin");
		Rider<T> rider0;
		file_set(rider0, file0, 0);

		File* file1 = new File();
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
		file0 = new File();
		file0->open("data0.bin");
		file_set(rider0, file0, 0);
		run_set(rider0, file0);

		file1 = new File();
		file1->open("data1.bin");
		file_set(rider1,file1,  0);
		run_set(rider1, file1);

		file2 = new File();
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
		file2 = new File();
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

#endif // OUTSORT_H
