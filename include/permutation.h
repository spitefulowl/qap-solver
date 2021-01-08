#pragma once
#include <vector>
#include <set>
#include <assert.h>

#if USE_TBB && (_WIN32 || _WIN64)
#include <tbb/scalable_allocator.h>
using allocator = tbb::scalable_allocator<std::size_t>;
#else
using allocator = std::allocator<std::size_t>;
#endif

using set_t = std::set<std::size_t, std::less<std::size_t>, allocator>;
using mask_t = unsigned long long;

class permutation {
public:
	permutation(std::size_t size);

	std::size_t& get(std::size_t idx) {
		assert(idx < this->determined_size());
		return my_permutation[idx];
	}
	void set(std::size_t idx, std::size_t value);
	void copy_to(permutation& copy_to);
	void make_used(std::size_t value);
	void make_last_unused();
	void swap(std::size_t left, std::size_t right);
	void reverse(std::size_t begin, std::size_t end);

	mask_t get_unused() {
		return my_unused_indexes;
	}

	set_t get_set_unused() { // TODO: delete
		set_t result;
		for (std::size_t value = 0; value < size(); ++value) {
			if (my_unused_indexes & 1uLL << (value + 1)) {
				result.insert(value);
			}
		}
		return result;
	}

	bool next_permutation(std::size_t begin_pos, std::size_t end_pos);
	std::size_t determined_size() const {
		return my_determined_size;
	}
	std::size_t size() const {
		return my_size;
	}

private:
	void flip_mask_bit(std::size_t pos) {
		my_unused_indexes ^= 1uLL << (pos + 1);
	}

	std::vector<std::size_t, allocator> my_permutation;
	std::size_t my_determined_size;
	std::size_t my_size;
	mask_t my_unused_indexes;
};

using solution = std::pair<permutation, std::size_t>;
