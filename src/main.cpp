/**
 ******************************************************************************
 * @file    main.cpp
 * @author  Maxim <aveter@bk.ru>
 * @date    19/05/2019
 * @brief   The main project file is "Matrix".
 ******************************************************************************
 */

#include "sparse_matrix.hpp"

#include <iostream>
#include <cassert>
#include <iomanip>


/** @brief Main function / entry point. */
int main()
{
  const int num_elem = 10;
  sparse_matrix<int> matrix;

  /* Initialization of the main diagonal of the matrix */
  for (size_t i = 0; i < num_elem; ++i) {
    matrix[i][i] = static_cast<int>(i);
  }

  /* Initialization of the secondary diagonal of the matrix */
  for (size_t i = 0; i < num_elem; ++i) {
    matrix[i][(num_elem - 1) - i] = static_cast<int>((num_elem - 1) - i);
  }

  /* Output matrix values from range [1,1] to [8,8] */
  for (size_t i = 1; i <= 8; ++i) {
    for (size_t j = 1; j <= 8; ++j) {
      std::cout << std::setw(3) << matrix[i][j] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << std::endl;

  /* Output the number of matrix cells occupied by the data */
  std::cout << matrix.size() << '\n' << std::endl;


  return 0;
}

