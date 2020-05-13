#include "permutation.h"
#include <numeric>
#include <algorithm>
#include <stdexcept>

permutation::permutation(std::size_t size) : my_size(size) {}

permutation::permutation(const permutation& copy)
{
	my_permutation = copy.my_permutation; // TODO: check correct of this
	my_size = copy.size();
}

permutation::permutation(const std::vector<std::size_t>& vector, std::size_t size) {
	my_permutation = vector;
	my_size = size ? size : vector.size();
}

permutation::permutation(const permutation& current, std::size_t value) {
	my_size = current.my_size;
	my_permutation = current.my_permutation;
	my_permutation.push_back(value);
	if (current.determined_size() == current.my_size) {
		fprintf(stderr, "This permutation is already finally built, no effect\n");
	}
}

std::size_t& permutation::operator[](std::size_t idx) {
	if (idx < this->determined_size()) {
		return my_permutation[idx];
	}
	else {
		throw std::runtime_error("Out of index");
	}
}

std::size_t permutation::determined_size() const {
	return my_permutation.size();
}

std::size_t permutation::size() const {
	return my_size;
}

std::list<std::size_t> permutation::get_unused() {
	std::list<std::size_t> result(size());
	std::iota(result.begin(), result.end(), 0);
	for (auto& elem : my_permutation) {
		auto iter = std::find(result.begin(), result.end(), elem);
		if (iter != result.end()) {
			result.erase(iter);
		}
	}
	return result;
}

std::vector<std::size_t>& permutation::extract() {
	return my_permutation;
}
