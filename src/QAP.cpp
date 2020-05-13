#include "QAP.h"
#include "qap_executor.h"
#include "bound.h"
#include <stdexcept>


QAP::QAP() : data_volume(nullptr), transfer_cost(nullptr) {}

QAP::QAP(Matrix* data_volume, Matrix* transfer_cost) : data_volume(data_volume), transfer_cost(transfer_cost) {}

QAP::~QAP() {}

solution QAP::get_solution(executors exec, lower_bounds lower, upper_bounds upper, bool is_concurrent) {
	if (data_volume->size() == transfer_cost->size()) {
		base_executor* executor = nullptr;
		base_bound* lower_bound = nullptr;
		base_bound* upper_bound = nullptr;
		switch (lower)
		{
		case degenerate:
			lower_bound = new degenerate_lower_bound(data_volume, transfer_cost);
			break;
		case greedy_incorrect:
			lower_bound = new greedy_incorrect_lower_bound(data_volume, transfer_cost);
			break;
		}
		switch (upper)
		{
		case random_bound:
			upper_bound = new random_upper_bound(data_volume, transfer_cost);
			break;
		case genetic_bound:
			upper_bound = new genetic_upper_bound(data_volume, transfer_cost);
			break;
		}
		switch (exec)
		{
		case seq_executor:
			executor = new sequential_executor(data_volume, transfer_cost, lower_bound, upper_bound, is_concurrent);
			break;
		}
		solution result = executor->get_solution();
		delete executor;
		delete lower_bound;
		delete upper_bound;
		return result;
	}
	else {
		throw std::runtime_error("size of data_volume not equal to size of transfer_cost");
	}
}

void QAP::set_data_volume(Matrix* matrix)
{
	if (data_volume) {
		delete data_volume;
	}
	data_volume = matrix;
}

void QAP::set_transfer_cost(Matrix* matrix)
{
	if (transfer_cost) {
		delete transfer_cost;
	}
	transfer_cost = matrix;
}
