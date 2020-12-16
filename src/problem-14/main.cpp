#include <iostream>
#include <numeric>
#include <unordered_map>
#include <utility>
#include <vector>
#include "dataset.hpp"

struct entry {
	unsigned long or_mask;
	unsigned long and_mask;
	std::vector<std::pair<long, long>> writes;
};

struct state {
	std::vector<entry> entries;
};

static long part1(const state& state);
static long part2(const state& state);

int main() {
	auto stream = open_dataset("data/problem-14.txt");
	state state;
	entry entry;
	std::string line;

	while (std::getline(stream, line)) {
		if (line.substr(0, 4) == "mask") {
			// mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
			if (entry.writes.size() > 0) {
				state.entries.push_back(std::move(entry));
				entry.writes.clear();
			}

			line = line.substr(7);
			entry.or_mask = 0;
			entry.and_mask = 0;

			for (std::size_t i = 0; i < 36; i++) {
				std::size_t j = 35 - i;

				if (line[j] == 'X') {
					entry.and_mask |= 1UL << i;
				} else if (line[j] == '1') {
					entry.or_mask |= 1UL << i;
				}
			}		} else {
			// mem[idx] = val
			std::size_t i = line.find_first_of('[') + 1;
			std::size_t j = line.find_first_of(']');
			std::size_t k = line.find_first_of('=') + 2;
			std::pair<long, long> kv({ std::stol(line.substr(i, j - i)), std::stol(line.substr(k)) });
			entry.writes.push_back(std::move(kv));
		}

		if (entry.writes.size() > 0) {
			state.entries.push_back(std::move(entry));
		}
	}

	std::cout << "Part 1 Solution: " << part1(state) << "\n";
	std::cout << "Part 2 Solution: " << part2(state) << "\n";
	return 0;
}

/** Sum of all values after completion of ruleset 1.
 * Time complexity: O(n)
 * Space complexity: O(n)
*/
long part1(const state& state) {
	std::unordered_map<long, long> memory;

	for (const auto& entry : state.entries) {
		for (const auto& write : entry.writes) {
			memory[write.first] = (write.second & entry.and_mask) | entry.or_mask;
		}
	}

	return std::accumulate(memory.begin(), memory.end(), 0L, [](long acc, const std::pair<long, long>& pair) {
		return acc + pair.second;
	});
}

/** Sum of all values after completion of ruleset 2.
 * Time complexity: O(n)
 * Space complexity: O(n)
*/
#include <bitset>
long part2(const state& state) {
	std::unordered_map<long, long> memory;
	std::vector<long> floating;
	floating.reserve(36);

	for (const auto& entry : state.entries) {
		for (std::size_t i = 0; i < 36; i++) {
			// entry.and_masks comprises of all floating bits
			if (((entry.and_mask >> i) & 1) != 0) {
				floating.push_back(1UL << i);
			}
		}

		for (const auto& write : entry.writes) {
			long base = (write.first & ~entry.and_mask) | entry.or_mask;

			// permute floating bits
			// iterate from [0, 2^k) and treat each bit as a selection of its corresponding value in floating
			for (std::size_t i = 0; i < (1UL << floating.size()); i++) {
				long value = 0;

				for (std::size_t j = 0; j < floating.size(); j++) {
					value |= floating[j] * ((i >> j) & 1);
				}

				memory[base | value] = write.second;
			}
		}

		floating.clear();
	}

	return std::accumulate(memory.begin(), memory.end(), 0L, [](long acc, const std::pair<long, long>& pair) {
		return acc + pair.second;
	});
}
