#include "qap_executor.h"
#include <thread>

#define BRUTE_START_LEVEL 8

base_executor::base_executor(matrix_t* data, matrix_t* cost, base_bound* lower, base_bound* upper) :
		data_volume(*data), transfer_cost(*cost),
		lower_bound(lower), upper_bound(upper) {
	my_calculator = new utils::calculator(data, cost);
	my_size = data->rows();
}

base_executor::~base_executor() {
	if (my_calculator) {
		delete my_calculator;
	}
}

std::size_t base_executor::size() {
	return my_size;
}

sequential_executor::sequential_executor(matrix_t* data, matrix_t* cost, base_bound* lower,
		base_bound* upper, bool concurrency, std::size_t approximate_level) : base_executor(data, cost, lower, upper) {

	if (concurrency && approximate_level) {
		throw std::runtime_error("incompatible parameters");
	}
	this->concurrency = std::thread::hardware_concurrency();
	threads_data.reserve(concurrency);
	for (std::size_t idx = 0; idx < this->concurrency; ++idx) {
		threads_data.emplace_back(permutation(this->size()));
		threads_criterion.push_back(UINT32_MAX);
	}
	enable_concurrency = concurrency;
	this->approximate_level = approximate_level;
}

sequential_executor::~sequential_executor() {}

bool sequential_executor::multithreading_start(const std::size_t& level) {
	return (this->size() - level == BRUTE_START_LEVEL);
}

void sequential_executor::multithreading_brute(permutation& current_permutation, permutation& result_permutation, std::size_t& result_criterion, std::size_t& better_upper_bound) {
	// prepare base permutations for each thread
	std::set<std::size_t> unused_indexes = current_permutation.get_unused();
	std::size_t current_size = current_permutation.determined_size();
	std::size_t idx = 0;
	for (auto unused_indexes_iterator = unused_indexes.begin(); unused_indexes_iterator != unused_indexes.end(); ++unused_indexes_iterator) {
		std::size_t unused_index = *unused_indexes_iterator;
		current_permutation.copy_to(threads_data[idx]);
		threads_data[idx].set(current_size, unused_index);
		++idx;
	}

	utils::calculator* calculator = my_calculator;
	// task implementation
	auto brute_task = [calculator](permutation& base_permutation, std::size_t& base_criterion) {
		std::set<std::size_t> unused_indexes = base_permutation.get_unused();
		permutation better_permutation(base_permutation.size());
		std::size_t current_criterion = 0;
		std::size_t base_size = base_permutation.determined_size();
		std::size_t size = base_permutation.size();
		std::size_t idx = 0;
		for (auto unused_indexes_iterator = unused_indexes.begin(); unused_indexes_iterator != unused_indexes.end(); ++unused_indexes_iterator) {
			std::size_t unused_index = *unused_indexes_iterator;
			base_permutation.set(base_size + idx, unused_index);
			++idx;
		}
		do {
			current_criterion = calculator->criterion(base_permutation);
			if (current_criterion < base_criterion) {
				base_criterion = current_criterion;
				base_permutation.copy_to(better_permutation);
			}
		} while(base_permutation.next_permutation(base_size, size));
		better_permutation.copy_to(base_permutation);
	};

	// run tasks
	std::vector<std::thread> thread_pool;
	for (std::size_t thread_idx = 0; thread_idx < threads_data.size(); ++thread_idx) {
		thread_pool.emplace_back(brute_task, std::ref(threads_data[thread_idx]), std::ref(threads_criterion[thread_idx]));
	}
	for (std::size_t thread_idx = 0; thread_idx < thread_pool.size(); ++thread_idx) {
		thread_pool[thread_idx].join();
	}

	// analyze results
	auto better_result = std::min_element(threads_criterion.begin(), threads_criterion.end());
	std::size_t better_result_idx = better_result - threads_criterion.begin();
	std::size_t better_result_criterion = *better_result;
	if (better_result_criterion < result_criterion) {
		result_criterion = better_result_criterion;
		threads_data[better_result_idx].copy_to(result_permutation);
		if (better_result_criterion < better_upper_bound) {
			better_upper_bound = better_result_criterion;
		}
	}
}

#include <stdio.h>

void sequential_executor::recursive_find(permutation& current_permutation, permutation& result_permutation, std::size_t& result_criterion, permutation& bound_permutation, std::size_t level, std::size_t& better_upper_bound) {
	std::set<std::size_t> unused_indexes = current_permutation.get_unused();
	for (auto unused_indexes_iterator = unused_indexes.begin(); unused_indexes_iterator != unused_indexes.end(); ++unused_indexes_iterator) {
		if (level == 0) printf("Return to level 0\n");

		if (approximate_level > 0 && level == approximate_level) {
			current_permutation.copy_to(bound_permutation);
			std::size_t current_upper_bound = upper_bound->get_bound(bound_permutation);
			if (result_criterion > current_upper_bound) {
				result_criterion = current_upper_bound;
				better_upper_bound = current_upper_bound;
				bound_permutation.copy_to(result_permutation);
			}
			return;
		}

		if (enable_concurrency && multithreading_start(level)) {
			multithreading_brute(current_permutation, result_permutation, result_criterion, better_upper_bound);
			return;
		}
		if (level == current_permutation.size() - 1) {
			std::size_t unused_index = *unused_indexes_iterator;
			current_permutation.set(level, unused_index);
			std::size_t current_criterion = my_calculator->criterion(current_permutation);
			if (result_criterion > current_criterion) {
				result_criterion = current_criterion;
				current_permutation.copy_to(result_permutation);
			}
			current_permutation.make_last_unused();
			return;
		}
		std::size_t unused_index = *unused_indexes_iterator;
		current_permutation.set(level, unused_index);
		current_permutation.copy_to(bound_permutation);
		std::size_t current_upper_bound = upper_bound->get_bound(bound_permutation);
		std::size_t current_lower_bound = lower_bound->get_bound(current_permutation);
		if (current_lower_bound >= better_upper_bound) {
			current_permutation.make_last_unused();
			continue;
		}
		if (current_upper_bound < better_upper_bound) {
			better_upper_bound = current_upper_bound;
			if (approximate_level) {
				if (current_upper_bound < result_criterion) {
					bound_permutation.copy_to(result_permutation);
					result_criterion = current_upper_bound;
				}
			}
		}
		recursive_find(current_permutation, result_permutation, result_criterion, bound_permutation, level + 1, better_upper_bound);
		current_permutation.make_last_unused();
	}
}

solution sequential_executor::get_solution() {
	permutation current_permutation(size());
	permutation result_permutation(size());
	permutation bound_permutation(size());
	std::size_t better_upper_bound = upper_bound->get_bound(bound_permutation);
	bound_permutation.copy_to(result_permutation);
	std::size_t result_criterion = better_upper_bound;
	std::size_t level = 0;
	recursive_find(current_permutation, result_permutation, result_criterion, bound_permutation, level, better_upper_bound);

	return std::make_pair(result_permutation, result_criterion);
}
