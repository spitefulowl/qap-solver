#pragma once
#include "qap_executor.h"
#include "bound.h"

#include <vector>
#include <utility>

enum executors { seq_executor };
enum lower_bounds { degenerate, greedy_incorrect };
enum upper_bounds { random_bound, genetic_bound };

class QAP {
public:
	QAP();
	QAP(Matrix* data_volume, Matrix* transfer_cost);
	~QAP();
	solution get_solution(executors exec, lower_bounds lower, upper_bounds upper, bool is_concurrent, bool is_approximate);
	void set_data_volume(Matrix* matrix);
	void set_transfer_cost(Matrix* matrix);
	
private:
	Matrix* data_volume; // обмен данными между подзадачами
	Matrix* transfer_cost; // стоимость обмена данными между вычислителями
};
