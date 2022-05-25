#ifndef SORT_H
#define SORT_H

inline void iswap(int& a, int& b) { a ^= b; b ^= a; a ^= b; }

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
			iswap(k, L);
		} while (m != 0);
		up = !up;
		p *= 2;
	} while (p < n);
	if (!up) {
		for (int i = 0; i < n; ++i)
			A[i] = A[i+n];
	}
}

#endif // SORT_H
