#pragma once
#include "permutation.h"
#include "helpers.h"
#include "genetic/GenAlg.h"
#include <utility>

class base_bound {
public:
	base_bound() = delete;
	base_bound(Matrix* data, Matrix* cost);
	~base_bound();
	virtual std::pair<permutation*, double> get_bound(permutation* base_permutation) = 0;
protected:
	randomizer random;
	calculator* calc;
	std::size_t permutation_size;
	Matrix* data_volume; // обмен данными между подзадачами
	Matrix* transfer_cost; // стоимость обмена данными между вычислителями
};

// lower_bound section

class degenerate_lower_bound : public base_bound {
public:
	degenerate_lower_bound(Matrix* data, Matrix* cost);
	std::pair<permutation*, double> get_bound(permutation* base_permutation);
};

class greedy_incorrect_lower_bound : public base_bound {
public:
	greedy_incorrect_lower_bound(Matrix* data, Matrix* cost);
	std::pair<permutation*, double> get_bound(permutation* base_permutation); // TODO: can be simplified
private:
	std::vector<std::size_t> ordered_cost;
};

// upper_bound section

class random_upper_bound : public base_bound {
public:
	random_upper_bound(Matrix* data, Matrix* cost);
	std::pair<permutation*, double> get_bound(permutation* base_permutation);
};

class genetic_upper_bound : public base_bound {
public:
	genetic_upper_bound(Matrix* data, Matrix* cost);
	~genetic_upper_bound();
	std::pair<permutation*, double> get_bound(permutation* base_permutation);
private:
	genetic::GenAlg* alg;
};