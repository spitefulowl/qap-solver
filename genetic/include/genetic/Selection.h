#pragma once
#include "utils.h"

namespace genetic {

class BaseSelection {
public:
	BaseSelection();
	void init(utils::Matrix* data_volume, utils::Matrix* transfer_cost, utils::Matrix* population, utils::Matrix* descendants, std::size_t betta = 0);
	std::vector<std::vector<std::size_t>>* exec();
protected:
	virtual std::vector<std::vector<std::size_t>>* internal_selection(std::size_t size) = 0;
	std::vector<std::vector<std::size_t>>* population;
	std::vector<std::vector<std::size_t>>* descendants;
	utils::Matrix* data_volume;
	utils::Matrix* transfer_cost;
	utils::calculator calc;
	std::size_t beta;
private:
};

class BetaTournament : public BaseSelection {
public:
	using BaseSelection::BaseSelection;
private:
	std::vector<std::vector<std::size_t>>* internal_selection(std::size_t size);
};
// Broken

//class ProportionalSelection : public BaseSelection {
//public:
//	using BaseSelection::BaseSelection;
//private:
//	std::vector<std::vector<std::size_t>>* internal_selection(std::size_t size);
//};

}