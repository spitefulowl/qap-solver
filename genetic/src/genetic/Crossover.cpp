#include "genetic/Crossover.h"

namespace genetic {

BaseCrossover::BaseCrossover() : population(nullptr), descendants(nullptr) {}
BaseCrossover::BaseCrossover(const BaseCrossover& base) {
	descendants = new std::vector<std::vector<std::size_t>>;
	this->population = base.population;
	this->start_idx = base.start_idx;
	this->length = (*base.population)[0].size();
	this->population = base.population;
}

void BaseCrossover::init(std::vector<std::vector<std::size_t>>* population, std::size_t start_idx) {
	this->population = population;
	this->start_idx = start_idx;
	this->length = (*population)[0].size();
	descendants = new std::vector<std::vector<std::size_t>>;
}

std::vector<std::vector<std::size_t>>* BaseCrossover::get_descendants() {
	return descendants;
}

void BaseCrossover::internal_exec() {
	std::size_t first_parent_idx = utils::random(population->size());
	std::size_t second_parent_idx = first_parent_idx;
	while (first_parent_idx == second_parent_idx) {
		second_parent_idx = utils::random(population->size());
	}
	std::vector<std::size_t> first_parent = (*population)[first_parent_idx];
	std::vector<std::size_t> second_parent = (*population)[second_parent_idx];
	std::size_t first_cut = 0;
	std::size_t second_cut = utils::random(length + 1, start_idx);
	auto individ1 = internal_generator(first_parent, second_parent, first_cut, second_cut);
	auto individ2 = internal_generator(second_parent, first_parent, first_cut, second_cut);
	descendants->push_back(individ1);
	descendants->push_back(individ2);
}


void BaseCrossover::exec() {
	std::size_t size = population->size();
	for (std::size_t iter = 0; iter < size; ++iter) {
		internal_exec();
	}
}

std::vector<std::size_t>
OrdinalCrossover::internal_generator(std::vector<std::size_t>& first_parent, std::vector<std::size_t>& second_parent,
		std::size_t first_cut, std::size_t second_cut) {
	std::vector<std::size_t> individ(length);
	std::fill(individ.begin(), individ.end(), -1);
	std::copy(first_parent.begin() + first_cut, first_parent.begin() + second_cut, individ.begin() + first_cut);
	auto individ_iterator = individ.begin() + second_cut;
	auto second_parent_iterator = second_parent.begin() + second_cut == second_parent.end() ? second_parent.begin() : second_parent.begin() + second_cut;
	while (second_parent_iterator != second_parent.end() && contains(individ, -1)) {
		if (individ_iterator != individ.end()) {
			if (!contains(individ, *second_parent_iterator)) {
				(*individ_iterator++) = *second_parent_iterator;
			}
			second_parent_iterator++;
		}
		else {
			individ_iterator = individ.begin();
		}
		if (second_parent_iterator == second_parent.end()) {
			second_parent_iterator = second_parent.begin();
		}
	}
	return individ;
}

std::vector<std::size_t>
PartialCrossover::internal_generator(std::vector<std::size_t>& first_parent, std::vector<std::size_t>& second_parent, std::size_t first_cut, std::size_t second_cut)
{
	std::vector<std::size_t> individ(length);
	std::fill(individ.begin(), individ.end(), -1);
	std::copy(first_parent.begin() + first_cut, first_parent.begin() + second_cut, individ.begin() + first_cut);
	auto individ_iterator = individ.begin() + second_cut;
	auto second_parent_iterator = second_parent.begin() + second_cut == second_parent.end() ? second_parent.begin() : second_parent.begin() + second_cut;
	std::map<std::size_t, std::size_t> my_map;
	for (auto iter = first_parent.begin() + first_cut; iter != first_parent.begin() + second_cut; ++iter) {
		auto dist = iter - first_parent.begin();
		my_map[*iter] = second_parent[dist];
		auto internal_find = [&]() {
			return std::find(first_parent.begin() + first_cut, first_parent.begin() + second_cut, my_map[*iter]);
		};
		while (internal_find() != first_parent.begin() + second_cut) {
			my_map[*iter] = second_parent[internal_find() - first_parent.begin()];
			if (*iter == my_map[*iter]) {
				break;
			}
		}
	}
	while (second_parent_iterator != second_parent.end() && contains(individ, -1)) {
		if (individ_iterator != individ.end()) {
			if (my_map.find(*second_parent_iterator) == my_map.end()) {
				(*individ_iterator++) = *second_parent_iterator;
			}
			else {
				(*individ_iterator++) = my_map[*second_parent_iterator];
			}
			second_parent_iterator++;
		}
		else {
			individ_iterator = individ.begin();
		}
		if (second_parent_iterator == second_parent.end()) {
			second_parent_iterator = second_parent.begin();
		}
	}
	return individ;
}

}