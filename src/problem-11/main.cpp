#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include "dataset.hpp"

enum class seat_state { empty = 'L', occupied = '#', floor = '.' };

using state = std::vector<std::vector<seat_state>>;

static long part1(const state& state);
static long part2(const state& state);
static std::pair<bool, state> simulate_part1(const state& state);
static std::pair<bool, state> simulate_part2(const state& state);

int main() {
	auto stream = open_dataset("data/problem-11.txt");
	state state;
	std::string line;

	while (stream >> line) {
		std::vector<seat_state> row;

		for (char ch : line) {
			row.push_back(static_cast<seat_state>(ch));
		}

		state.push_back(std::move(row));
	}

	std::cout << "Part 1 Solution: " << part1(state) << "\n";
	std::cout << "Part 2 Solution: " << part2(state) << "\n";
	return 0;
}

/** Continuously simulates the state until there are no changes and yields the number of occupied seats.
 * Time complexity: O(mn) [where m = # of simulations]
 * Space complexity: O(mn)
*/
long part1(const state& state) {
	auto newState = state;

	while (true) {
		auto pair = simulate_part1(newState);
		newState = std::move(pair.second);

		if (!pair.first) {
			long occupied = 0;

			for (const auto& row : newState) {
				for (seat_state st : row) {
					if (st == seat_state::occupied) {
						occupied += 1;
					}
				}
			}

			return occupied;
		}
	}
}

/** Continuously simulates the state until there are no changes and yields the number of occupied seats.
 * Time complexity: O(mn^1.5) [where m = # of simulations]
 * Space complexity: O(mn^1.5)
*/
long part2(const state& state) {
	auto newState = state;

	while (true) {
		auto pair = simulate_part2(newState);
		newState = std::move(pair.second);

		if (!pair.first) {
			long occupied = 0;

			for (const auto& row : newState) {
				for (seat_state st : row) {
					if (st == seat_state::occupied) {
						occupied += 1;
					}
				}
			}

			return occupied;
		}
	}
}

/** Simulates the part 1 ruleset once and returns the state.
 * Time complexity: O(n)
 * Space complexity: O(n)
 */
std::pair<bool, state> simulate_part1(const state& state) {
	bool hasChanges = false;
	auto newState = state;

	auto getAdjacent = [&](long row, long col) {
		std::vector<seat_state> adjacent;
		adjacent.reserve(8);

		for (long i = row - 1; i <= row + 1; i++) {
			for (long j = col - 1; j <= col + 1; j++) {
				if (!(i == row && j == col)
					&& i >= 0 && i < static_cast<long>(state.size())
					&& j >= 0 && j < static_cast<long>(state[i].size())
				) {
					adjacent.push_back(state[i][j]);
				}
			}
		}

		return adjacent;
	};

	for (long i = 0; i < static_cast<long>(state.size()); i++) {
		for (long j = 0; j < static_cast<long>(state[i].size()); j++) {
			if (state[i][j] == seat_state::empty) {
				// Rule 1
				auto adjacent = getAdjacent(i, j);

				if (std::count(adjacent.begin(), adjacent.end(), seat_state::occupied) == 0) {
					newState[i][j] = seat_state::occupied;
					hasChanges = true;
				}
			} else if (state[i][j] == seat_state::occupied) {
				// Rule 2
				auto adjacent = getAdjacent(i, j);

				if (std::count(adjacent.begin(), adjacent.end(), seat_state::occupied) >= 4) {
					newState[i][j] = seat_state::empty;
					hasChanges = true;
				}
			}
		}
	}

	return { hasChanges, newState };
}

/** Simulates the part 2 ruleset once and returns the state.
 * Time complexity: O(n^1.5)
 * Space complexity: O(n)
 */
std::pair<bool, state> simulate_part2(const state& state) {
	bool hasChanges = false;
	auto newState = state;

	auto getAdjacent = [&](long row, long col) {
		std::vector<seat_state> adjacent;
		adjacent.reserve(8);

		for (long i = row - 1; i <= row + 1; i++) {
			for (long j = col - 1; j <= col + 1; j++) {
				if (!(i == row && j == col)
					&& i >= 0 && i < static_cast<long>(state.size())
					&& j >= 0 && j < static_cast<long>(state[i].size())
				) {
					if (state[i][j] == seat_state::floor) {
						// scan if floor
						long di = i - row;
						long dj = j - col;
						long i2 = i;
						long j2 = j;

						while (i2 >= 0 && i2 < static_cast<long>(state.size())
							&& j2 >= 0 && j2 < static_cast<long>(state[i2].size())
						) {
							if (state[i2][j2] != seat_state::floor) {
								adjacent.push_back(state[i2][j2]);
								break;
							}

							i2 += di;
							j2 += dj;
						}
					} else {
						adjacent.push_back(state[i][j]);
					}
				}
			}
		}

		return adjacent;
	};

	for (long i = 0; i < static_cast<long>(state.size()); i++) {
		for (long j = 0; j < static_cast<long>(state[i].size()); j++) {
			if (state[i][j] == seat_state::empty) {
				// Rule 1
				auto adjacent = getAdjacent(i, j);

				if (std::count(adjacent.begin(), adjacent.end(), seat_state::occupied) == 0) {
					newState[i][j] = seat_state::occupied;
					hasChanges = true;
				}
			} else if (state[i][j] == seat_state::occupied) {
				// Rule 2
				auto adjacent = getAdjacent(i, j);

				if (std::count(adjacent.begin(), adjacent.end(), seat_state::occupied) >= 5) {
					newState[i][j] = seat_state::empty;
					hasChanges = true;
				}
			}
		}
	}

	return { hasChanges, newState };
}
