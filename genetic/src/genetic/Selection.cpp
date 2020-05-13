#include "genetic/Selection.h"

namespace genetic {

#define OVERLAP_COEF 0.5

BaseSelection::BaseSelection() : population(nullptr), descendants(nullptr), data_volume(nullptr), transfer_cost(nullptr) {}

void BaseSelection::init(utils::Matrix* data_volume, utils::Matrix* transfer_cost,
	utils::Matrix* population, utils::Matrix* descendants, std::size_t beta) {
	this->data_volume = data_volume;
	this->transfer_cost = transfer_cost;
	this->population = population;
	this->descendants = descendants;
	this->beta = beta;
	this->calc = utils::calculator(data_volume, transfer_cost);
}

utils::Matrix* BaseSelection::exec() {
	utils::Matrix* new_population = new utils::Matrix;
	std::size_t base_population_count = static_cast<std::size_t>(OVERLAP_COEF * (double)population->size());
	std::size_t descendants_count = population->size() - base_population_count;
	std::random_shuffle(population->begin(), population->end());
	std::copy(population->begin(), population->begin() + base_population_count, std::back_inserter(*new_population));
	auto* new_descendants = internal_selection(descendants_count);
	std::copy(new_descendants->begin(), new_descendants->end(), std::back_inserter(*new_population));
	delete new_descendants;
	return new_population;
}

utils::Matrix* BetaTournament::internal_selection(std::size_t size) {
	utils::Matrix* result = new utils::Matrix;
	for (std::size_t iter = 0; iter < size; ++iter) {
		std::vector<std::size_t> criterion;
		utils::Matrix* beta_tour = new utils::Matrix;
		for (std::size_t beta_idx = 0; beta_idx < beta; ++beta_idx) {
			beta_tour->push_back((*descendants)[utils::random(descendants->size())]);
		}
		for (auto iter = beta_tour->begin(); iter != beta_tour->end(); ++iter) {
			criterion.push_back(calc.criterion(&(*iter)));
		}
		result->push_back((*beta_tour)[std::min_element(criterion.begin(), criterion.end()) - criterion.begin()]);
		delete beta_tour;
	}
	return result;
}
// Broken

//utils::Matrix* ProportionalSelection::internal_selection(std::size_t size) {
//	utils::Matrix* result = new utils::Matrix;
//	std::vector<std::size_t> criterion;
//	for (auto iter = descendants->begin(); iter != descendants->end(); ++iter) {
//		iter->push_back((*iter)[0]);
//		criterion.push_back(calc.criterion(&(*iter)));
//		iter->pop_back();
//	}
//	//double criterion_avg = std::accumulate(criterion.begin(), criterion.end(), 0.) / (double)(criterion.end() - criterion.begin());
//	//for (std::size_t idx = 0; idx < descendants->size(); ++idx) {
//	//	for (std::size_t counter = 0; counter < criterion_avg / criterion[idx]; ++counter) {
//	//		if (result->size() < size) {
//	//			result->push_back((*descendants)[idx]);
//	//		}
//	//	}
//	//}
//
//
//	//custom
//	//double criterion_sum = std::accumulate(criterion.begin(), criterion.end(), 0.);
//	//std::vector<double> partial_sum;
//	//std::partial_sum(criterion.begin(), criterion.end(), std::back_inserter(partial_sum));
//	//while (result->size() < size) {
//	//	std::size_t selected_value = utils::random(*partial_sum.rbegin() + 1.);
//	//	std::size_t selected_idx = std::find_if(partial_sum.rbegin(), partial_sum.rend(), [&selected_value](double& value) {
//	//		return value <= selected_value;
//	//	}) - partial_sum.rbegin();
//	//	result->push_back((*descendants)[partial_sum.size() - selected_idx]);
//	//}
//	//return result;
//
//	double criterion_sum = std::accumulate(criterion.begin(), criterion.end(), 0.);
//	double max = *std::max_element(criterion.begin(), criterion.end());
//	double upper = max / criterion_sum;
//	double internal_probability = upper;
//	std::size_t multiplier = 1;
//	while (internal_probability < 10) {
//		internal_probability *= 10.;
//		multiplier *= 10;
//	}
//	while (result->size() < size) {
//		for (std::size_t idx = 0; idx < descendants->size(); ++idx) {
//			double probability = (upper - criterion[idx] / criterion_sum) * multiplier;
//			if (((double)utils::random(upper * multiplier)) <= probability && result->size() < size) {
//				result->push_back((*descendants)[idx]);
//			}
//		}
//	}
//
//	return result;
//}

}