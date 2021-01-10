#pragma once
#include <set>
#include <vector>
#include <assert.h>

static constexpr std::size_t qap_alignment = 128;

#if USE_TBB && (_WIN32 || _WIN64)
#include <tbb/scalable_allocator.h>
template<typename T>
using allocator = tbb::scalable_allocator<T>;
#else
template<typename T>
using allocator = std::allocator<T>;
#endif

#if USE_TBB
#include <tbb/cache_aligned_allocator.h>
template<typename T>
using vector_allocator = tbb::cache_aligned_allocator<T>;
#else
template<typename T>
using vector_allocator = allocator<T>;
#endif

using set_t = std::set<std::size_t, std::less<std::size_t>, allocator<std::size_t>>;
using mask_t = unsigned long long;

class alignas(qap_alignment) permutation {
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
	set_t get_set_unused();
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

	alignas(qap_alignment) std::vector<std::size_t, vector_allocator<std::size_t>> my_permutation;
	alignas(qap_alignment) std::size_t my_determined_size;
	alignas(qap_alignment) std::size_t my_size;
	alignas(qap_alignment) mask_t my_unused_indexes;
};

using solution = std::pair<permutation, std::size_t>;
