#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
#include "dataset.hpp"

using input = std::vector<long>;

static long part1(const input& input);
static long part2(const input& input);

int main() {
	auto stream = open_dataset("data/problem-10.txt");
	input input;
	long value;

	while (stream >> value) {
		input.push_back(value);
	}

	std::cout << "Part 1 Solution: " << part1(input) << "\n";
	std::cout << "Part 2 Solution: " << part2(input) << "\n";
	return 0;
}

/** Product of number of 1-jolt differences and number of 3-jolt differences.
 * Time complexity: O(n)
 * Space complexity: O(n)
*/
long part1(const input& input) {
	std::unordered_set<long> seen(input.begin(), input.end());
	long oneJoltDifferences = 0;
	long threeJoltDifferences = 1;
	seen.insert(0);

	for (long value : input) {
		if (seen.count(value - 1) > 0) {
			oneJoltDifferences += 1;
		} else if (seen.count(value - 3) > 0) {
			threeJoltDifferences += 1;
		}
	}

	return oneJoltDifferences * threeJoltDifferences;
}

/** Count the number of distinct ways the adapters can be arranged.
 * Time complexity: O(n)
 * Space complexity: O(n)
*/
long part2(const input& input) {
	long max = *std::max_element(input.begin(), input.end()) + 3;
	std::vector<long> parentCount(max + 1, 0);
	std::for_each(input.begin(), input.end(), [&](long value) { parentCount[value] = value; });
	parentCount.at(0) = 1;
	parentCount.at(max) = max;

	auto getParentCount = [&](long value) { return value >= 0 ? parentCount[value] : 0; };

	for (long value : parentCount) {
		if (value != 0) {
			parentCount[value] = getParentCount(value - 1) + getParentCount(value - 2) + getParentCount(value - 3);
		}
	}

	return parentCount.at(max);
}
