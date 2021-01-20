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
#include "utils.h"

#include <iostream>
#include <random>
#include <numeric>
#include <string.h>
#include <chrono>

#include "qap.h"

int main(int argc, char* argv[])
{
	if (argc < 6)
	{
		std::cout << "Started." << std::endl;
		utils::matrix_reader<std::size_t> handler_tasks("chr12a_tasks.txt");
		utils::matrix_reader<std::size_t> handler_nodes("chr12a_nodes.txt");
		//utils::matrix_reader<std::size_t> handler_tasks("tasks.txt");
		//utils::matrix_reader<std::size_t> handler_nodes("nodes.txt");

		executors exec = par_executor;
		lower_bounds lower = greedy_incorrect;
		upper_bounds upper = genetic_bound;
		bool concurrent = true;
		//

		auto matrix_tasks = handler_tasks.get_matrix();
		auto matrix_nodes = handler_nodes.get_matrix();
		std::cout << "Matrix are read successful" << std::endl;

		qap my_qap(matrix_tasks, matrix_nodes);
		auto start = std::chrono::high_resolution_clock::now();
		auto result = my_qap.get_solution(exec, lower, upper, concurrent);
		auto end = std::chrono::high_resolution_clock::now();

		std::cout << "Result = " << result.second << std::endl;

		std::cout << std::endl;

		auto elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Elapsed time = " << elapsed_seconds << std::endl;
		return 0;
	}

	{
		std::cout << "Started." << std::endl;
		utils::matrix_reader<std::size_t> handler_tasks(argv[1]);
		utils::matrix_reader<std::size_t> handler_nodes(argv[2]);

		executors exec = seq_executor;
		lower_bounds lower = degenerate;
		upper_bounds upper = random_bound;
		bool concurrent = false;
		std::size_t approximate_level = 0;
		//
		if (!strcmp(argv[3], "seq")) {
			exec = seq_executor;
		}
		#ifdef USE_TBB
		if (!strcmp(argv[3], "par")) {
			exec = par_executor;
		}
		#endif
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
		if (!strcmp(argv[5], "optimized")) {
			upper = optimized_bound;
		}
		//
		if (argv[6] != nullptr) {
			if (!strcmp(argv[6], "multi")) {
				std::printf("concurrent\n");
				concurrent = true;
			}
		}
		if (argc > 7) {
			if (!strcmp(argv[7], "approximate")) {
				if (argv[8] != nullptr) {
					approximate_level = static_cast<std::size_t>(atoi(argv[8]));
				}
				printf("approximate: %llu\n", approximate_level);
			}
		}

		auto matrix_tasks = handler_tasks.get_matrix();
		auto matrix_nodes = handler_nodes.get_matrix();
		std::cout << "Matrix are read successful" << std::endl;

		qap my_qap(matrix_tasks, matrix_nodes);

		auto start = std::chrono::high_resolution_clock::now();
		auto result = my_qap.get_solution(exec, lower, upper, concurrent, approximate_level);
		auto end = std::chrono::high_resolution_clock::now();

		std::cout << "Result = " << result.second << std::endl;

		std::cout << std::endl;

		auto elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Elapsed time = " << elapsed_seconds << std::endl;

	}
	 //_CrtDumpMemoryLeaks();
}