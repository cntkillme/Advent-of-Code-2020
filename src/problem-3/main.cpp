#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include "dataset.hpp"

using input = std::vector<std::string>;

static long part1(const input& input);
static long part2(const input& input);
static long count_trees(const input& input, int vx, int vy);

int main() {
	auto stream = open_dataset("data/problem-3.txt");
	input input;
	std::string line;

	while (stream >> line) {
		input.push_back(line);
	}

	std::cout << "Part 1 Solution: " << part1(input) << "\n";
	std::cout << "Part 2 Solution: " << part2(input) << "\n";
	return 0;
}

/** Tree count of horizontally wrapping map with movement 3 right, 1 down.
 * Runtime complexity: O(n)
 * Space complexity: O(1)
*/
long part1(const input& input) {
	return count_trees(input, 3, 1);
}

/** Product of tree counts of horizontally wrapping map of movements:
 * - 1 right, 1 down;
 * - 3 right, 1 down;
 * - 5 right, 1 down;
 * - 7 right, 1 down;
 * - 1 right, 2 down
 * Runtime complexity: O(n)
 * Space complexity: O(1)
*/
long part2(const input& input) {
	return count_trees(input, 1, 1)
		* count_trees(input, 3, 1)
		* count_trees(input, 5, 1)
		* count_trees(input, 7, 1)
		* count_trees(input, 1, 2);
}

/**
 * Tree count of horizontally wrapping map with movement vx right, vy down.
 * Runtime complexity: O(n)
 * Spacea complexity: O(1)
 */
long count_trees(const input& input, int vx, int vy) {
	long trees = 0;
	std::size_t cols = input[0].size();
	std::size_t x = 0;
	std::size_t y = 0;

	while (y < input.size() - vy) {
		x = (x + vx) % cols;
		y += vy;

		if (input[y][x] == '#') {
			trees += 1;
		}
	}

	return trees;
}
