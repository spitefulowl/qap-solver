#include "genetic/GenAlg.h"

namespace genetic {

GenAlg::GenAlg(BaseGeneration* generation, BaseCrossover* crossover, BaseMutation* mutation, BaseSelection* selection,
	utils::Matrix* data_volume, utils::Matrix* transfer_cost,
	std::size_t iterations, std::size_t population_size, std::size_t probability, std::size_t beta) :
		generation(generation), crossover(crossover), mutation(mutation), selection(selection), 
		data_volume(data_volume), transfer_cost(transfer_cost), iterations(iterations), population_size(population_size) , probability(probability), beta(beta) {}

std::vector<std::size_t> GenAlg::execute(std::vector<std::size_t> base_permutation, std::size_t individ_size) {
	utils::calculator calc(data_volume, transfer_cost);
	generation->init(base_permutation, individ_size, population_size);
	std::vector<std::vector<std::size_t>>* my_population = nullptr;
	std::size_t start_idx = base_permutation.size();
	std::vector<std::size_t> result;
	for (std::size_t idx = 0; idx < 1; ++idx) {
		my_population = generation->exec();
		for (std::size_t iter = 0; iter < iterations; ++iter) {
			crossover->init(my_population, start_idx);
			crossover->exec();
			auto* descendants = crossover->get_descendants();
			mutation->init(descendants, probability, start_idx);
			mutation->exec();
			selection->init(data_volume, transfer_cost, my_population, descendants, beta);
			auto* my_population_tmp = selection->exec();
			delete descendants;
			delete my_population;
			my_population = my_population_tmp;
		}
		if (result.size() > 0) {
			my_population->push_back(result);
		}
		result = *std::min_element(my_population->begin(), my_population->end(), [&calc](auto& first, auto& second) {
			double first_c = calc.criterion(&first);
			double second_c = calc.criterion(&second);
			return first_c < second_c;
			});
		delete my_population;
	}
	return result;
}

GenAlg::~GenAlg() {
	delete generation;
	delete crossover;
	delete mutation;
	delete selection;
}

}