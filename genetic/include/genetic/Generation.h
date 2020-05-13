#pragma once
#include "utils.h"

namespace genetic {

class BaseGeneration {
public:
	BaseGeneration();
	void init(std::vector<std::size_t>& base_permutation, std::size_t individ_size, std::size_t population_size);
	virtual std::vector<std::vector<std::size_t>>* exec() = 0;
protected:
	std::vector<std::size_t> base_permutation;
	std::size_t individ_size;
	std::size_t population_size;
};

class RandomGeneration : public BaseGeneration {
public:
	using BaseGeneration::BaseGeneration;
	std::vector<std::vector<std::size_t>>* exec();
};

}