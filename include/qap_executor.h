#pragma once
#include "permutation.h"
#include "bound.h"
#include "helpers.h"
#include "graph.h"

class base_executor {
public:
	using node_t = node<permutation>;
	base_executor() = delete;
	base_executor(Matrix* data, Matrix* cost, base_bound* lower, base_bound* upper); // TODO: check size equivalence 
	base_executor& operator=(const base_executor&) = delete;
	~base_executor();
	virtual solution get_solution() = 0;
protected:
	std::vector<permutation>* next_permutations(permutation* base);
	std::vector<node_t*> next_nodes(std::vector<permutation>* permutations);
	std::size_t size();
	base_bound* lower_bound;
	base_bound* upper_bound;
	Matrix* data_volume; // обмен данными между подзадачами
	Matrix* transfer_cost; // стоимость обмена данными между вычислителями
	graph<permutation>* my_graph;
	permutation* better_permutation;
	calculator* calc;
	bool global_solution;
	double minimal_upper_bound;
private:
	std::size_t my_size;
};

class sequential_executor : public base_executor {
public:
	sequential_executor(Matrix* data, Matrix* cost, base_bound* lower, base_bound* upper, bool concurrency, bool is_approximate = false);
	~sequential_executor();
	solution get_solution();
private:
	void refresh_minimal_upper_bound();
	void find_unpromising();
	void multithreading_brute();
	void recursive_find();
private:
	std::uint32_t concurrency;
	bool enable_concurrency;
	bool is_approximate;
};
