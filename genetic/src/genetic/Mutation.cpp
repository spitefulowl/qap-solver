#include "genetic/Mutation.h"

namespace genetic {

BaseMutation::BaseMutation() : descendants(nullptr), perc_probability(0), start_idx(0) {}

void BaseMutation::init(std::vector<std::vector<std::size_t>>* descendants, std::size_t perc_probability, std::size_t start_idx) {
	this->descendants = descendants;
	this->perc_probability = perc_probability;
	this->start_idx = start_idx;
}

void BaseMutation::exec() {
	for (auto& mutant : *descendants) {
		if (utils::random(101, 0) <= perc_probability) {
			internal_mutation(mutant);
		}
	}
}

void PointMutation::internal_mutation(std::vector<std::size_t>& mutant) {
	if (mutant.size() >= start_idx) {
		return;
	}
	std::size_t mutating_gene = utils::random(mutant.size(), start_idx + 1);
	if (mutating_gene == 0) {
		std::swap(mutant[0], mutant[1]);
		return;
	}
	if (mutating_gene == mutant.size() - 1) {
		std::swap(mutant[mutating_gene], mutant[mutating_gene - 1]);
		return;
	}
	if (utils::random(2)) {
		std::swap(mutant[mutating_gene], mutant[mutating_gene + 1]);
	}
	else {
		std::swap(mutant[mutating_gene], mutant[mutating_gene - 1]);
	}
}

void InversionMutation::internal_mutation(std::vector<std::size_t>& mutant) {
	std::size_t first_pos = utils::random(mutant.size() - 1, start_idx);
	std::size_t second_pos = utils::random(mutant.size() + 1, first_pos + 1);
	std::reverse(mutant.begin() + first_pos, mutant.begin() + second_pos);
}

}