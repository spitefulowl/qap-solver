#pragma once
#include "utils.h"
#include "bound.h"

#ifdef USE_TBB
	#include <tbb/global_control.h>
	#include <tbb/task_group.h>
	#include <tbb/task_arena.h>
#endif

#define TREE_HEIGHT 2

class base_executor {
public:
	base_executor() = delete;
	base_executor(utils::matrix_t* data, utils::matrix_t* cost, base_bound* lower, base_bound* upper); // TODO: check size equivalence
	base_executor& operator=(const base_executor&) = delete;
	virtual ~base_executor() noexcept(false);
	virtual solution get_solution() = 0;
protected:
	std::size_t size() {
		return my_size;
	}
	base_bound* lower_bound;
	base_bound* upper_bound;
	utils::matrix_t& data_volume;
	utils::matrix_t& transfer_cost;
	utils::calculator* my_calculator;
private:
	std::size_t my_size;
};

class sequential_executor : public base_executor {
public:
	sequential_executor(utils::matrix_t* data, utils::matrix_t* cost, base_bound* lower, base_bound* upper, bool concurrency = false, std::size_t approximate_level = 0);
	~sequential_executor() override = default;
	solution get_solution() override;
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

#ifdef USE_TBB

#include <tbb/concurrent_vector.h>
#include <atomic>

class parallel_executor : public base_executor {
public:
	parallel_executor(utils::matrix_t* data, utils::matrix_t* cost, base_bound* lower, base_bound* upper, std::size_t approximate_level = 0, std::size_t task_tree_height = TREE_HEIGHT);
	~parallel_executor() override;
	solution get_solution() override;
private:
	void generate_tasks(permutation& current_permutation, permutation& bound_permutation, std::size_t level);

	class parallel_task {
	public:
		parallel_task(parallel_executor* executor, permutation& start_permutation);
		parallel_task(const parallel_task&) = delete;
		parallel_task(parallel_task&&);
		~parallel_task();
		void operator()() const;
	private:
		void recursive_find(permutation& result_permutation, std::size_t& result_criterion, permutation& bound_permutation, std::size_t level) const;
		std::size_t my_start_level;
		permutation* my_permutation;
		parallel_executor* executor;
	};

	tbb::task_group task_group;
	tbb::global_control* global_control;

	std::size_t task_tree_height;
	std::size_t approximate_level;
	std::atomic<std::size_t> better_upper_bound;
	tbb::concurrent_vector<solution> tasks_solutions;
};
#endif
