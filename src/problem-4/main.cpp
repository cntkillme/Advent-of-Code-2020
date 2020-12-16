#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>
#include "dataset.hpp"

enum class field { byr, iyr, eyr, hgt, hcl, ecl, pid, cid };
using input_entry = std::unordered_map<field, std::string>;
using input = std::vector<input_entry>;

static std::unordered_map<std::string, field> mapping({
	{ "byr", field::byr },
	{ "iyr", field::iyr },
	{ "eyr", field::eyr },
	{ "hgt", field::hgt },
	{ "hcl", field::hcl },
	{ "ecl", field::ecl },
	{ "pid", field::pid },
	{ "cid", field::cid }
});

static std::size_t part1(const input& input);
static std::size_t part2(const input& input);
static bool validate_int(const std::string& value, long min, long max);

int main() {
	auto stream = open_dataset("data/problem-4.txt");
	input input;
	input_entry entry;
	std::string line;

	while (stream >> line) {
		// field:value
		std::size_t k = line.find(':');
		auto key = line.substr(0, k);
		auto value = line.substr(k + 1);
		entry[mapping.at(key)] = value;

		if (stream.peek() == '\n') {
			stream.get();

			// new passport entry
			if (stream.peek() == '\n' || stream.peek() == std::ifstream::traits_type::eof()) {
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

static std::unordered_map<field, bool(*)(const input_entry& entry)> validators({
	{
		field::byr,
		[](const input_entry& entry) {
			return validate_int(entry.at(field::byr), 1920, 2002);
		}
	},
	{
		field::iyr,
		[](const input_entry& entry) {
			return validate_int(entry.at(field::iyr), 2010, 2020);
		}
	},
	{
		field::eyr,
		[](const input_entry& entry) {
			return validate_int(entry.at(field::eyr), 2020, 2030);
		}
	},
	{
		field::hgt,
		[](const input_entry& entry) {
			const auto& hgt = entry.at(field::hgt);

			if (hgt.size() >= 3) {
				auto val = hgt.substr(0, hgt.size() - 2);
				auto unit = hgt.substr(hgt.size() - 2, 2);

				return unit == "cm"
					? validate_int(val, 150, 193)
					: unit == "in"
					? validate_int(val, 59, 76)
					: false;
			}

			return false;
		}
	},
	{
		field::hcl,
		[](const input_entry& entry) {
			const auto& hcl = entry.at(field::hcl);

			// #xxxxxx
			return hcl.size() == 7
				&& hcl[0] == '#'
				&& std::all_of(hcl.begin() + 1, hcl.end(), [](char ch) {
					return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f');
				});
		}
	},
	{
		field::ecl,
		[](const input_entry& entry) {
			const auto& ecl = entry.at(field::ecl);

			return ecl == "amb"
				|| ecl == "blu"
				|| ecl == "brn"
				|| ecl == "gry"
				|| ecl == "grn"
				|| ecl == "hzl"
				|| ecl == "oth";
		}
	},
	{
		field::pid,
		[](const input_entry& entry) {
			const auto& pid = entry.at(field::pid);

			// ddddddddd
			return pid.size() == 9
				&& std::all_of(pid.begin(), pid.end(), [](char ch) { return ch >= '0' && ch <= '9'; });
		}
	},
	{ field::cid, [](const input_entry& entry) { return true; } }
});

/** Count passports with all required fields.
 * Time complexity: O(n)
 * Space complexity: O(1)
*/
std::size_t part1(const input& input) {
	return std::accumulate(
		input.begin(),
		input.end(),
		static_cast<std::size_t>(0), [](std::size_t acc, const input_entry& entry) {
			return acc + (
				std::all_of(mapping.begin(), mapping.end(), [&entry](const auto& pair) {
					// cid optional
					return entry.count(pair.second) > 0 || pair.second == field::cid;
				}) ? 1 : 0
			);
		}
	);
}

/** Count passports with all required fields with valid values.
 * Time complexity: O(n)
 * Space complexity: O(1)
*/
std::size_t part2(const input& input) {
	return std::accumulate(
		input.begin(),
		input.end(),
		static_cast<std::size_t>(0),
		[](std::size_t acc, const input_entry& entry) {
			return acc + (
				std::all_of(mapping.begin(), mapping.end(), [&entry](const auto& pair) {
					// cid optional
					return entry.count(pair.second) > 0 ? validators[pair.second](entry) : pair.second == field::cid;
				}) ? 1 : 0
			);
		}
	);
}

bool validate_int(const std::string& value, long min, long max) {
	try {
		long res = std::stol(value);
		return res >= min && res <= max;
	} catch (...) {
		return false;
	}
}
