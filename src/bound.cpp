#include "bound.h"
#include "helpers.h"
#include <numeric>
#include <algorithm>
#include <list>

base_bound::base_bound(matrix_t* data, matrix_t* cost) : data_volume(*data), transfer_cost(*cost) {
	my_calculator = new utils::calculator(data, cost);
	if (data) {
		permutation_size = data->rows();
	}
	else {
		permutation_size = 0;
	}
}

base_bound::~base_bound() {
	delete my_calculator;
}

// lower_bound section

degenerate_lower_bound::degenerate_lower_bound(matrix_t* data, matrix_t* cost) : base_bound(data, cost) {}

std::size_t degenerate_lower_bound::get_bound(permutation&) {
	return static_cast<std::size_t>(1);
}

// upper_bound section

random_upper_bound::random_upper_bound(matrix_t* data, matrix_t* cost) : base_bound(data, cost) { }

std::size_t random_upper_bound::get_bound(permutation& base_permutation) {
	std::vector<std::size_t, allocator> shuffled_values(data_volume.rows());
	if (base_permutation.size() == base_permutation.determined_size()) {
		return my_calculator->criterion(base_permutation);
	}
	const std::set<std::size_t>& unused_values = base_permutation.get_unused();
	std::copy(unused_values.begin(), unused_values.end(), shuffled_values.begin());
	std::random_shuffle(shuffled_values.begin(), shuffled_values.begin() + unused_values.size(), random);
	for (std::size_t& value : shuffled_values) {
		base_permutation.make_used(value);
	}
	return my_calculator->criterion(base_permutation);
}

greedy_incorrect_lower_bound::greedy_incorrect_lower_bound(matrix_t* data, matrix_t* cost) : base_bound(data, cost) {
	for (std::size_t row = 0; row < transfer_cost.rows(); ++row) {
		for (std::size_t column = row + 1; column < transfer_cost.rows(); ++column) {
			if (transfer_cost(row, column)) {
				ordered_cost.push_back(transfer_cost(row, column));
			}
		}
		//std::sort(ordered_cost.begin(), ordered_cost.end(), std::greater<std::size_t>{});
	}
	std::sort(ordered_cost.begin(), ordered_cost.end());
}

std::size_t greedy_incorrect_lower_bound::get_bound(permutation& base_permutation) {
	if (base_permutation.size() == base_permutation.determined_size()) {
		return my_calculator->criterion(base_permutation);
	}
	std::size_t result_criterion = my_calculator->criterion(base_permutation);

	std::vector<std::size_t> unfinished_data;

	for (std::size_t idx = base_permutation.determined_size(); idx < base_permutation.size(); ++idx) {
		for (std::size_t column = idx; column < data_volume.rows(); ++column) {
			if (data_volume(idx, column)) {
				unfinished_data.push_back(data_volume(idx, column));
			}
		}
	}

	for (std::size_t idx = base_permutation.determined_size(); idx < base_permutation.size(); ++idx) {
		for (std::size_t column = 0; column < base_permutation.determined_size(); ++column) {
			if (data_volume(idx, column)) {
				unfinished_data.push_back(data_volume(idx, column));
			}
		}
	}

	std::sort(unfinished_data.begin(), unfinished_data.end(), std::greater<std::size_t>{});

	for (std::size_t idx = 0; idx < unfinished_data.size(); ++idx) {
		result_criterion += unfinished_data[idx] * ordered_cost[idx] * 2;
	}

	return result_criterion - 1;
}

genetic_upper_bound::genetic_upper_bound(matrix_t* data, matrix_t* cost) : base_bound(data, cost) {
	//std::size_t probability = 71;
	//std::size_t beta = 7;
	//std::size_t iterations = 10;
	//std::size_t population = 10;
	std::size_t probability = 81;
	std::size_t beta = 12;
	std::size_t iterations = 15;
	std::size_t population = 20;
	//alg = new genetic::GenAlg(new genetic::RandomGeneration, new genetic::OrdinalCrossover, new genetic::PointMutation, new genetic::BetaTournament, data, cost, iterations, population, probability, beta);
	alg = new genetic::gen_alg(new genetic::random_generation, new genetic::partial_crossover, new genetic::inversion_mutation, new genetic::beta_tournament, data, cost, iterations, population, probability, beta);
}

genetic_upper_bound::~genetic_upper_bound() {
	delete alg;
}

std::size_t genetic_upper_bound::get_bound(permutation& base_permutation) {
	alg->execute(base_permutation);
	return my_calculator->criterion(base_permutation);
}

optimized_upper_bound::optimized_upper_bound(base_bound* first_bound, base_bound* second_bound, std::size_t approximate_level) : base_bound(nullptr, nullptr) {
	this->first_bound = first_bound;
	this->second_bound = second_bound;
	this->approximate_level = approximate_level;
}

optimized_upper_bound::~optimized_upper_bound() {
	delete first_bound;
	delete second_bound;
}

std::size_t optimized_upper_bound::get_bound(permutation& base_permutation) {
	if (base_permutation.determined_size() == approximate_level) {
		return second_bound->get_bound(base_permutation);
	}
	return first_bound->get_bound(base_permutation);
}
