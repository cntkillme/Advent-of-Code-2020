#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "dataset.hpp"

using input = std::vector<long>;

static long part1(const input& input);
static long part2(const input& input);
static long simulate(const input& input, long n);

int main() {
	auto stream = open_dataset("data/problem-15.txt");
	input input;
	long value;

	while (stream >> value) {
		input.push_back(value);
		stream.ignore();
	}

	std::cout << "Part 1 Solution: " << part1(input) << "\n";
	std::cout << "Part 2 Solution: " << part2(input) << "\n";
	return 0;
}

/** Returns the 2020th number said.
 * Time complexity: O(n)
 * Space complexity: O(n)
*/
long part1(const input& input) {
	return simulate(input, 2020);
}

/** Returns the 30,000,000th number said.
 * Time complexity: O(n)
 * Space complexity: O(n)
*/
long part2(const input& input) {
	return simulate(input, 30000000);
}

/** Simulates n steps of the game.
 * Time complexity: O(n)
 * Space complexity: O(n)
 */
static long simulate(const input& input, long n) {
	long lastSpoken;
	std::unordered_map<long, std::vector<long>> seen;

	for (long turn = 0; turn < static_cast<long>(input.size()); turn++) {
		lastSpoken = input[turn];
		seen[lastSpoken].push_back(turn);
	}

	for (long turn = input.size(); turn < n; turn++) {
		long value;
		bool seenLast = seen[lastSpoken].size() > 1;

		if (seenLast) {
			const auto& history = seen[lastSpoken];
			value = history[history.size() - 1] - history[history.size() - 2];
		} else {
			value = 0;
		}

		seen[value].push_back(turn);
		lastSpoken = value;
	}

	return lastSpoken;
}
