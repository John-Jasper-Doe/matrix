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
  /* Example of initialization by the list of a one-dimensional matrix. */
  sparse_matrix<int> m1{2,4,7,9};

  std::cout << "Matrix m1{2,4,7,9}:" << std::endl;
  std::cout << "Size m1: " << m1.m_size() << "x" <<  m1.n_size() << std::endl;

  for (size_t j = 0; j < m1.n_size(); ++j) {
    for (size_t i = 0; i < m1.m_size(); ++i) {
      std::cout << std::setw(3) << m1[i][j] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << std::endl;

  /* Example of initialization by the list of a multidimensional matrix. */
  sparse_matrix<int> m2{{3,4,5,6,7}, {2,4,7,9}, {1,2,3,4,5}};

  std::cout << "Matrix m2{{3,4,5,6,7}, {2,4,7,9}, {1,2,3,4,5}}:" << std::endl;
  std::cout << "Size m2: " << m2.m_size() << "x" <<  m2.n_size() << std::endl;

  for (size_t j = 0; j < m2.n_size(); ++j) {
    for (size_t i = 0; i < m2.m_size(); ++i) {
      std::cout << std::setw(3) << m2[i][j] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << std::endl;

  /* Example of initialization by index of a multidimensional matrix. */
  sparse_matrix<int> m3;

  std::cout << "Matrix m3 (Init by index):" << std::endl;
  for (size_t j = 0; j < 5; ++j) {
    for (size_t i = 0; i < 7; ++i) {
      m3[i][j] = static_cast<int>(i);
    }
  }

  std::cout << "Size m2: " << m2.m_size() << "x" <<  m2.n_size() << std::endl;



  return 0;
}

