#include "bound.h"
#include "helpers.h"
#include <numeric>
#include <algorithm>
#include <list>

base_bound::base_bound(Matrix* data, Matrix* cost) : data_volume(data), transfer_cost(cost) {
	calc = new calculator(data, cost);
	permutation_size = data->size();
}

base_bound::~base_bound() {
	delete calc;
}

// lower_bound section

degenerate_lower_bound::degenerate_lower_bound(Matrix* data, Matrix* cost) : base_bound(data, cost) {}

std::pair<permutation*, double> degenerate_lower_bound::get_bound(permutation* base_permutation) {
	return std::make_pair(new permutation(*base_permutation), static_cast<double>(1));
}

// upper_bound section

random_upper_bound::random_upper_bound(Matrix* data, Matrix* cost) : base_bound(data, cost) {}

std::pair<permutation*, double> random_upper_bound::get_bound(permutation* base_permutation) {
	if (base_permutation->size() == base_permutation->determined_size()) {
		return std::make_pair(new permutation(*base_permutation), calc->criterion(base_permutation));
	}
	std::vector<std::size_t> result = base_permutation->my_permutation;
	std::list<std::size_t> items = base_permutation->get_unused();
	std::size_t old_size = result.size();
	std::copy(items.begin(), items.end(), std::back_inserter(result));
	std::random_shuffle(result.begin() + old_size, result.end(), random);
	permutation* result_permutation = new permutation(result);
	return std::make_pair(result_permutation, calc->criterion(result_permutation));
}

greedy_incorrect_lower_bound::greedy_incorrect_lower_bound(Matrix* data, Matrix* cost) : base_bound(data, cost) {
	printf("greedy selected\n");
	for (std::size_t row = 0; row < cost->size(); ++row) {
		for (std::size_t column = 0; column < cost->size(); ++column) {
			if ((*cost)[row][column]) {
				ordered_cost.push_back((*cost)[row][column]);
			}
		}
		std::sort(ordered_cost.begin(), ordered_cost.end());
	}
}

std::pair<permutation*, double> greedy_incorrect_lower_bound::get_bound(permutation* base_permutation) { // TODO: can be simplified
	if (base_permutation->size() == base_permutation->determined_size()) {
		return std::make_pair(new permutation(*base_permutation), calc->criterion(base_permutation));
	}
	double result_criterion = calc->criterion(base_permutation);

	std::vector<std::size_t> unfinished_data;
	std::vector<std::size_t> unfinished_cost;

	for (std::size_t idx = base_permutation->determined_size(); idx < base_permutation->size(); ++idx) {
		for (std::size_t column = 0; column < data_volume->size(); ++column) {
			if ((*data_volume)[idx][column]) {
				unfinished_data.push_back((*data_volume)[idx][column]);
			}
		}
	}
	std::sort(unfinished_data.begin(), unfinished_data.end());

	std::copy(ordered_cost.begin(), ordered_cost.begin() + unfinished_data.size(), std::back_inserter(unfinished_cost));
	std::reverse(unfinished_cost.begin(), unfinished_cost.end());

	for (std::size_t idx = 0; idx < unfinished_data.size(); ++idx) {
		result_criterion += unfinished_data[idx] * unfinished_cost[idx];
	}
	return std::pair<permutation*, double>(nullptr, result_criterion);
}

genetic_upper_bound::genetic_upper_bound(Matrix* data, Matrix* cost) : base_bound(data, cost) {
	std::size_t probability = 71;
	std::size_t beta = 7;
	std::size_t iterations = 10;
	std::size_t population = 10;
	alg = new genetic::GenAlg(new genetic::RandomGeneration, new genetic::OrdinalCrossover, new genetic::PointMutation, new genetic::BetaTournament, data, cost, iterations, population, probability, beta);
}

genetic_upper_bound::~genetic_upper_bound() {
	delete alg;
}

std::pair<permutation*, double> genetic_upper_bound::get_bound(permutation* base_permutation) {
	std::vector<std::size_t>& extracted = base_permutation->extract();
	std::vector<std::size_t> result = alg->execute(extracted, base_permutation->size());
	permutation* result_permutation = new permutation(result);
	return std::make_pair(result_permutation, calc->criterion(result_permutation));
}