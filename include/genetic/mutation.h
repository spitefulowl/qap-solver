#pragma once
#include "utils.h"

namespace genetic {

class base_mutation {
public:
	base_mutation();
	void init(std::vector<permutation>& descendants, std::size_t perc_probability, std::size_t start_idx);
	void exec();
protected:
	std::vector<permutation>* my_descendants;
	std::size_t my_start_idx;
	virtual void internal_mutation(permutation& mutant) = 0;
	utils::randomizer random;
private:
	std::size_t my_perc_probability;
};

class point_mutation : public base_mutation {
public:
	using base_mutation::base_mutation;
private:
	virtual void internal_mutation(permutation& mutant) override;
};

class inversion_mutation : public base_mutation {
public:
	using base_mutation::base_mutation;
private:
	virtual void internal_mutation(permutation& mutant) override;
};

}
