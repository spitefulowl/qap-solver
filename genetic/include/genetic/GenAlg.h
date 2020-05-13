#pragma once
#include "Generation.h"
#include "Crossover.h"
#include "Mutation.h"
#include "Selection.h"
#include <fstream>

namespace genetic {

class GenAlg {
public:
	GenAlg(BaseGeneration* generation, BaseCrossover* crossover, BaseMutation* mutation, BaseSelection* selection,
		utils::Matrix* data_volume, utils::Matrix* transfer_cost,
		std::size_t iterations, std::size_t population_size, std::size_t probability, std::size_t beta);
	std::vector<std::size_t> execute(std::vector<std::size_t> base_permutation, std::size_t individ_size);
	~GenAlg();
private:
	utils::Matrix* data_volume;
	utils::Matrix* transfer_cost;
	BaseGeneration* generation;
	BaseCrossover* crossover;
	BaseMutation* mutation;
	BaseSelection* selection;
	std::size_t iterations;
	std::size_t population_size;
	std::size_t probability;
	std::size_t beta;
};

}
