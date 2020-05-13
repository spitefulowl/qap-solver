#include "helpers.h"
#include <stdexcept>

randomizer::randomizer() {
	generator = std::mt19937(rd());
}

randomizer::~randomizer() {}

std::size_t randomizer::operator()(std::size_t max) {
	std::uniform_int_distribution<std::size_t> range(static_cast<std::size_t>(0), max - 1);
	return range(generator);
}

calculator::calculator(Matrix* data, Matrix* cost) : data_volume(data), transfer_cost(cost) {}

double calculator::criterion(permutation* permutation) {
	double result = 0;
	if (data_volume && transfer_cost) {
		if (data_volume->size() == transfer_cost->size() && transfer_cost->size() >= permutation->determined_size()) {
			for (std::size_t row = 0; row < permutation->determined_size(); ++row) {
				for (std::size_t column = row; column < permutation->determined_size(); ++column) {
					result += (*data_volume)[row][column] * (*transfer_cost)[(*permutation)[row]][(*permutation)[column]];
				}
			}
			return result * 2;
		}
		else {
			throw std::runtime_error("The permutation length is greater than the dimension of the matrix (or size of data_volume not equal to size of transfer_cost)");
		}
	}
	else {
		throw std::runtime_error("Not all fields are initialized");
	}
}

calculator::~calculator() {
}
