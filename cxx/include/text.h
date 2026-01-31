#ifndef TEXT_H
#define TEXT_H

#include <string>

using std::size_t;
using Size = long long; // signed

template <typename Char>
Size straight_string_search(const Char* text, size_t n, const Char* pattern, size_t m)
{
	size_t i = 0, j = 0;
	while (i <= n - m && j < m)
		if (text[i + j] == pattern[j])
			++j;
		else {
			j = 0;
			++i;
		}
	return j == m ? i : -1;
}

template <typename Char>
Size knuth_morris_pratt_string_search(const Char* text, size_t n, const Char* pattern, size_t m)
{
	const size_t mmax = m + 1LL;
	auto* distance = new Size[mmax];

	// calculate distanses
	distance[0] = -1;
	distance[1] = pattern[0] != pattern[1] ? 0 : -1;
	Size j = 1, k = 0;
	while (j < static_cast<Size>(m - 1))
		if (k >= 0 && pattern[j] != pattern[k])
			k = distance[k];
		else {
			++j, ++k;
			distance[j] = pattern[j] != pattern[k] ? k : distance[k];
		}

	// search
	size_t i = 0;
	j = 0;
	while (j < static_cast<Size>(m) && i < n)
		if (j >= 0 && text[i] != pattern[j])
			j = distance[j];
		else
			++i, ++j;
	delete[] distance;
	return j == static_cast<Size>(m) ? i - m : -1LL;
}

template <typename Char>
Size boyer_moore_string_search(const Char* text, size_t n, const Char* pattern, size_t m)
{
	Size distance[128];
	for (auto& d: distance)
		d = m;
	for (size_t i = 0; i < m - 1; ++i)
		distance[pattern[i]] = m - i - 1;
	Size i = m, j = m, k = m;
	while (j > 0 && i < static_cast<Size>(n))
		if (text[k-1] == pattern[j-1])
			--k, --j;
		else {
			i += distance[text[i - 1]];
			j = m;
			k = i;
		}
	return j <= 0 ? k : -1;
}

#endif // TEXT_H
