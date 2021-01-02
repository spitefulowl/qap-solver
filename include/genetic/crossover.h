#pragma once
#include "utils.h"
#include <iterator>
#include <map>

namespace genetic {

class base_crossover {
public:
	base_crossover();
	void init(std::vector<permutation>& population, std::size_t start_idx);
	std::vector<permutation>& exec();
protected:
	void internal_exec(std::size_t iter_idx);
	virtual void internal_generator(permutation& individ, permutation& first_parent, permutation& second_parent, std::size_t cut) = 0;
	std::vector<permutation>* my_population;
	std::vector<permutation> my_descendants;
	std::size_t my_start_idx;
	std::size_t length;
	utils::randomizer random;
};

class ordinal_crossover : public base_crossover {
public:
	using base_crossover::base_crossover;
private:
	virtual void internal_generator(permutation& individ, permutation& first_parent, permutation& second_parent, std::size_t cut) override;
};

class partial_crossover : public base_crossover {
public:
	using base_crossover::base_crossover;
private:
	virtual void internal_generator(permutation& individ, permutation& first_parent, permutation& second_parent, std::size_t cut) override;
};

}
