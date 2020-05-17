#include "qap_executor.h"
#include <thread>
#include <mutex>

std::mutex m;
std::vector<permutation> shared;

using node_t = node<permutation>;

base_executor::base_executor(Matrix* data, Matrix* cost, base_bound* lower, base_bound* upper) :
		data_volume(data), transfer_cost(cost), 
		lower_bound(lower), upper_bound(upper), better_permutation(nullptr), global_solution(false), minimal_upper_bound(-1.) {
	calc = new calculator(data, cost); 
	my_size = data->size();
	my_graph = new graph<permutation>();
}

base_executor::~base_executor() {
	if (calc) {
		delete calc;
		delete my_graph;
	}
}

std::size_t base_executor::size() {
	return my_size;
}

std::vector<permutation>* base_executor::next_permutations(permutation* base)
{
	std::list<std::size_t> unused = base->get_unused();
	std::vector<permutation>* result = new std::vector<permutation>;
	for (std::size_t& elem : unused) {
		result->push_back(permutation(*base, elem));
	}
	return result;
}

std::vector<node_t*> base_executor::next_nodes(std::vector<permutation>* permutations) {
	std::vector<node_t*> result;
	for (permutation& elem : (*permutations)) {
		result.push_back(my_graph->make_node(elem));
	}
	return result;
}

sequential_executor::sequential_executor(Matrix* data, Matrix* cost, base_bound* lower, base_bound* upper, bool concurrency, bool is_approximate) : base_executor(data, cost, lower, upper) {
	this->concurrency = std::thread::hardware_concurrency();
	enable_concurrency = concurrency;
	this->is_approximate = is_approximate;
}

sequential_executor::~sequential_executor() {}

void sequential_executor::refresh_minimal_upper_bound() {
	for (std::size_t idx = 0; idx < my_graph->count_of_nodes(); ++idx) {
		if (!my_graph->try_select_next(idx)) {
			continue;
		}
		auto current_upper = upper_bound->get_bound(&my_graph->get_current_node()->get_value());
		if (current_upper.second < minimal_upper_bound) {
			if (current_upper.second < 30000) printf("Upper bound: %lu\n", current_upper.second);
			minimal_upper_bound = current_upper.second;
			delete better_permutation;
			better_permutation = current_upper.first;
		}
		else {
			delete current_upper.first;
		}
		my_graph->back();
	}
}

void sequential_executor::find_unpromising() {
	node_t* my_position = my_graph->get_current_node();
	permutation my_position_value = my_graph->get_current_node()->get_value();
	std::size_t my_position_value_size = my_position_value.determined_size();
	do {
		for (std::size_t idx = 0; idx < my_graph->count_of_nodes(); ++idx) {
			if (!my_graph->try_select_next(idx)) {
				continue;
			}
			auto lower_b = lower_bound->get_bound(&my_graph->get_current_node()->get_value());
			if (lower_b.second >= minimal_upper_bound) {
				bool equal = true;
				auto& value = my_graph->get_current_node()->get_value();
				std::size_t value_size = value.determined_size();
				std::size_t distance = std::min(value_size, my_position_value_size);
				for (std::size_t elem_idx = 0; elem_idx < distance; ++elem_idx) {
					if (value[elem_idx] != my_position_value[elem_idx]) {
						equal = false;
						break;
					}
				}
				if (!equal) {
					my_graph->back();
					my_graph->destroy_by_index(idx);
					if (lower_b.first) {
						delete lower_b.first;
					}
					continue;
				}
			}
			if (lower_b.first) {
				delete lower_b.first;
			}
			my_graph->back();
		}
	} while (my_graph->back());
	my_graph->set_current_node(my_position);
}

void sequential_executor::multithreading_brute() {
	shared.clear();
	permutation base_brute_value = my_graph->get_current_node()->get_value();
	std::vector<permutation>* next_p = next_permutations(&base_brute_value);
	std::vector<std::thread> thread_pool;
	calculator* tmp_calc = this->calc;
	auto base_task = [tmp_calc](permutation& my_permutation) {
		std::vector<std::size_t> per;
		per.reserve(my_permutation.size());
		for (std::size_t idx = 0; idx < my_permutation.determined_size(); ++idx) {
			per.push_back(my_permutation[idx]);
		}
		std::list<std::size_t> unused = my_permutation.get_unused();
		std::size_t per_size = per.size();
		std::copy(unused.begin(), unused.end(), std::back_inserter(per));
		std::sort(per.begin() + per_size, per.end());
		permutation per_c = permutation(per);
		std::size_t my_crit = tmp_calc->criterion(&per_c);
		permutation internal_solution = per;
		do {
			per_c = permutation(per);
			std::size_t tmp_crit = tmp_calc->criterion(&per_c);
			if (tmp_crit < my_crit) {
				my_crit = tmp_crit;
				internal_solution = per;
			}
		} while (std::next_permutation(per.begin() + per_size, per.end()));
		m.lock();
		shared.push_back(permutation(internal_solution));
		m.unlock();
	};
	for (auto iter = next_p->begin(); iter != next_p->end(); ++iter) {
		thread_pool.emplace_back([&base_task, iter]() {
			base_task(*iter);
		});
	}
	for (std::size_t idx = 0; idx < thread_pool.size(); ++idx) {
		thread_pool[idx].join();
	}
	auto internal_better_it = std::min_element(shared.begin(), shared.end(), [tmp_calc](permutation& first, permutation& second) {
		std::size_t first_estim = tmp_calc->criterion(&first);
		std::size_t second_estim = tmp_calc->criterion(&second);
		return first_estim < second_estim;
	});
	std::size_t current_better = calc->criterion(better_permutation);
	std::size_t internal_better_estim = tmp_calc->criterion(&(*internal_better_it));
	if (current_better > internal_better_estim) {
		delete better_permutation;
		better_permutation = new permutation(*internal_better_it);
		minimal_upper_bound = internal_better_estim;
	}
	my_graph->back();
	delete next_p;
}

void sequential_executor::recursive_find()
{
	if (my_graph->get_current_node()->valid_node_value()) {
		permutation current = my_graph->get_current_node()->get_value();
		auto lower = lower_bound->get_bound(&current);
		auto upper = upper_bound->get_bound(&current);
		std::size_t crit = calc->criterion(&current);
		if (my_graph->count_of_nodes()) {
			if (lower.second == upper.second && my_graph->get_current_node()->get_level() < size() - 3) {
				printf("Find global on level = %u\n", my_graph->get_current_node()->get_level());
				global_solution = true;
				delete better_permutation;
				better_permutation = upper.first;
				if (lower.first) {
					delete lower.first;
				}
				return;
			}
		}
		else {
			std::size_t current_better = calc->criterion(better_permutation);
			if (current_better > upper.second) {
				delete better_permutation;
				better_permutation = new permutation(current);
			}
			if (lower.first) {
				delete lower.first;
				lower.first = nullptr;
			}
		}
		if (lower.first) {
			delete lower.first;
		}
		delete upper.first;
		refresh_minimal_upper_bound();
		find_unpromising();
	}

	std::size_t levels_to_the_end = data_volume->size() - my_graph->get_current_node()->get_level();

	if (enable_concurrency && levels_to_the_end < 9) { // < 9
		multithreading_brute();
		return;
	}

	if (is_approximate && my_graph->get_current_node()->get_level() >= 6/*size() / 2*/) {
		my_graph->back();
		return;
	}


	for (std::size_t idx = 0; idx < my_graph->count_of_nodes(); ++idx) {
		if (global_solution) {
			return;
		}
		if (!my_graph->try_select_next(idx)) {
			continue;
		}
		if (my_graph->get_current_node()->get_level() == 1) {
			printf("On level 1\n");
		}
		if (my_graph->get_current_node()->get_level() == 2) {
			printf("On level 2\n");
		}
		auto next_p = this->next_permutations(&my_graph->get_current_node()->get_value());
		auto next_n = this->next_nodes(next_p);
		my_graph->set_next_nodes(next_n);
		delete next_p;
		recursive_find();
		if (global_solution) { // crutch
			return;
		}
		my_graph->destroy_by_index(idx);
	}
	my_graph->back();
}

solution sequential_executor::get_solution() {
	permutation* start_permutation = new permutation(this->size());
	std::vector<permutation>* next_p = next_permutations(start_permutation);
	std::vector<node_t*> next_nodes = this->next_nodes(next_p);
	my_graph->set_next_nodes(next_nodes);
	auto test_upper_bound = upper_bound->get_bound(&(*(next_nodes.begin()))->get_value());
	minimal_upper_bound = test_upper_bound.second;
	better_permutation = test_upper_bound.first;
	delete next_p;
	delete start_permutation;
	next_p = nullptr;

	recursive_find();

	auto result = std::make_pair(*better_permutation, calc->criterion(better_permutation));
	delete better_permutation;

	return result;

	//return {*new permutation(this->size()), static_cast<std::size_t>(0)};
}
