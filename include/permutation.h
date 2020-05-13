#pragma once
#include <vector>
#include <list>

using Matrix = std::vector<std::vector<std::size_t>>;

class permutation {
public:
	permutation(std::size_t size);
	permutation(const permutation& copy);
	permutation(const permutation& current, std::size_t value);
	permutation(const std::vector<std::size_t>& vector, std::size_t size = 0);

	std::size_t& operator[](std::size_t idx);
	std::vector<std::size_t>& extract();

	std::size_t determined_size() const;
	std::size_t size() const;
	std::list<std::size_t> get_unused();

private:
	friend class random_upper_bound;
	std::vector<std::size_t> my_permutation; // номера задач, индексы - номера вычислителей
	std::size_t my_size;
};

using solution = std::pair<permutation, double>;
