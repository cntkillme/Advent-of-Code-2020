#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "dataset.hpp"

struct state {
	long acc;
	std::size_t pc;
	std::unordered_multiset<std::size_t> pc_history;
};

enum class opcode { acc, jmp, nop };

struct instruction {
	typename ::opcode opcode;
	long argument;
};

using input = std::vector<instruction>;

static std::unordered_map<std::string, opcode> mapping({
	{ "acc", opcode::acc },
	{ "jmp", opcode::jmp },
	{ "nop", opcode::nop }
});

static long part1(const input& input);
static long part2(const input& input);
static state execute(const input& input, std::size_t execInstrLimit);

int main() {
	auto stream = open_dataset("data/problem-8.txt");
	input input;

	while (stream.peek() != std::ifstream::traits_type::eof()) {
		std::string opcodeName;
		long argument;
		stream >> opcodeName >> argument;
		input.push_back({ mapping.at(opcodeName), argument });

		if (stream.peek() == '\n') {
			stream.get();
		}
	}

	std::cout << "Part 1 Solution: " << part1(input) << "\n";
	std::cout << "Part 2 Solution: " << part2(input) << "\n";
	return 0;
}

/** Return the accumulator right before any instruction is about to run twice.
 * Time complexity: O(n)
 * Space complexity: O(n)
*/
long part1(const input& input) {
	return execute(input, 1).acc;
}

/** Return the accumulator after replacing a single jmp/nop with a nop/jmp such that pc is at one after the end.
 * Time complexity: O(mn) [m = pc history limit]
 * Space complexity: O(mn)
*/
long part2(const input& input) {
	std::vector<instruction> copy(input);
	std::size_t pcHistoryLimit = 100;

	for (std::size_t i = 0; i < copy.size(); i++) {
		opcode opcode = copy[i].opcode;

		if (opcode == opcode::jmp || opcode == opcode::nop) {
			auto newOpcode = opcode == opcode::jmp ? opcode::nop : opcode::jmp;
			copy[i].opcode = newOpcode; // replace jmp/nop with nop/jmp
			auto state = execute(copy, pcHistoryLimit);
			copy[i].opcode = opcode; // undo change

			if (state.pc == input.size()) {
				return state.acc;
			}
		}
	}

	throw std::runtime_error("Part 2: No Solution!");
}

/** Executes the instructions, terminates if the pc history limit is passed for any pc.
 * Time complexity: O(mn) [m = pc history limit]
 * Space complexity: O(mn)
 */
state execute(const input& input, std::size_t pcHistoryLimit) {
	state state({ 0, 0, {} });

	while (state.pc < input.size()) {
		const auto& instruction = input[state.pc];

		if (state.pc_history.count(state.pc) >= pcHistoryLimit) {
			break;
		}

		state.pc_history.insert(state.pc);

		if (instruction.opcode == opcode::acc) {
			state.acc += instruction.argument;
		} else if (instruction.opcode == opcode::jmp) {
			state.pc += instruction.argument;
			continue;
		}

		state.pc += 1;
	}

	return state;
}
