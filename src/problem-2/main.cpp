#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "dataset.hpp"

struct input_entry {
	std::size_t min;
	std::size_t max;
	char letter;
	std::string password;
};

using input = std::vector<input_entry>;

static std::size_t part1(const input& input);
static std::size_t part2(const input& input);

int main() {
	auto stream = open_dataset("data/problem-2.txt");
	input input;

	while (stream.peek() != std::ifstream::traits_type::eof()) {
		// 1-2 a: password
		input_entry entry;
		stream >> entry.min;
		stream.ignore(); // skip dash
		stream >> entry.max;
		stream.ignore(); // skip space
		stream >> entry.letter;
		stream.ignore(2); // skip colon, skip space
		stream >> entry.password;
		input.push_back(std::move(entry));

		if (stream.peek() == '\n') {
			stream.get();
		}
	}

	std::cout << "Part 1 Solution: " << part1(input) << "\n";
	std::cout << "Part 2 Solution: " << part2(input) << "\n";
	return 0;
}

/** Count valid passwords where letter occurs between min to max times.
 * Time complexity: O(mn)
 * Space complexity: O(1)
*/
std::size_t part1(const input& input) {
	auto isValid = [](const input_entry& entry) {
		std::size_t cnt = std::count(entry.password.begin(), entry.password.end(), entry.letter);
		return cnt >= entry.min && cnt <= entry.max;
	};

	return std::accumulate(
		input.begin(),
		input.end(),
		static_cast<std::size_t>(0),
		[&isValid](std::size_t total, const input_entry& entry) {
			return total + (isValid(entry) ? 1 : 0);
		}
	);
}

/** Count valid passwords where letter occurs exactly once at position min and position max.
 * Time complexity: O(n)
 * Space complexity: O(1)
*/
std::size_t part2(const input& input) {
	auto isValid = [](const input_entry& entry) {
		bool a = entry.password[entry.min - 1] == entry.letter;
		bool b = entry.password[entry.max - 1] == entry.letter;
		return a != b;
	};

	return std::accumulate(
		input.begin(),
		input.end(),
		static_cast<std::size_t>(0),
		[&isValid](std::size_t total, const input_entry& entry) {
			return total + (isValid(entry) ? 1 : 0);
		}
	);
}
