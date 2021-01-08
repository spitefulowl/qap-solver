#include "permutation.h"
#include <numeric>
#include <algorithm>
#include <stdexcept>

permutation::permutation(std::size_t size) {
	my_size = size;
	my_determined_size = 0;
	my_permutation.resize(size);
	my_unused_indexes = (mask_t)-1;
}

void permutation::copy_to(permutation& copy_to) {
	for (std::size_t idx = 0; idx < this->determined_size(); ++idx) {
		copy_to.my_permutation[idx] = this->my_permutation[idx];
	}
	copy_to.my_determined_size = this->my_determined_size;
	copy_to.my_unused_indexes = this->my_unused_indexes;
}

void permutation::set(std::size_t idx, std::size_t value) {
	bool return_flag = true;
	if (idx == my_determined_size) {
		return_flag = false;
		my_determined_size++;
	}
	assert(my_determined_size <= my_size);
	assert(idx < my_determined_size);
	if (return_flag) flip_mask_bit(my_permutation[idx]); // my_unused_indexes.insert(my_permutation[idx]);
	flip_mask_bit(value);
	my_permutation[idx] = value;
}

void permutation::make_used(std::size_t value) {
	assert(my_determined_size < my_size);
	my_permutation[my_determined_size++] = value;
	flip_mask_bit(value);
}

void permutation::make_last_unused() {
	if (my_determined_size > 0) {
		flip_mask_bit(my_permutation[my_determined_size - 1]);
		my_determined_size--;
	}
}

void permutation::swap(std::size_t left, std::size_t right) {
	assert(left < my_determined_size);
	assert(right < my_determined_size);
	std::swap(my_permutation[left], my_permutation[right]);
}

void permutation::reverse(std::size_t begin, std::size_t end) {
	assert(begin < my_determined_size);
	assert(end <= my_determined_size);
	assert(begin <= end);
	std::reverse(my_permutation.begin() + begin, my_permutation.begin() + end);
}

bool permutation::next_permutation(std::size_t begin_pos, std::size_t end_pos) {
	assert(my_determined_size == my_size);
	assert(end_pos <= my_size);
	return std::next_permutation(my_permutation.begin() + begin_pos, my_permutation.begin() + end_pos);
}
