#pragma once
#include "utils.h"
#include "genetic/gen_alg.h"
#include <utility>

class base_bound {
public:
	base_bound() = delete;
	base_bound(utils::matrix_t* data, utils::matrix_t* cost);
	virtual ~base_bound();
	virtual std::size_t get_bound(permutation& base_permutation) = 0;
protected:
	utils::randomizer random;
	utils::calculator* my_calculator;
	std::size_t permutation_size;
	utils::matrix_t& data_volume;
	utils::matrix_t& transfer_cost;
};

// lower_bound section

class degenerate_lower_bound : public base_bound {
public:
	degenerate_lower_bound(utils::matrix_t* data, utils::matrix_t* cost);
	std::size_t get_bound(permutation&) override;
};

class greedy_incorrect_lower_bound : public base_bound {
public:
	greedy_incorrect_lower_bound(utils::matrix_t* data, utils::matrix_t* cost);
	std::size_t get_bound(permutation& base_permutation) override;
private:
	std::vector<std::size_t, allocator> ordered_cost;
};

// upper_bound section

class random_upper_bound : public base_bound {
public:
	random_upper_bound(utils::matrix_t* data, utils::matrix_t* cost);
	std::size_t get_bound(permutation& base_permutation) override;
};

class genetic_upper_bound : public base_bound {
public:
	genetic_upper_bound(utils::matrix_t* data, utils::matrix_t* cost);
	~genetic_upper_bound() override;
	std::size_t get_bound(permutation& base_permutation) override;
private:
	genetic::gen_alg* alg;
};

class optimized_upper_bound : public base_bound {
public:
	optimized_upper_bound(base_bound* first_bound, base_bound* second_bound, std::size_t approximate_level);
	~optimized_upper_bound() override;
	std::size_t get_bound(permutation& base_permutation) override;
private:
	base_bound* first_bound;
	base_bound* second_bound;
	std::size_t approximate_level;
};