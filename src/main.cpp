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

  return 0;
}

