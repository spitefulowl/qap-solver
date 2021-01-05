#include "qap.h"
#include "qap_executor.h"
#include "bound.h"
#include <stdexcept>

qap::qap(matrix_t* data_volume, matrix_t* transfer_cost) : data_volume(*data_volume), transfer_cost(*transfer_cost) {}

qap::~qap() {}

solution qap::get_solution(executors exec, lower_bounds lower, upper_bounds upper, bool is_concurrent, std::size_t approximate_level) {
	if (data_volume.rows() == transfer_cost.rows()) {
		base_executor* executor = nullptr;
		base_bound* lower_bound = nullptr;
		base_bound* upper_bound = nullptr;
		switch (lower)
		{
		case degenerate:
			lower_bound = new degenerate_lower_bound(&data_volume, &transfer_cost);
			break;
		case greedy_incorrect:
			lower_bound = new greedy_incorrect_lower_bound(&data_volume, &transfer_cost);
			break;
		}
		switch (upper)
		{
		case random_bound:
			upper_bound = new random_upper_bound(&data_volume, &transfer_cost);
			break;
		case genetic_bound:
			upper_bound = new genetic_upper_bound(&data_volume, &transfer_cost);
			break;
		case optimized_bound:
			base_bound* my_random_bound = new random_upper_bound(&data_volume, &transfer_cost);
			base_bound* my_genetic_bound = new genetic_upper_bound(&data_volume, &transfer_cost);
			upper_bound = new optimized_upper_bound(my_random_bound, my_genetic_bound, approximate_level);
		}
		switch (exec)
		{
		case seq_executor:
			executor = new sequential_executor(&data_volume, &transfer_cost, lower_bound, upper_bound, is_concurrent, approximate_level);
			break;
		#ifdef USE_TBB
		case par_executor:
			executor = new parallel_executor(&data_volume, &transfer_cost, lower_bound, upper_bound);
			break;
		#endif
		}
		solution result = executor->get_solution();
		delete executor;
		delete lower_bound;
		delete upper_bound;
		return result;
	}
	else {
		throw std::runtime_error("size of data_volume not equal to size of transfer_cost");
	}
}
