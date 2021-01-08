#pragma once
#include "utils.h"

namespace genetic {

class base_selection {
public:
	base_selection();
	virtual void init(utils::matrix_t* data_volume, utils::matrix_t* transfer_cost, std::vector<permutation>& population, std::vector<permutation>& descendants, std::size_t beta = 0);
	void exec();
protected:
	virtual void internal_selection(std::size_t descendants_count) = 0;
	std::vector<permutation>* my_population;
	std::vector<permutation>* my_descendants;
	utils::matrix_t* my_data_volume;
	utils::matrix_t* my_transfer_cost;
	utils::calculator* my_calculator;
	std::size_t my_beta;
	utils::randomizer random;
};

class beta_tournament : public base_selection {
public:
	using base_selection::base_selection;
private:
	virtual void init(utils::matrix_t* data_volume, utils::matrix_t* transfer_cost, std::vector<permutation>& population, std::vector<permutation>& descendants, std::size_t beta = 0) override;
	virtual void internal_selection(std::size_t descendants_count) override;
	std::vector<std::size_t> beta_tournament_indexes;
	std::vector<std::size_t> beta_tournament_criterions;
};

}
