#include "genetic/selection.h"

namespace genetic {

#define OVERLAP_COEF 0.5

base_selection::base_selection() : my_data_volume(nullptr), my_transfer_cost(nullptr), my_population(nullptr), my_descendants(nullptr), my_calculator(nullptr), my_beta(0) { }

void base_selection::init(utils::matrix_t* data_volume, utils::matrix_t* transfer_cost,
		std::vector<permutation>& population, std::vector<permutation>& descendants, std::size_t beta) {
	if (data_volume != my_data_volume || transfer_cost != my_transfer_cost) {
		this->my_calculator = new utils::calculator(data_volume, transfer_cost);
	}
	this->my_data_volume = data_volume;
	this->my_transfer_cost = transfer_cost;
	this->my_population = &population;
	this->my_descendants = &descendants;
	this->my_beta = beta;
}

void base_selection::exec() {
	std::size_t base_population_count = static_cast<std::size_t>(OVERLAP_COEF * (double)my_population->size());
	std::size_t descendants_count = my_population->size() - base_population_count;
	std::random_shuffle(my_population->begin(), my_population->end());
	internal_selection(descendants_count);
}

void beta_tournament::init(utils::matrix_t* data_volume, utils::matrix_t* transfer_cost,
		std::vector<permutation>& population, std::vector<permutation>& descendants, std::size_t beta) {
	base_selection::init(data_volume, transfer_cost, population, descendants, beta);
	beta_tournament_indexes.resize(beta);
	beta_tournament_criterions.resize(beta);
}

void beta_tournament::internal_selection(std::size_t descendants_count) {
	std::size_t population_idx = my_population->size() - descendants_count;
	for (std::size_t iter = 0; iter < descendants_count; ++iter) {
		for (std::size_t beta_idx = 0; beta_idx < my_beta; ++beta_idx) {
			std::size_t index = random(my_descendants->size());
			beta_tournament_indexes[beta_idx] = index;
			beta_tournament_criterions[beta_idx] = my_calculator->criterion((*my_descendants)[index]);
		}
		std::size_t min_element_idx = std::min_element(beta_tournament_criterions.begin(), beta_tournament_criterions.end()) - beta_tournament_criterions.begin();
		permutation& source_permutation = (*my_descendants)[beta_tournament_indexes[min_element_idx]];
		permutation& target_permutation = (*my_population)[population_idx++];
		source_permutation.copy_to(target_permutation);
	}
}

}
