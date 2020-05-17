//#ifdef _DEBUG
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
//// allocations to be of _CLIENT_BLOCK type
//#else
//#define DBG_NEW new
//#endif
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
#include "genetic/utils.h"

#include <iostream>
#include <random>
#include <numeric>
#include <string.h>
#include <chrono>

#include "QAP.h"

int main(int argc, char* argv[])
{
	//{
	//	std::cout << "Started." << std::endl;
	//	utils::FileHandler<std::size_t> handler_tasks("chr12a_tasks.txt");
	//	utils::FileHandler<std::size_t> handler_nodes("chr12a_nodes.txt");
	//	//utils::FileHandler<std::size_t> handler_tasks("tasks.txt");
	//	//utils::FileHandler<std::size_t> handler_nodes("nodes.txt");

	//	executors exec = seq_executor;
	//	lower_bounds lower = greedy_incorrect;
	//	upper_bounds upper = genetic_bound;
	//	bool concurrent = false;
	//	//

	//	auto matrix_tasks = handler_tasks.GetMatrix();
	//	auto matrix_nodes = handler_nodes.GetMatrix();
	//	std::cout << "Matrix are read successful" << std::endl;

	//	QAP qap(&matrix_tasks, &matrix_nodes);
	//	auto start = std::chrono::high_resolution_clock::now();
	//	auto result = qap.get_solution(exec, lower, upper, concurrent, true);
	//	auto end = std::chrono::high_resolution_clock::now();

	//	std::cout << "Result = " << result.second << std::endl;

	//	for (std::size_t idx = 0; idx < result.first.determined_size(); ++idx) {
	//		std::cout << result.first[idx] << " ";
	//	}
	//	std::cout << std::endl;

	//	int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	//	std::cout << "Elapsed time = " << elapsed_seconds << std::endl;
	//}
	//return 0;
	{
		if (argc < 6) {
			std::terminate();
		}
		std::cout << "Started." << std::endl;
		utils::FileHandler<std::size_t> handler_tasks(argv[1]);
		utils::FileHandler<std::size_t> handler_nodes(argv[2]);

		executors exec = seq_executor;
		lower_bounds lower = degenerate;
		upper_bounds upper = random_bound;
		bool concurrent = false;
		bool is_approximate = false;
		//
		if (!strcmp(argv[3], "seq")) {
			exec = seq_executor;
		}
		//
		if (!strcmp(argv[4], "deg")) {
			lower = degenerate;
		}
		if (!strcmp(argv[4], "greedy")) {
			lower = greedy_incorrect;
		}
		//
		if (!strcmp(argv[5], "random")) {
			upper = random_bound;
		}
		if (!strcmp(argv[5], "genetic")) {
			upper = genetic_bound;
		}
		//
		if (argv[6] != nullptr) {
			if (!strcmp(argv[6], "multi")) {
				concurrent = true;
			}
		}
		if (argc > 7) {
			if (!strcmp(argv[7], "approximate")) {
				is_approximate = true;
				printf("true\n");
			}
		}

		auto matrix_tasks = handler_tasks.GetMatrix();
		auto matrix_nodes = handler_nodes.GetMatrix();
		std::cout << "Matrix are read successful" << std::endl;

		QAP qap(&matrix_tasks, &matrix_nodes);
		auto start = std::chrono::high_resolution_clock::now();
		auto result = qap.get_solution(exec, lower, upper, concurrent, is_approximate);
		auto end = std::chrono::high_resolution_clock::now();

		std::cout << "Result = " << result.second << std::endl;

		for (std::size_t idx = 0; idx < result.first.determined_size(); ++idx) {
			std::cout << result.first[idx] << " ";
		}
		std::cout << std::endl;

		int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Elapsed time = " << elapsed_seconds << std::endl;
	}
	 //_CrtDumpMemoryLeaks();
}