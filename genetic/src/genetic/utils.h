#pragma once
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <random>
#include <sstream>
#include <type_traits>

namespace utils {

using Matrix = std::vector<std::vector<std::size_t>>;

struct calculator {
public:
	calculator() = default;
	calculator(Matrix* data, Matrix* cost);
	double criterion(std::vector<std::size_t>* permutation);
	~calculator();
private:
	Matrix* data_volume;
	Matrix* transfer_cost;
};

std::set<std::size_t> get_unused(std::vector<std::size_t> items, std::size_t max_idx);

std::size_t
random(int last, int start = 0);

template<typename T>
class FileHandler {
public:
	std::vector<std::vector<T>>&
	GetMatrix() {
		file = std::fstream(filename, std::ios::in);
		while (!file.eof()) {
			T value = 0;
			matrix.emplace_back(std::vector<T>{});
			std::string str;
			std::getline(file, str);
			std::stringstream sstream(str);
			while (!sstream.eof()) {
				sstream >> value;
				(matrix.rbegin())->push_back(value);
			}
		}
		return matrix;
	}

	FileHandler(const char* filename) {
		this->filename = filename;
	}

	~FileHandler() {
		file.close();
	}

private:
	std::string filename;
	std::vector<std::vector<T>> matrix;
	std::fstream file;
};

}