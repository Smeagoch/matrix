#include <iostream>
#include <assert.h>

#include "matrix.h"

int main()
{
	// бесконечная матрица int заполнена значениями -1
	Matrix<int, 0> matrix;
	assert(matrix.size() == 0); // все ячейки свободны

	auto a = matrix[0][0];
	assert(a == 0);
	assert(matrix.size() == 0);

	matrix[100][100] = 314;
	assert(matrix[100][100] == 314);
	assert(matrix.size() == 1);

	for(auto c: matrix)	{
		uint64_t x;
		uint64_t y;
		int v;
		std::tie(x, y, v) = c;
		std::cout << x << y << v << std::endl;
	}

	matrix[100][100] = 0;
	assert(matrix[100][100] == 0);
	assert(matrix.size() == 0);

	for (int i = 0; i < 10; ++i) {
		matrix[i][i] = i;
		matrix[i][9 - i] = i;
	}

	std::cout << matrix.size() << std::endl;

	for (int i = 1; i < 10; ++i) {
		for (int j = 1; j < 10; ++j)
			std::cout << matrix[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for(auto c: matrix)	{
		uint64_t row;
		uint64_t col;
		int val;
		std::tie(row, col, val) = c;
		std::cout << "[" << row << "]";
		std::cout << "[" << col << "] ";
		std::cout << val << std::endl;
	}

	return 0;
}
