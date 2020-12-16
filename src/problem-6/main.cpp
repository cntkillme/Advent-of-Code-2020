#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>
#include "dataset.hpp"

using input_entry = std::vector<std::unordered_set<char>>;
using input = std::vector<input_entry>;

static std::size_t part1(const input& input);
static std::size_t part2(const input& input);

int main() {
	auto stream = open_dataset("data/problem-6.txt");
	input input;
	input_entry entry;
	std::string line;

	while (stream >> line) {
		entry.push_back(std::unordered_set<char>(line.begin(), line.end()));

		if (stream.peek() == '\n') {
			stream.get();

			// new group entry
			if (stream.peek() == '\n' || stream.peek() == -1) {
				stream.get();
				input.push_back(std::move(entry));
				entry.clear();
			}
		}
	}

	std::cout << "Part 1 Solution: " << part1(input) << "\n";
	std::cout << "Part 2 Solution: " << part2(input) << "\n";
	return 0;
}

/** Sum of unique answers of each group.
 * Time complexity: O(mn)
 * Space complexity: O(m)
*/
std::size_t part1(const input& input) {
	return std::accumulate(
		input.begin(),
		input.end(),
		static_cast<std::size_t>(0), [](std::size_t acc, const input_entry& entry) {
			std::unordered_set<char> uniqueAnswers;

			std::for_each(entry.begin(), entry.end(), [&uniqueAnswers](const auto& answers) {
				uniqueAnswers.insert(answers.begin(), answers.end());
			});

			return acc + uniqueAnswers.size();
		}
	);
}

/** Sum of unique answers of each group shared by all members of that group.
 * Time complexity: O()
 * Space complexity: O()
*/
std::size_t part2(const input& input) {
	return std::accumulate(
		input.begin(),
		input.end(),
		static_cast<std::size_t>(0), [](std::size_t acc, const input_entry& entry) {
			std::unordered_set<char> uniqueAnswers(entry[0]);

			std::for_each(entry.begin(), entry.end(), [&uniqueAnswers](const auto& answers) {
				for (auto iter = uniqueAnswers.begin(); iter != uniqueAnswers.end();) {
					if (answers.count(*iter) == 0) {
						iter = uniqueAnswers.erase(iter);
					} else {
						++iter;
					}
				}
			});

			return acc + uniqueAnswers.size();
		}
	);
}
