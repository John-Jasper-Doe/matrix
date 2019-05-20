/**
 ******************************************************************************
 * @file    sparse_matrix.h
 * @author  Maxim <aveter@bk.ru>
 * @date    19/05/2019
 * @brief   Description of the template "Sparse matrix".
 ******************************************************************************
 */

#ifndef SPARSE_MATRIX_H_
#define SPARSE_MATRIX_H_

#include <unistd.h>
#include <map>
#include <utility>
#include <tuple>


/**
 *  @brief The sparse matrix template class.
 *  @tparam value_type_t - matrix element type.
 *  @tparam default_value - default value (default -1).
 *  @tparam matrix_size - matrix size (default 2).
 */
template<typename value_type_t, value_type_t default_value = -1,
                                                        size_t matrix_size = 2>
class sparse_matrix
{
  private:
    /**
     *  @brief The struct for generate other index type for matrix.
     *  @tparam index_type_t - index type.
     *  @tparam index - index.
     *  @tparam types_t - other input tupes.
     */
    template<typename index_type_t, unsigned index, typename... types_t>
    struct generate_index_type {
        using type = typename generate_index_type<index_type_t,
                                                  index - 1,
                                                  index_type_t,
                                                  types_t...>::type;
    };

    /**
     *  @brief The structure for generating an index type with an index of 0.
     *  @tparam index_type_t - index type.
     *  @tparam types_t - other input tupes.
     */
    template<typename index_type_t, typename... types_t>
    struct generate_index_type<index_type_t, 0, types_t...> {
        using type = std::tuple<types_t...>;
    };

    /* Aliases */
    using index_t = typename generate_index_type<size_t, matrix_size>::type;
    using contener_t = typename std::map<index_t, value_type_t>;

  public:
    /**
     * The default constructor.
     */
    sparse_matrix() = default;

    /**
     *  @brief Copy constructor.
     *  @param other [in] - the object to copy.
     */
    sparse_matrix(const sparse_matrix &other)
      : data_(other.data_) {}

    /**
     *  @brief Move constructor.
     *  @param other [in] - the object to move.
     */
    sparse_matrix(sparse_matrix &&other)
      : data_(std::move(other.data_)) {}

    /**
     * The default destructor.
     */
    virtual ~sparse_matrix() = default;

    /**
     *  @brief Copy operator.
     *  @param other [in] - the object to copy.
     */
    sparse_matrix & operator=(const sparse_matrix &other) {
      if (*this != other)
        data_ = other.data_;

      return *this;
    }

    /**
     *  @brief Move operator.
     *  @param other [in] - the object to move.
     */
    sparse_matrix & operator=(sparse_matrix &&other) {
      data_ = std::move(other.data_);
      return *this;
    }

    /**
     *  @brief Comparison operator.
     *  @param other [in] - the object to move.
     */
    bool operator==(const sparse_matrix &other) const {
      return data_ == other.data_;
    }


  private:
    std::map<index_t, value_type_t> data_;            /** - data container. */
    const value_type_t default_value_{default_value}; /** - default value. */
};

#endif /* SPARSE_MATRIX_H_ */
