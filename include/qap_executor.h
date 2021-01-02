#pragma once
#include "permutation.h"
#include "bound.h"
#include "helpers.h"

class base_executor {
public:
	base_executor() = delete;
	base_executor(matrix_t* data, matrix_t* cost, base_bound* lower, base_bound* upper); // TODO: check size equivalence
	base_executor& operator=(const base_executor&) = delete;
	~base_executor();
	virtual solution get_solution() = 0;
protected:
	std::size_t size();
	base_bound* lower_bound;
	base_bound* upper_bound;
	matrix_t& data_volume;
	matrix_t& transfer_cost;
	utils::calculator* my_calculator;
private:
	std::size_t my_size;
};

class sequential_executor : public base_executor {
public:
	sequential_executor(matrix_t* data, matrix_t* cost, base_bound* lower, base_bound* upper, bool concurrency = false, std::size_t approximate_level = 0);
	~sequential_executor();
	solution get_solution();
private:
	bool multithreading_start(const std::size_t& level);
	void multithreading_brute(permutation& current_permutation, permutation& result_permutation, std::size_t& result_criterion, std::size_t& better_upper_bound);
	void recursive_find(permutation& current_permutation, permutation& result_permutation, std::size_t& result_criterion, permutation& bound_permutation, std::size_t level, std::size_t& better_upper_bound);
private:
	std::vector<permutation> threads_data;
	std::vector<std::size_t> threads_criterion;
	std::uint32_t concurrency;
	bool enable_concurrency;
	std::size_t approximate_level;
};
