#pragma once

#include <fstream>
#include <stdexcept>
#include <string>

inline std::ifstream open_dataset(const std::string& path) {
	std::ifstream file(path);

	if (!file) {
		throw std::runtime_error("could not open file: " + path);
	}

	return file;
}
