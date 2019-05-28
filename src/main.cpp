/**
 ******************************************************************************
 * @file    main.cpp
 * @author  Maxim <maxim_gusev@bk.ru>
 * @date    25/05/2019
 * @brief   The main project file is "Matrix".
 ******************************************************************************
 */

#include "sparse_matrix.hpp"

#include <iostream>
#include <iomanip>


/**
 * @brief Output matrix on the screen.
 * @tparam T - matrix element type.
 * @tparam DEFAULT_VALUE - default value (default -1).
 * @tparam MATRIX_SIZE - matrix size (default 2).
 * @param matrix [in] - input data for output.
 */
template<typename T, T DEFAULT_VALUE = -1, size_t MATRIX_SIZE = 2>
void print_matrix(sparse_matrix<T, DEFAULT_VALUE, MATRIX_SIZE> &matrix)
{
  for (size_t j = 0; j < matrix.n_size(); ++j) {
    for (size_t i = 0; i < matrix.m_size(); ++i) {
      std::cout << std::setw(3) << matrix[i][j] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << std::endl;
}


/**
 * @brief Main function / entry point.
 * @return Execution status.
 */
int main()
{
  /* Example of initialization by the list of a one-dimensional matrix. */
  sparse_matrix<int> m1{2,4,7,9};

  std::cout << "Matrix m1{2,4,7,9}:" << std::endl;
  std::cout << "Size m1: " << m1.m_size() << "x" <<  m1.n_size() << std::endl;
  print_matrix(m1);

  /* Example of initialization by the list of a multidimensional matrix. */
  sparse_matrix<int> m2{{3,4,5,6,7}, {2,4,7,9}, {1,2,3,4,5}};

  std::cout << "Matrix m2{{3,4,5,6,7}, {2,4,7,9}, {1,2,3,4,5}}:" << std::endl;
  std::cout << "Size m2: " << m2.m_size() << "x" <<  m2.n_size() << std::endl;
  print_matrix(m2);

  /* Example of initialization by index of a multidimensional matrix. */
  sparse_matrix<int> m3;

  std::cout << "Matrix m3 (Init by index):" << std::endl;
  for (size_t j = 0; j < 5; ++j) {
    for (size_t i = 0; i < 7; ++i) {
      m3[i][j] = static_cast<int>(i);
    }
  }

  std::cout << "Size m3: " << m3.m_size() << "x" <<  m3.n_size() << std::endl;
  print_matrix(m3);


  /* Example of multiplying a matrix by a number. */
  sparse_matrix<int> m4 = m3 * 4;
  std::cout << "Multiplying m3 on number 4 (m4 = m3 * 4):" << std::endl;
  std::cout << "Size m4: " << m4.m_size() << "x" <<  m4.n_size() << std::endl;
  print_matrix(m4);

  /* Example of the addition of two matrixs. */
  sparse_matrix<int> m5 = m3 + m4;
  std::cout << "Addition m3 and m4 (m5 = m3 + m4):" << std::endl;
  std::cout << "Size m5: " << m5.m_size() << "x" <<  m5.n_size() << std::endl;
  print_matrix(m5);

  /* Example of iterated output */
  for (const std::tuple<size_t, size_t, int> &elem : m3) {
    const size_t &hindex = std::get<0>(elem);
    const size_t &vindex = std::get<1>(elem);
    const int &value = std::get<2>(elem);

    std::cout << "[" << hindex << ", " << vindex << "] = " << value << '\n';
  }

  std::cout << std::endl;

  return 0;
}

