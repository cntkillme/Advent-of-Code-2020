#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>
#include "dataset.hpp"

struct input_entry {
	std::size_t id;
	std::size_t row;
	std::size_t column;
};

using input = std::vector<input_entry>;

static std::size_t part1(const input& input);
static std::size_t part2(const input& input);
static std::size_t bsp(const std::string& data, char lowerChar);

int main() {
	auto stream = open_dataset("data/problem-5.txt");
	input input;
	std::string line;

	while (stream >> line) {
		input_entry entry;
		entry.row = bsp(line.substr(0, 7), 'F');
		entry.column = bsp(line.substr(7), 'L');
		entry.id = (8 * entry.row) + entry.column;
		input.push_back(std::move(entry));
	}

	std::cout << "Part 1 Solution: " << part1(input) << "\n";
	std::cout << "Part 2 Solution: " << part2(input) << "\n";
	return 0;
}

/** Returns the highest seat ID.
 * Runtime complexity: O(n)
 * Space complexity: O(1)
*/
std::size_t part1(const input& input) {
	return std::max_element(input.begin(), input.end(), [](const input_entry& left, const input_entry& right) {
		return left.id < right.id;
	})->id;
}

/** Returns an empty seat ID after the first seat ID.
 * Runtime complexity: O(n^2)
 * Space complexity: O(n)
*/
std::size_t part2(const input& input) {
	typename ::input sorted(input);

	std::sort(sorted.begin(), sorted.end(), [](const auto& left, const auto& right) {
		return left.id < right.id;
	});

	for (std::size_t i = 1; i < sorted.size(); i++) {
		if (sorted[i].id != sorted[i - 1].id + 1) {
			return sorted[i - 1].id + 1;
		}
	}

	throw new std::runtime_error("Part 2: No Solution!");
}

/** Returns the underlying value of a string (binary space partitioning).
 * Runtime complexity: O(n)
 * Space complexity: O(1)
 */
std::size_t bsp(const std::string& data, char lowerChar) {
	std::size_t i = 0;
	std::size_t j = (1 << data.size()) - 1;

	for (char ch : data) {
		std::size_t k = (i + j) / 2;

		if (ch == lowerChar) {
			j = k;
		} else {
			i = 1 + k;
		}
	}

	return j;
}
