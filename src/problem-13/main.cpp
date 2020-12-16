#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>
#include "dataset.hpp"

struct input {
	long timestamp;
	std::vector<long> active;
};

static long part1(const input& input);
static long part2(const input& input);

int main() {
	auto stream = open_dataset("data/problem-13.txt");
	input input;
	long value;

	stream >> input.timestamp;

	while (stream >> value) {
		input.active.push_back(value);
		stream.ignore(); // skip comma

		while (std::isalpha(stream.peek())) {
			// x comma
			input.active.push_back(-1);
			stream.ignore();
			stream.ignore();
		}
	}

	std::cout << "Part 1 Solution: " << part1(input) << "\n";
	std::cout << "Part 2 Solution: " << part2(input) << "\n";
	return 0;
}

/** The product of the first active bus ID and the time delta.
 * Time complexity: O(n^2)
 * Space complexity: O(1)
*/
long part1(const input& input) {
	long time = input.timestamp;

	while (true) {
		for (long bus : input.active) {
			if (bus >= 0 && time % bus == 0) {
				return bus * (time - input.timestamp);
			}
		}

		time += 1;
	}
}

/** The first timestamp such that each bus is active i minutes after the timestamp.
 * Time complexity: O(mn^2) ??
 * Space complexity: O(1)
*/
long part2(const input& input) {
	long time = input.active[0];
	long step = input.active[0];
	std::size_t current = 1;

	while (true) {
		bool valid = true;

		// skip x's
		while (current < input.active.size() && input.active[current] == -1) {
			current += 1;
		}

		// check if current timestamp is valid
		for (std::size_t idx = 0; idx <= current; idx++) {
			long bus = input.active[idx];

			if (bus >= 0 && (time + idx) % bus != 0) {
				valid = false;
				break;
			}
		}

		if (valid) {
			if (current == input.active.size() - 1) {
				return time;
			}

			step *= input.active[current];
			current += 1;
		}

		time += step;
	}

	return -1;
}
