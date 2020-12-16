#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "dataset.hpp"

struct command {
	char direction;
	long count;
};

using input = std::vector<command>;

static long part1(const input& input);
static long part2(const input& input);

int main() {
	auto stream = open_dataset("data/problem-12.txt");
	input input;
	std::string line;

	while (stream >> line) {
		char direction = line[0];
		long count = std::strtol(line.data() + 1, nullptr, 10);
		input.push_back({ direction, count });
	}

	std::cout << "Part 1 Solution: " << part1(input) << "\n";
	std::cout << "Part 2 Solution: " << part2(input) << "\n";
	return 0;
}

/** Calculate the manhattan distance between ship's final location.
 * Time complexity: O(n)
 * Space complexity: O(1)
*/
long part1(const input& input) {
	long x = 0;
	long y = 0;
	long dx = 1;
	long dy = 0;

	for (const command& cmd : input) {
		switch (cmd.direction) {
			case 'N': y += cmd.count; break;
			case 'S': y -= cmd.count; break;
			case 'E': x += cmd.count; break;
			case 'W': x -= cmd.count; break;
			case 'F': x += cmd.count * dx; y += cmd.count * dy; break;
			default: { // L R
				long k = cmd.count;

				for (; k > 0; k -= 90) {
					long temp = dx;

					if (cmd.direction == 'L') {
						// (dx + dyi)i = -dy + dxi
						dx = -dy;
						dy = temp;
					} else {
						// (dx + dyi)-i = dy - dxi
						dx = dy;
						dy = -temp;
					}
				}
			} break;
		}
	}

	return std::abs(x) + std::abs(y);
}

/** Calculate the manhattan distance between ship's final location.
 * Time complexity: O(n)
 * Space complexity: O(1)
*/
long part2(const input& input) {
	long x = 0;
	long y = 0;
	long dx = 10;
	long dy = 1;

	for (const command& cmd : input) {
		switch (cmd.direction) {
			case 'N': dy += cmd.count; break;
			case 'S': dy -= cmd.count; break;
			case 'E': dx += cmd.count; break;
			case 'W': dx -= cmd.count; break;
			case 'F': x += cmd.count * dx; y += cmd.count * dy; break;
			default: { // L R
				long k = cmd.count;

				for (; k > 0; k -= 90) {
					long temp = dx;

					if (cmd.direction == 'L') {
						// (dx + dyi)i = -dy + dxi
						dx = -dy;
						dy = temp;
					} else {
						// (dx + dyi)-i = dy - dxi
						dx = dy;
						dy = -temp;
					}
				}
			} break;
		}
	}

	return std::abs(x) + std::abs(y);
}
