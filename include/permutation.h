#pragma once
#include <vector>
#include <set>

class permutation {
public:
	permutation(std::size_t size);

	std::size_t get(std::size_t idx);
	void set(std::size_t idx, std::size_t value);
	void copy_to(permutation& copy_to);
	void make_used(std::size_t value);
	void make_last_unused();
	void swap(std::size_t left, std::size_t right);
	void reverse(std::size_t begin, std::size_t end);

	std::set<std::size_t>& get_unused();
	bool next_permutation(std::size_t begin_pos, std::size_t end_pos);
	std::size_t determined_size() const;
	std::size_t size() const;

private:
	std::vector<std::size_t> my_permutation;
	std::size_t my_determined_size;
	std::size_t my_size;
	std::set<std::size_t> my_unused_indexes;
};

using solution = std::pair<permutation, std::size_t>;
