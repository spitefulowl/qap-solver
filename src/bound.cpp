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

std::pair<permutation*, std::size_t> degenerate_lower_bound::get_bound(permutation* base_permutation) {
	return std::make_pair(new permutation(*base_permutation), static_cast<std::size_t>(1));
}

// upper_bound section

random_upper_bound::random_upper_bound(Matrix* data, Matrix* cost) : base_bound(data, cost) {}

std::pair<permutation*, std::size_t> random_upper_bound::get_bound(permutation* base_permutation) {
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
		std::sort(ordered_cost.begin(), ordered_cost.end(), std::greater<std::size_t>{});
	}
}

std::pair<permutation*, std::size_t> greedy_incorrect_lower_bound::get_bound(permutation* base_permutation) {
	if (base_permutation->size() == base_permutation->determined_size()) {
		return std::make_pair(new permutation(*base_permutation), calc->criterion(base_permutation));
	}
	std::size_t result_criterion = calc->criterion(base_permutation);

	std::vector<std::size_t> unfinished_data;

	for (std::size_t idx = base_permutation->determined_size(); idx < base_permutation->size(); ++idx) {
		for (std::size_t column = idx; column < data_volume->size(); ++column) {
			if ((*data_volume)[idx][column]) {
				unfinished_data.push_back((*data_volume)[idx][column]);
			}
		}
	}

	for (std::size_t idx = base_permutation->determined_size(); idx < base_permutation->size(); ++idx) {
		for (std::size_t column = 0; column < base_permutation->determined_size(); ++column) {
			if ((*data_volume)[idx][column]) {
				unfinished_data.push_back((*data_volume)[idx][column]);
			}
		}
	}

	std::sort(unfinished_data.begin(), unfinished_data.end());
	std::size_t begin_idx = ordered_cost.size() - unfinished_data.size();

	//std::vector<std::size_t> products;
	//products.reserve(unfinished_data.size());

	for (std::size_t idx = 0; idx < unfinished_data.size(); ++idx) {
		//products.push_back(unfinished_data[idx] * ordered_cost[begin_idx + idx]);
		result_criterion += unfinished_data[idx] * ordered_cost[begin_idx + idx] * 2;
	}
	//std::sort(products.begin(), products.end());
	//com_count /= 8;
	//for (std::size_t idx = 0; idx < com_count; ++idx) {
	//	result_criterion += products[idx] * 2;
	//}
	//for (std::size_t idx = com_count; idx < products.size(); ++idx) {
	//	result_criterion += products[idx];
	//}
	return std::pair<permutation*, std::size_t>(nullptr, result_criterion);
}

genetic_upper_bound::genetic_upper_bound(Matrix* data, Matrix* cost) : base_bound(data, cost) {
	//std::size_t probability = 71;
	//std::size_t beta = 7;
	//std::size_t iterations = 10;
	//std::size_t population = 10;
	std::size_t probability = 81;
	std::size_t beta = 12;
	std::size_t iterations = 15;
	std::size_t population = 20;
	//alg = new genetic::GenAlg(new genetic::RandomGeneration, new genetic::OrdinalCrossover, new genetic::PointMutation, new genetic::BetaTournament, data, cost, iterations, population, probability, beta);
	alg = new genetic::GenAlg(new genetic::RandomGeneration, new genetic::PartialCrossover, new genetic::InversionMutation, new genetic::BetaTournament, data, cost, iterations, population, probability, beta);
}

genetic_upper_bound::~genetic_upper_bound() {
	delete alg;
}

std::pair<permutation*, std::size_t> genetic_upper_bound::get_bound(permutation* base_permutation) {
	std::vector<std::size_t>& extracted = base_permutation->extract();
	std::vector<std::size_t> result = alg->execute(extracted, base_permutation->size());
	permutation* result_permutation = new permutation(result);
	return std::make_pair(result_permutation, calc->criterion(result_permutation));
}