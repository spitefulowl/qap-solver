#include "genetic/crossover.h"

namespace genetic {

base_crossover::base_crossover() : my_population(nullptr), my_start_idx(0) {}

void base_crossover::init(std::vector<permutation>& population, std::size_t start_idx) {
	static bool init_flag = true;
	my_population = &population;
	my_start_idx = start_idx;
	length = population[0].size();
	if (init_flag) {
		for (std::size_t idx = 0; idx < population.size() * 2; ++idx) {
			my_descendants.emplace_back(permutation(length));
		}
		init_flag = false;
	}
}

void base_crossover::internal_exec(std::size_t iter_idx) {
	std::size_t first_parent_idx = random(my_population->size());
	std::size_t second_parent_idx = first_parent_idx;
	while (first_parent_idx == second_parent_idx) {
		second_parent_idx = random(my_population->size());
	}
	permutation& first_parent = (*my_population)[first_parent_idx];
	permutation& second_parent = (*my_population)[second_parent_idx];
	std::size_t cut = random(length, my_start_idx);
	internal_generator(my_descendants[iter_idx], first_parent, second_parent, cut);
	internal_generator(my_descendants[iter_idx + 1], second_parent, first_parent, cut);
}

std::vector<permutation>& base_crossover::exec() {
	for (std::size_t iter_idx = 0; iter_idx < my_descendants.size(); iter_idx += 2) {
		internal_exec(iter_idx);
	}
	return my_descendants;
}

void ordinal_crossover::internal_generator(permutation& individ,
	permutation& first_parent, permutation& second_parent, std::size_t cut) {

	for (std::size_t idx = 0; idx < cut; ++idx) {
		individ.set(idx, first_parent.get(idx));
	}
	std::size_t second_parent_idx = cut;
	std::set<std::size_t>& unused_indexes = individ.get_unused();
	std::size_t second_parent_value = 0;
	for (std::size_t idx = cut; idx < individ.size(); ++idx) {
		second_parent_value = second_parent.get(second_parent_idx);
		while (unused_indexes.find(second_parent_value) == unused_indexes.end()) {
			second_parent_idx = second_parent_idx + 1 >= second_parent.determined_size() ? 0 : second_parent_idx + 1;
			second_parent_value = second_parent.get(second_parent_idx);
		}
		individ.set(idx, second_parent_value);
		++second_parent_idx;
	}
}

void partial_crossover::internal_generator(permutation& individ,
		permutation& first_parent, permutation& second_parent, std::size_t cut) {
	std::map<std::size_t, std::size_t> mapping;
	for (std::size_t idx = 0; idx < cut; ++idx) {
		std::size_t first_parent_value = first_parent.get(idx);
		individ.set(idx, first_parent_value);
		mapping[first_parent_value] = second_parent.get(idx);
	}
	for (std::size_t idx = cut; idx < individ.size(); ++idx) {
		std::size_t value = second_parent.get(idx);
		while (mapping.find(value) != mapping.end()) {
			value = mapping[value];
		}
		individ.set(idx, value);
	}

}

}
