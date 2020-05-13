#include "genetic/Generation.h"

namespace genetic {

BaseGeneration::BaseGeneration() : population_size(0) {}

void BaseGeneration::init(std::vector<std::size_t>& base_permutation, std::size_t individ_size, std::size_t population_size) {
	this->base_permutation = std::vector<std::size_t>(base_permutation);
	this->individ_size = individ_size;
	this->population_size = population_size;
}

std::vector<std::vector<std::size_t>>* RandomGeneration::exec() {
	utils::Matrix* result_generation = new utils::Matrix;
	auto unused = utils::get_unused(base_permutation, individ_size);
	std::size_t start_idx = base_permutation.size();
	std::vector<std::size_t> base_result(individ_size);
	std::copy(base_permutation.begin(), base_permutation.end(), base_result.begin());
	std::copy(unused.begin(), unused.end(), base_result.begin() + start_idx);
	for (std::size_t idx = 0; idx < population_size; ++idx) {
		std::vector<std::size_t> result(base_result);
		std::random_shuffle(result.begin() + start_idx, result.end());
		result_generation->push_back(result);
	}
	return result_generation;
}

}