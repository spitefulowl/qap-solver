#pragma once
#include "utils.h"

namespace genetic {

class BaseMutation {
public:
	BaseMutation();
	void init(std::vector<std::vector<std::size_t>>* descendants, std::size_t perc_probability, std::size_t start_idx);
	void exec();
protected:
	std::vector<std::vector<std::size_t>>* descendants;
	std::size_t start_idx;
	virtual void internal_mutation(std::vector<std::size_t>& mutant) = 0;
private:
	std::size_t perc_probability;
};

class PointMutation : public BaseMutation {
public:
	using BaseMutation::BaseMutation;
private:
	void internal_mutation(std::vector<std::size_t>& mutant);
};

class InversionMutation : public BaseMutation {
public:
	using BaseMutation::BaseMutation;
private:
	void internal_mutation(std::vector<std::size_t>& mutant);
};

}