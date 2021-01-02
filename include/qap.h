#pragma once
#include "qap_executor.h"
#include "bound.h"

#include <vector>
#include <utility>

enum executors { seq_executor };
enum lower_bounds { degenerate, greedy_incorrect };
enum upper_bounds { random_bound, genetic_bound, optimized_bound };

class qap {
public:
	qap(matrix_t* data_volume, matrix_t* transfer_cost);
	~qap();
	solution get_solution(executors exec, lower_bounds lower, upper_bounds upper, bool is_concurrent = false, std::size_t approximate_level = 0);

private:
	matrix_t& data_volume;
	matrix_t& transfer_cost;
};
