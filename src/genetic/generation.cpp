#include "genetic/generation.h"

namespace genetic {

base_generation::base_generation() : my_base_permutation(nullptr), my_population_size(0) {}

void base_generation::init(permutation& base_permutation, std::size_t population_size) {
	my_base_permutation = &base_permutation;
	if (my_population_size != population_size) {
		my_population_size = population_size;
		my_population.reserve(population_size);
		std::size_t individ_size = my_base_permutation->size();
		for (std::size_t idx = 0; idx < population_size; ++idx) {
			this->my_population.emplace_back(permutation(individ_size));
		}
	}
}

void random_generation::init(permutation& base_permutation, std::size_t population_size) {
	std::size_t individ_size = base_permutation.size();
	if (my_base_permutation != nullptr) {
		std::size_t my_individ_size = my_base_permutation->size();
		if (my_individ_size != individ_size) {
			shuffled_values.reserve(individ_size);
		}
	}
	else {
		shuffled_values.reserve(individ_size);
	}
	base_generation::init(base_permutation, population_size);
}

std::vector<permutation>& random_generation::exec() {
	auto unused_values = my_base_permutation->get_set_unused();
	shuffled_values.resize(my_base_permutation->size() - my_base_permutation->determined_size());
	std::copy(unused_values.begin(), unused_values.end(), shuffled_values.begin());
	for (std::size_t idx = 0; idx < my_population_size; ++idx) {
		std::random_shuffle(shuffled_values.begin(), shuffled_values.begin() + unused_values.size(), random);
		my_base_permutation->copy_to(my_population[idx]);
		for (std::size_t& value : shuffled_values) {
			my_population[idx].make_used(value);
		}
	}
	return my_population;
}

}
