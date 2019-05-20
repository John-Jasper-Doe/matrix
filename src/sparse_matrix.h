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

    /**
     *  @brief Class to work with matrix index.
     *  @tparam num - index number.
     *  @tparam types_t - index types.
     */
    template<size_t num, typename... types_t>
    class index_matrix;

    /**
     *  @brief Class to work with matrix index, if index = 0.
     *  @tparam types_t - index types.
     */
    template<typename... types_t>
    class index_matrix<0, types_t...>;


    /* Aliases */
    using index_t = typename generate_index_type<size_t, matrix_size>::type;
    using contener_t = typename std::map<index_t, value_type_t>;
    using next_index = index_matrix<matrix_size - 1, size_t>;


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


/**
 * Class to work with matrix index.
 */
template<typename value_type_t, value_type_t default_value, size_t matrix_size>
template<size_t num, typename... types_t>
class sparse_matrix<value_type_t, default_value, matrix_size>::index_matrix
{
  private:
    /* Aliases */
    using next_index = index_matrix<num - 1, types_t..., size_t>;

    const sparse_matrix::contener_t &data_; /** - container with data. */
    std::tuple<types_t...> index_view_;     /** - representation of the index
                                             *    as a tuple. */

  public:
    /**
     *  @brief The constructor.
     *  @param index_view [in] - matrix index.
     *  @param data [in] - contener.
     */
    index_matrix(std::tuple<types_t...> index_view,
                 const sparse_matrix::contener_t &data)
      : data_{data}, index_view_{index_view}
    {}

    /**
     *  @brief Access operator by index.
     *  @param index [in] - matrix index.
     */
    next_index operator[](size_t index) {
      return next_index(std::tuple_cat(index_view_, std::tie(index)), data_);
    }

    /**
     *  @brief Constant access operator by index.
     *  @param index [in] - matrix index.
     */
    const next_index operator[](size_t index) const {
      return next_index(std::tuple_cat(index_view_, std::tie(index)), data_);
    }
};


/**
 * Class to work with matrix index, if index = 0.
 */
template<typename value_type_t, value_type_t default_value, size_t matrix_size>
template<typename... types_t>
class sparse_matrix<value_type_t, default_value, matrix_size>::
    index_matrix<0, types_t...>
{
  private:
    const sparse_matrix::contener_t &data_;   /** - container with data. */
    std::tuple<types_t...> index_view_;       /** - representation of the index
                                               *    as a tuple. */
    const value_type_t default_value_{default_value};

  public:
    /**
     *  @brief The constructor.
     *  @param index_view [in] - matrix index.
     *  @param data [in] - contener.
     */
    index_matrix(std::tuple<types_t...> index_view,
                 const sparse_matrix::contener_t &data)
      : data_{data}, index_view_{index_view}
    {}

    /**
     *  @brief Copy operator.
     *  @param value [in] - matrix value.
     */
    auto & operator=(const value_type_t &value) {
      if (value != default_value_)
        const_cast<sparse_matrix::contener_t &>(data_)[index_view_] = value;
      else {
        auto it = data_.find(index_view_);
        if (it != data_.cend())
          const_cast<sparse_matrix::contener_t &>(data_).erase(it);
      }
      return *this;
    }

    /**
     * Conversion operator.
     */
    operator const value_type_t &() const {
      auto it = data_.find(index_view_);
      return (it != data_.cend()) ? it->second : default_value_;
    }
};


#endif /* SPARSE_MATRIX_H_ */
