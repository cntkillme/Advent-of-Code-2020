#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "dataset.hpp"

struct constraint {
	std::size_t bag;
	std::string name;
	std::unordered_set<std::size_t> parents;
	std::unordered_map<std::size_t, long> children;
};

using input = std::vector<constraint>;
using mapping = std::unordered_map<std::string, std::size_t>;

static std::size_t part1(const input& input, const mapping& mapping);
static std::size_t part2(const input& input, const mapping& mapping);

/** Grammar
 * constraint = bag "contains?" bag_list '\n';
 * bag = WORD WORD "bags?";
 * bag_list = LONG bag '.' | LONG bag ',' bag_list | "no other bags.";
 */
int main() {
	auto stream = open_dataset("data/problem-7.txt");
	input input;
	mapping mapping;

	auto recordBag = [&, id = static_cast<std::size_t>(0)](const std::string& name) mutable {
		if (mapping.count(name) == 0) {
			input.push_back({ id, name, {}, {} });
			mapping[name] = id;
			return id++;
		} else {
			return mapping.at(name);
		}
	};

	// bag = WORD WORD "bags?"
	auto parseBag = [&]() {
		std::string adjective, color, terminator;
		stream >> adjective >> color >> terminator;
		std::string name = adjective + " " + color;
		char last = terminator.at(terminator.size() - 1);

		if (last == ',' || last == '.') {
			stream.unget();
		}

		return recordBag(name);
	};

	// bag_list = LONG bag '.' | LONG bag ',' bag_list | "no other bags."
	auto parseBagList = [&]() {
		std::unordered_map<std::size_t, long> children;
		std::string count;

		stream >> count;

		if (count == "no") {
			// "no other bags."
			stream >> count; // skip "other"
			stream >> count; // skip "bags."
		} else {
			auto bag = parseBag();

			while (stream.peek() == ',') {
				// LONG bag ','
				children[bag] = std::stol(count);
				stream.get(); // skip ','
				stream >> count;
				bag = parseBag();
			}

			// LONG BAG '.'
			children.insert({ bag, std::stol(count) });
			children[bag] = std::stol(count);
			stream.get(); // skip '.'
		}

		return children;
	};

	// constraint = bag "contains?" bag_list '\n';
	auto parseConstraint = [&]() {
		std::string temp;
		auto bag = parseBag();
		stream >> temp; // skip "contains?"
		input.at(bag).children = parseBagList();
		stream.ignore(1, '\n'); // skip '\n'

		// assign parents
		std::for_each(
			input.at(bag).children.begin(),
			input.at(bag).children.end(),
			[&](const auto& pair) { input.at(pair.first).parents.insert(bag); }
		);
	};

	while (stream.peek() != std::ifstream::traits_type::eof()) {
		parseConstraint();

		if (stream.peek() == '\n') {
			stream.get();
		}
	}

	std::cout << "Part 1 Solution: " << part1(input, mapping) << "\n";
	std::cout << "Part 2 Solution: " << part2(input, mapping) << "\n";
	return 0;
}

/** Counts the number of bags that eventually contain one shiny gold bag.
 * Runtime complexity: O(n)
 * Space complexity: O(n)
*/
std::size_t part1(const input& input, const mapping& mapping) {
	std::unordered_set<std::size_t> ancestors;
	std::vector<std::size_t> scan;
	scan.push_back(mapping.at("shiny gold"));

	// get ancestors (no cycles)
	while (!scan.empty()) {
		const auto& constraint = input.at(scan.back());
		scan.pop_back();

		for (const auto& parent : constraint.parents) {
			if (ancestors.count(parent) == 0) {
				ancestors.insert(parent);
				scan.push_back(parent);
			}
		}
	}

	return ancestors.size();
}

/** Counts the total number of bags that one shiny gold bag will contain.
 * Runtime complexity: O(n)
 * Space complexity: O(n)
*/
std::size_t part2(const input& input, const mapping& mapping) {
	std::vector<decltype(constraint::children)::value_type> scan;
	std::size_t total = 0;
	scan.emplace_back(mapping.at("shiny gold"), 1);

	// get descendants and count (no cycles)
	while (!scan.empty()) {
		auto [bag, count] = scan.back();
		total += count;
		scan.pop_back();

		// add children bag count
		for (auto [child, childCount] : input.at(bag).children) {
			std::size_t totalCount = count * childCount;
			scan.emplace_back(child, totalCount);
		}
	}

	// exclude the shiny gold bag itself
	return total - 1;
}
