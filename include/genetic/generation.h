#pragma once
#include "utils.h"

namespace genetic {

class base_generation {
public:
	base_generation();
	virtual void init(permutation& base_permutation, std::size_t population_size);
	virtual std::vector<permutation>& exec() = 0;
protected:
	permutation* my_base_permutation;
	std::vector<permutation> my_population;
	std::size_t my_population_size;
};

class random_generation : public base_generation {
public:
	using base_generation::base_generation;
	virtual void init(permutation& base_permutation, std::size_t population_size) override;
	virtual std::vector<permutation>& exec() override;
private:
	std::vector<std::size_t> shuffled_values;
	utils::randomizer random;
	std::mt19937 random_mt;
};

}
