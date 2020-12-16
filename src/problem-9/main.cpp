#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>
#include "dataset.hpp"

using input = std::vector<long>;

static long part1(const input& input);
static long part2(const input& input, long invalid);

int main() {
	auto stream = open_dataset("data/problem-9.txt");
	input input;
	long value;

	while (stream >> value) {
		input.push_back(value);
	}

	long part1Solution = part1(input);
	std::cout << "Part 1 Solution: " << part1Solution << "\n";
	std::cout << "Part 2 Solution: " << part2(input, part1Solution) << "\n";
	return 0;
}

/** Returns true if there are two integers that sum to value.
 * Time complexity: O(n)
 * Space complexity: O(n)
 */
template<typename Iterator>
bool has_pair(Iterator begin, Iterator end, long value) {
	std::unordered_set<long> seen;

	for (auto iter = begin; iter != end; ++iter) {
		seen.insert(*iter);

		if (seen.count(value - *iter) > 0) {
			return true;
		}
	}

	return false;
}

/** Returns the first number such that it is not a sum of the previous 25 numbers.
 * Time complexity: O(n)
 * Space complexity: O(1)
 */
long part1(const input& input) {
	for (auto iter = input.begin() + 25; iter != input.end(); ++iter) {
		// has_pair is O(1) here because the input size is a constant
		if (!has_pair(iter - 25, iter, *iter)) {
			return *iter;
		}
	}

	throw std::runtime_error("Part 1: No Solution!");
}

/** Returns the sum of the smallest and largest number in a continuous range of at least 2 values that sum to invalid.
 * Time complexity: O(n^2)
 * Space complexity: O(n)
*/
long part2(const input& input, long invalid) {
	long total = 0;
	std::vector<long> values;
	std::size_t last = 0;

	for (std::size_t i = 0; i < input.size(); i++) {
		total += input[i];
		values.push_back(input[i]);

		if (values.size() == 1) {
			last = i + 1;
		}

		if (total == invalid && values.size() >= 2) {
			return *std::min_element(values.begin(), values.end()) + *std::max_element(values.begin(), values.end());
		} else if (total > invalid) {
			total = 0;
			values.clear();
			i = last;
		}
	}

	throw std::runtime_error("Part 2 No Solution!");
}
