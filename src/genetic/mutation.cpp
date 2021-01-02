#include "genetic/mutation.h"

namespace genetic {

base_mutation::base_mutation() : my_descendants(nullptr), my_perc_probability(0), my_start_idx(0) {}

void base_mutation::init(std::vector<permutation>& descendants, std::size_t perc_probability, std::size_t start_idx) {
	my_descendants = &descendants;
	my_perc_probability = perc_probability;
	my_start_idx = start_idx;
}

void base_mutation::exec() {
	for (auto& mutant : *my_descendants) {
		if (random(101) <= my_perc_probability) {
			internal_mutation(mutant);
		}
	}
}

void point_mutation::internal_mutation(permutation& mutant) {
	if (mutant.size() >= my_start_idx) {
		return;
	}
	std::size_t mutating_gene = random(mutant.size(), my_start_idx + 1);
	if (mutating_gene == 0) {
		mutant.swap(0, 1);
		return;
	}
	if (mutating_gene == mutant.size() - 1) {
		mutant.swap(mutating_gene, mutating_gene - 1);
		return;
	}
	if (random(2)) {
		mutant.swap(mutating_gene, mutating_gene + 1);
	}
	else {
		mutant.swap(mutating_gene, mutating_gene - 1);
	}
}

void inversion_mutation::internal_mutation(permutation& mutant) {
	if (my_start_idx == mutant.size()) return;
	std::size_t first_pos = random(mutant.size() - 1, my_start_idx);
	std::size_t second_pos = random(mutant.size() + 1, first_pos + 1);
	mutant.reverse(first_pos, second_pos);
}

}
