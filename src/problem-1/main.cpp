#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>
#include "dataset.hpp"

using input = std::vector<long>;

static long part1(const input& input);
static long part2(const input& input);

int main() {
	auto stream = open_dataset("data/problem-1.txt");
	input input;
	long value;

	while (stream >> value) {
		input.push_back(value);
	}

	std::cout << "Part 1 Solution: " << part1(input) << "\n";
	std::cout << "Part 2 Solution: " << part2(input) << "\n";
	return 0;
}

/** Product of the two entries that sum to 2020.
 * Time complexity: O(n)
 * Space complexity: O(n)
*/
long part1(const input& input) {
	std::unordered_set<long> seen;

	for (long value : input) {
		long complement = 2020 - value;

		if (seen.count(complement) > 0) {
			return value * complement;
		} else {
			seen.insert(value);
		}
	}

	throw std::runtime_error("Part 1: No Solution!");
}

/** Product of the three entries that sum to 2020.
 * Time complexity: O(n^2)
 * Space complexity: O(n)
*/
long part2(const input& input) {
	std::unordered_set<long> seen(input.begin(), input.end());

	for (std::size_t i = 0; i < input.size(); i++) {
		for (std::size_t j = i + 1; j < input.size(); j++) {
			long a = input[i];
			long b = input[j];
			long complement = 2020 - (a + b);

			if (seen.count(complement) > 0) {
				return a * b * complement;
			}
		}
	}

	throw std::runtime_error("Part 2: No Solution!");
}
