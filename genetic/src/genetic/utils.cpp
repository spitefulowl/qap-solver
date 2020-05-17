#include "genetic/utils.h"
#include <stdexcept>
std::random_device rd;
std::mt19937 generator(rd());
std::uniform_int_distribution<std::size_t> range(0, 10000);

namespace std {

} //namespace std

namespace utils {

std::set<std::size_t> get_unused(std::vector<std::size_t> items, std::size_t max_idx) {
	std::set<std::size_t> result;
	for (std::size_t idx = 0; idx < max_idx; ++idx) {
		result.insert(idx);
	}
	for (std::size_t idx = 0; idx < items.size(); ++idx) {
		result.erase(items[idx]);
	}
	return result;
}

std::size_t
random(int last, int start) {
	//std::uniform_int_distribution<std::size_t> range(start, last - 1);
	return start + range(generator) % (last - start);
}

calculator::calculator(Matrix* data, Matrix* cost) : data_volume(data), transfer_cost(cost) {}

double calculator::criterion(std::vector<std::size_t>* permutation) {
	double result = 0;
	if (data_volume && transfer_cost) {
		if (data_volume->size() == transfer_cost->size() && transfer_cost->size() >= permutation->size()) {
			for (std::size_t row = 0; row < permutation->size(); ++row) {
				for (std::size_t column = row; column < permutation->size(); ++column) {
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


} //namespace utils