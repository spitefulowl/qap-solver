#pragma once
#include "permutation.h"
#include <random>

struct randomizer {
public:
	randomizer();
	~randomizer();
	std::size_t operator()(std::size_t max);
private:
	std::random_device rd;
	std::mt19937 generator;
};

struct calculator {
public:
	calculator(Matrix* data, Matrix* cost);
	double criterion(permutation* permutation); // restore const modifier, use const_cast
	~calculator();
private:
	Matrix* data_volume; // ����� ������� ����� �����������
	Matrix* transfer_cost; // ��������� ������ ������� ����� �������������
};