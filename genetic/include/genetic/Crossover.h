#pragma once
#include "utils.h"
#include <iterator>
#include <map>

namespace genetic {

class BaseCrossover {
public:
	BaseCrossover();
	void init(std::vector<std::vector<std::size_t>>* population, std::size_t start_idx);
	void exec();
	std::vector<std::vector<std::size_t>>* get_descendants();
protected:
	std::size_t base_position;
	std::vector<std::vector<std::size_t>>* population;
	std::vector<std::vector<std::size_t>>* descendants;
	std::size_t start_idx;
	std::size_t length;
	template<typename Vector>
	bool contains(Vector& vector, std::size_t value) {
		return std::find(vector.begin(), vector.end(), value) != vector.end();
	}
	void internal_exec();
	virtual std::vector<std::size_t> internal_generator(std::vector<std::size_t>& first_parent, std::vector<std::size_t>& second_parent, std::size_t first_cut, std::size_t second_cut) = 0;
};

class OrdinalCrossover : public BaseCrossover {
public:
	using BaseCrossover::BaseCrossover;
private:
	std::vector<std::size_t> internal_generator(std::vector<std::size_t>& first_parent, std::vector<std::size_t>& second_parent, std::size_t first_cut, std::size_t second_cut);
};

class PartialCrossover : public BaseCrossover {
public:
	using BaseCrossover::BaseCrossover;
private:
	std::vector<std::size_t> internal_generator(std::vector<std::size_t>& first_parent, std::vector<std::size_t>& second_parent, std::size_t first_cut, std::size_t second_cut);
};

}