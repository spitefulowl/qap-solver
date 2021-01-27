#include "genetic/gen_alg.h"

namespace genetic {

gen_alg::gen_alg(base_generation* generation, base_crossover* crossover, base_mutation* mutation, base_selection* selection,
	utils::matrix_t* data_volume, utils::matrix_t* transfer_cost,
	std::size_t iterations, std::size_t population_size, std::size_t probability, std::size_t beta) :
		generation(generation), crossover(crossover), mutation(mutation), selection(selection),
		data_volume(*data_volume), transfer_cost(*transfer_cost), iterations(iterations), population_size(population_size) , probability(probability), beta(beta), my_calculator(nullptr) {}

void gen_alg::execute(permutation& base_permutation) {
	if (!my_calculator) my_calculator = new utils::calculator(&data_volume, &transfer_cost);
	generation->init(base_permutation, population_size);
	std::size_t start_idx = base_permutation.size();
	auto& my_population = generation->exec();
	for (std::size_t iter = 0; iter < iterations; ++iter) {
		crossover->init(my_population, start_idx);
		auto& descendants = crossover->exec();
		mutation->init(descendants, probability, start_idx);
		mutation->exec();
		selection->init(&data_volume, &transfer_cost, my_population, descendants, beta);
		selection->exec();
	}
	auto& result = *std::min_element(my_population.begin(), my_population.end(), [this](auto& first, auto& second) {
		std::size_t first_criterion = (*this->my_calculator).criterion(first);
		std::size_t second_criterion = (*this->my_calculator).criterion(second);
		return first_criterion < second_criterion;
	});
	result.copy_to(base_permutation);
}

gen_alg::~gen_alg() {
	delete generation;
	delete crossover;
	delete mutation;
	delete selection;
}

}
