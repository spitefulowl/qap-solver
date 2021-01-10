#pragma once
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <random>
#include <sstream>
#include <type_traits>
#include <assert.h>

#include "permutation.h"

namespace utils {

template<typename T>
struct alignas(qap_alignment) matrix {
public:
	matrix(std::size_t rows, std::size_t columns) :
		my_rows(rows), my_columns(columns) {

		my_data.resize(rows * columns);
	}
	T& operator()(std::size_t row, std::size_t column) {
		return my_data[row * my_columns + column];
	}
	std::size_t rows() { return my_rows; }
	std::size_t columns() { return my_columns; }
private:
	alignas(qap_alignment) std::size_t my_rows;
	alignas(qap_alignment) std::size_t my_columns;
	alignas(qap_alignment) std::vector<T, vector_allocator<T>> my_data;
};

using matrix_t = utils::matrix<std::size_t>;

struct alignas(qap_alignment) calculator {
public:
	calculator(utils::matrix_t* data, utils::matrix_t* cost);
	std::size_t criterion(permutation& permutation);
	~calculator();
private:
	utils::matrix_t& data_volume; // обмен данными между подзадачами
	utils::matrix_t& transfer_cost; // стоимость обмена данными между вычислителями
};

struct randomizer {
public:
	randomizer();
	~randomizer();
	std::size_t operator()(std::size_t max, std::size_t min = 0);
private:
	std::random_device rd;
	std::mt19937 generator;
	std::uniform_int_distribution<std::size_t> range;
};

template<typename T>
class matrix_reader {
public:
	matrix<T>* get_matrix() {
		file = std::fstream(filename, std::ios::in);
		std::vector<T> read_values;
		std::size_t row = 0;
		while (!file.eof()) {
			T value = 0;
			std::string current_line;
			std::getline(file, current_line);
			current_line.erase(std::remove(current_line.begin(), current_line.end(), '\n'), current_line.end());
			current_line.erase(std::remove(current_line.begin(), current_line.end(), '\r'), current_line.end());
			std::stringstream sstream(current_line);
			while (!sstream.eof()) {
				sstream >> value;
				read_values.push_back(value);
			}
			if (!my_matrix) {
				my_matrix = new matrix<T>(read_values.size(), read_values.size());
			}
			for (std::size_t column = 0; column < read_values.size(); ++column) {
				(*my_matrix)(row, column) = read_values[column];
			}
			read_values.clear();
			row++;
		}
		return my_matrix;
	}

	matrix_reader(const char* filename) {
		this->filename = filename;
		my_matrix = nullptr;
	}

	~matrix_reader() {
		file.close();
	}

private:
	std::string filename;
	matrix<T>* my_matrix;
	std::fstream file;
};

}
