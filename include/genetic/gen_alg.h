#pragma once
#include "genetic/generation.h"
#include "genetic/crossover.h"
#include "genetic/mutation.h"
#include "genetic/selection.h"
#include "utils.h"
#include <fstream>

namespace genetic {

class gen_alg {
public:
	gen_alg(base_generation* generation, base_crossover* crossover, base_mutation* mutation, base_selection* selection,
		utils::matrix_t* data_volume, utils::matrix_t* transfer_cost,
		std::size_t iterations, std::size_t population_size, std::size_t probability, std::size_t beta);
	void execute(permutation& base_permutation);
	~gen_alg();
private:
	utils::matrix_t& data_volume;
	utils::matrix_t& transfer_cost;
	utils::calculator* my_calculator;
	base_generation* generation;
	base_crossover* crossover;
	base_mutation* mutation;
	base_selection* selection;
	std::size_t iterations;
	std::size_t population_size;
	std::size_t probability;
	std::size_t beta;
};

}
