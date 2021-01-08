#include "utils.h"
#include <stdexcept>

namespace utils {

randomizer::randomizer() {
	generator = std::mt19937(rd());
	range = std::uniform_int_distribution<std::size_t>(static_cast<std::size_t>(0), UINT32_MAX);
}

randomizer::~randomizer() {}

std::size_t randomizer::operator()(std::size_t max, std::size_t min) {
	assert(max >= min);
	if (max == min) return min;
	// return min + range(generator) % (max - min);
	return min + UINT32_MAX % (max - min);
}

calculator::calculator(matrix<std::size_t>* data, matrix<std::size_t>* cost) : data_volume(*data), transfer_cost(*cost) {}

std::size_t calculator::criterion(permutation& permutation) {
	std::size_t result = 0;
	if (data_volume.rows() == transfer_cost.rows() && transfer_cost.rows() >= permutation.determined_size()) {
		for (std::size_t row = 0; row < permutation.determined_size(); ++row) {
			for (std::size_t column = row; column < permutation.determined_size(); ++column) {
				result += data_volume(row, column) * transfer_cost(permutation.get(row), permutation.get(column));
			}
		}
		return result * 2;
	}
	else {
		throw std::runtime_error("The permutation length is greater than the dimension of the matrix (or size of data_volume not equal to size of transfer_cost)");
	}
}

calculator::~calculator() { }


} //namespace utils