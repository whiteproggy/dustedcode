#ifndef SORT_H
#define SORT_H

template <typename T>
void swap(T& a, T& b) { T c{a}; a = b; b = c; }

template <>
void swap(int& a, int& b) { a ^= b; b ^= a; a ^= b; }

/*******************************************************************/

// [Wirth, 1985] (ch. 2.4.1, p. 72)
// Input:  array A[0, 2n), [0, n) - data for sorting
// Output: array A[0, 2n), [n, 2n) - sorted data

template<typename T>
void straight_merge(T* A, int n) {
	bool up = true;
	int p = 1;
	do {
		int h = 1, m = n, i, j, k, L;
		if (up)
			i = 0, j = n-1, k = n, L = 2 * n - 1;
		else
			k = 0, L = n-1, i = n, j = 2 * n - 1;
		do {
			int q = m >= p ? p : m;
			m -= q;
			int r = m >= p ? p : m;
			m -= r;
			while (q > 0 && r > 0)
				if (A[i] < A[j])
					A[k] = A[i++], k += h, q--;
				else
					A[k] = A[j--], k += h, r--;
			while (r > 0)
				A[k] = A[j--], k += h, r--;
			while (q > 0)
				A[k] = A[i++], k += h, q--;
			h = -h;
			swap(k, L);
		} while (m != 0);
		up = !up;
		p *= 2;
	} while (p < n);
	if (!up) {
		for (int i = 0; i < n; ++i)
			A[i] = A[i+n];
	}
}

template<typename T>
void shell_sort(T* A, int left, int right) {
    int h;
    for (h = left; h <= (right - left) / 9; h = 3 * h + 1)
        ;
    for (; h > 0; h /= 3)
        for (int i = left + h; i <= right; ++i) {
            int j = i;
            T v = A[i];
            while (j >= left + h && v < A[j - h]) {
                A[j] = A[j-h];
                j -= h;
            }
            A[j] = v;
        }
}

#endif // SORT_H
