#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "dataset.hpp"

struct input_entry {
	int min;
	int max;
	char letter;
	std::string password;
};

using input = std::vector<input_entry>;

static long part1(const input& input);
static long part2(const input& input);

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
 * Runtime complexity: O(mn)
 * Space complexity: O(1)
*/
long part1(const input& input) {
	auto isValid = [](const input_entry& entry) {
		auto cnt = std::count(entry.password.begin(), entry.password.end(), entry.letter);
		return cnt >= entry.min && cnt <= entry.max;
	};

	return std::accumulate(
		input.begin(),
		input.end(),
		0L,
		[&isValid](long total, const input_entry& entry) {
			return total + (isValid(entry) ? 1 : 0);
		}
	);
}

/** Count valid passwords where letter occurs exactly once at position min and position max.
 * Runtime complexity: O(n)
 * Space complexity: O(1)
*/
long part2(const input& input) {
	auto isValid = [](const input_entry& entry) {
		bool a = entry.password[entry.min - 1] == entry.letter;
		bool b = entry.password[entry.max - 1] == entry.letter;
		return a != b;
	};

	return std::accumulate(
		input.begin(),
		input.end(),
		0L,
		[&isValid](long total, const input_entry& entry) {
			return total + (isValid(entry) ? 1 : 0);
		}
	);
}
