/**
 ******************************************************************************
 * @file    matrix.hpp
 * @author  Maxim <aveter@bk.ru>
 * @date    25/05/2019
 * @brief   Description of the template "Matrix".
 ******************************************************************************
 */

#ifndef JJD_MATRIX_HPP_
#define JJD_MATRIX_HPP_

#include <unistd.h>
#include <map>
#include <utility>
#include <tuple>
#include <iostream>
#include <initializer_list>


namespace jjd {

/**
 * @brief The sparse matrix template class.
 * @tparam T - matrix element type.
 * @tparam DEFAULT_VALUE - default value (default -1).
 * @tparam MATRIX_SIZE - matrix size (default 2).
 */
template<typename T, T DEFAULT_VALUE = -1, size_t MATRIX_SIZE = 2>
class matrix
{
  private:
    /**
     * @brief The struct for generate other index type for matrix.
     * @tparam index_type_t - index type.
     * @tparam index - index.
     * @tparam types_t - other input tupes.
     */
    template<typename index_type_t, unsigned index, typename... types_t>
    struct generate_index_type {
        using type = typename generate_index_type<index_type_t,
                                                  index - 1,
                                                  index_type_t,
                                                  types_t...>::type;
    };

    /**
     * @brief The structure for generating an index type with an index of 0.
     * @tparam index_type_t - index type.
     * @tparam types_t - other input tupes.
     */
    template<typename index_type_t, typename... types_t>
    struct generate_index_type<index_type_t, 0, types_t...> {
        using type = std::tuple<types_t...>;
    };

    /**
     * @brief Class to work with matrix index.
     * @tparam num - index number.
     * @tparam types_t - index types.
     */
    template<size_t num, typename... types_t>
    class index_matrix;

    /**
     * @brief Class to work with matrix index, if index = 0.
     * @tparam types_t - index types.
     */
    template<typename... types_t>
    class index_matrix<0, types_t...>;

    /**
     * Class that implements an iterator for working with matrix data.
     */
    class iterator_matrix;

    /**
     * Class that implements an const iterator for working with matrix data.
     */
    class const_iterator_matrix;


    /* Aliases */
    using index_t = typename generate_index_type<size_t, MATRIX_SIZE>::type;
    using contener_t = typename std::map<index_t, T>;
    using next_index = index_matrix<MATRIX_SIZE - 1, size_t>;
    using init_list_t = typename std::initializer_list<T>;


  public:
    /**
     * The default constructor.
     */
    matrix() = default;

    /**
     * @brief The constructor with a list of initialization.
     * @param list [in] - list with initialization parameters.
     */
    matrix(const init_list_t &list) {
      size_t idx_m = 0;
      for (auto &value : list) {
        add(idx_m, 0, value);
        ++idx_m;
      }
    }

    /**
     * @brief The constructor with a list of initialization parameter lists.
     * @param list [in] - list with initialization parameters.
     */
    matrix(const std::initializer_list<const init_list_t> &list) {
      size_t idx_n = 0;
      for (auto &sub_list : list) {
        size_t idx_m = 0;
        for (auto &value : sub_list) {
          add(idx_m, idx_n, value);
          ++idx_m;
        }
        ++idx_n;
      }
    }

    /**
     * @brief Copy constructor.
     * @param other [in] - the object to copy.
     */
    matrix(const matrix &other)
      : data_(other.data_) {}

    /**
     * @brief Move constructor.
     * @param other [in] - the object to move.
     */
    matrix(matrix &&other)
      : data_(std::move(other.data_)) {}

    /**
     * The default destructor.
     */
    virtual ~matrix() = default;

    /**
     * @brief Copy operator.
     * @param other [in] - the object to copy.
     */
    matrix & operator=(const matrix &other) {
      data_ = other.data_;
      return *this;
    }

    /**
     * @brief Move operator.
     * @param other [in] - the object to move.
     */
    matrix & operator=(matrix &&other) {
      data_ = std::move(other.data_);
      return *this;
    }

    /**
     * @brief Multiplication by integer operation.
     * @param value [in] - multiplication value.
     * @return Operation result.
     */
    matrix operator*(const int value) {
      matrix tmp;
      auto it_m = begin();
      for (; it_m != end(); ++it_m) {
        tmp[std::get<0>(*it_m)][std::get<1>(*it_m)] = std::get<2>(*it_m) *
                                                      value;
      }
      return tmp;
    }

    /**
     * @brief Multiplication by integer operation (to myself).
     * @param value [in] - multiplication value.
     * @return Operation result.
     */
    matrix operator*=(const int value) {
      auto it_m = begin();
      for (; it_m != end(); ++it_m) {
         std::get<2>(*it_m) *= value;
      }
      return *this;
    }

    /**
     * @brief Addition with another matrix.
     * @param other [in] - another matrix for addition.
     * @return Operation result.
     */
    matrix operator+(const matrix &other) {
      const std::tuple<size_t, size_t> m1 = size();
      const std::tuple<size_t, size_t> m2 = other.size();
      if (m1 != m2) {
        /* TODO: Added exeption */
      }

      matrix tmp;
      auto it_m1 = begin();
      auto it_m2 = other.cbegin();
      for (; it_m1 != end(); ++it_m1, ++it_m2) {
        tmp[std::get<0>(*it_m1)][std::get<1>(*it_m1)] = std::get<2>(*it_m1) +
                                                        std::get<2>(*it_m2);
      }
      return tmp;
    }

    /**
     * @brief Addition with another matrix (to myself)
     * @param other [in] - another matrix for addition.
     * @return Operation result.
     */
    matrix operator+=(const matrix &other) {
      const std::tuple<size_t, size_t> m1 = size();
      const std::tuple<size_t, size_t> m2 = other.size();
      if (m1 != m2) {
        /* TODO: Added exeption */
      }

      auto it_m1 = begin();
      auto it_m2 = other.cbegin();
      for (; it_m1 != end(); ++it_m1, ++it_m2) {
        std::get<2>(*it_m1) += std::get<2>(*it_m2);
      }
      return *this;
    }

    /**
     * @brief Comparison operator.
     * @param other [in] - the object to compare.
     */
    bool operator==(const matrix &other) const {
      return data_ == other.data_;
    }

    /**
     * @brief Comparison operator.
     * @param other [in] - the object to compare.
     */
    bool operator!=(const matrix &other) const {
      return data_ != other.data_;
    }

    /**
     * @brief Access operator by index.
     * @param index [in] - matrix index.
     */
    next_index operator[](const size_t index) {
      return next_index(static_cast<std::tuple<size_t>>(index), data_);
    }

    /**
     * @brief Constant access operator by index.
     * @param index [in] - matrix index.
     */
    const next_index operator[](const size_t index) const {
      return next_index(static_cast<std::tuple<size_t>>(index), data_);
    }

    /**
     * @brief  The begin iterator of the matrix.
     * @return Returns an iterator to the beginning of the matrix.
     */
    auto begin() {
      return iterator_matrix(data_.begin());
    }

    /**
     * @brief  The const begin iterator of the matrix.
     * @return Returns an const iterator to the beginning of the matrix.
     */
    const auto cbegin() const {
      return const_iterator_matrix(data_.cbegin());
    }

    /**
     * @brief  The end iterator of the matrix.
     * @return Returns an iterator to the end of the matrix.
     */
    auto end() {
      return iterator_matrix(data_.end());
    }

    /**
     * @brief  The const end iterator of the matrix.
     * @return Returns an const iterator to the end of the matrix.
     */
    const auto cend() const {
      return const_iterator_matrix(data_.cend());
    }

    /**
     * @brief The number of calls occupied by the data matrix.
     * @return The number of occupied calls.
     */
    size_t data_count() const {
      return data_.size();
    }

    /**
     * Clearing the matrix data.
     */
    void clear() {
      data_.clear();
    }

    /**
     * @brief "m" matrix size.
     * @return "m" size.
     */
    size_t m_size() {
      return std::get<0>(size()) + 1;
    }

    /**
     * @brief "n" matrix size.
     * @return "n" size.
     */
    size_t n_size() {
      return std::get<1>(size()) + 1;
    }


  private:
    std::map<index_t, T> data_;            /**< - data container. */
    const T default_value_{DEFAULT_VALUE}; /**< - default value. */

    /**
     * @brief Added value in matrix by index.
     * @param idx_m [in] - "m" index.
     * @param idx_n [in] - "n" index.
     * @param value [in] - data.
     */
    void add(unsigned idx_m, unsigned idx_n, T value) {
      data_[std::make_tuple(idx_m, idx_n)] = value;
    }

    /**
     * @brief Matrix size.
     * @return Tuple with size.
     */
    const std::tuple<size_t, size_t> & size() const {
      return (--data_.end())->first;
    }
};


/**
 * Class to work with matrix index.
 */
template<typename T, T DEFAULT_VALUE, size_t MATRIX_SIZE>
template<size_t num, typename... types_t>
class matrix<T, DEFAULT_VALUE, MATRIX_SIZE>::index_matrix
{
  private:
    /* Aliases */
    using next_index = index_matrix<num - 1, types_t..., size_t>;

    const matrix::contener_t &data_;      /**< - container with data. */
    std::tuple<types_t...> index_view_;   /**< - representation of the index
                                           *     as a tuple. */

  public:
    /**
     * @brief The constructor.
     * @param index_view [in] - matrix index.
     * @param data [in] - contener.
     */
    index_matrix(std::tuple<types_t...> index_view,
                 const matrix::contener_t &data)
      : data_{data}, index_view_{index_view}
    {}

    /**
     * @brief Access operator by index.
     * @param index [in] - matrix index.
     */
    next_index operator[](size_t index) {
      return next_index(std::tuple_cat(index_view_, std::tie(index)), data_);
    }

    /**
     * @brief Constant access operator by index.
     * @param index [in] - matrix index.
     */
    const next_index operator[](size_t index) const {
      return next_index(std::tuple_cat(index_view_, std::tie(index)), data_);
    }
};


/**
 * Class to work with matrix index, if index = 0.
 */
template<typename T, T DEFAULT_VALUE, size_t MATRIX_SIZE>
template<typename... types_t>
class matrix<T, DEFAULT_VALUE, MATRIX_SIZE>::index_matrix<0, types_t...>
{
  private:
    const matrix::contener_t &data_;        /**< - container with data. */
    std::tuple<types_t...> index_view_;     /**< - representation of the
                                             *     index as a tuple. */
    const T default_value_{DEFAULT_VALUE};

  public:
    /**
     * @brief The constructor.
     * @param index_view [in] - matrix index.
     * @param data [in] - contener.
     */
    index_matrix(std::tuple<types_t...> index_view,
                 const matrix::contener_t &data)
      : data_{data}, index_view_{index_view}
    {}

    /**
     * @brief Copy operator.
     * @param value [in] - matrix value.
     */
    auto & operator=(const T &value) {
      if (value != default_value_)
        const_cast<matrix::contener_t &>(data_)[index_view_] = value;
      else {
        auto it = data_.find(index_view_);
        if (it != data_.cend())
          const_cast<matrix::contener_t &>(data_).erase(it);
      }
      return *this;
    }

    /**
     * Conversion operator.
     */
    operator const T &() const {
      auto it = data_.find(index_view_);
      return (it != data_.cend()) ? it->second : default_value_;
    }
};


/**
 * Class that implements an iterator for working with matrix data.
 */
template<typename T, T DEFAULT_VALUE, size_t MATRIX_SIZE>
class matrix<T, DEFAULT_VALUE, MATRIX_SIZE>::iterator_matrix
{
  private:
    /* Aliases */
    using iterator_t = typename matrix::contener_t::iterator;
    using iterator_category_t = std::input_iterator_tag;

    iterator_t map_iterator_;   /**< - iterator of the data. */


  public:
   using value_t = decltype(std::tuple_cat((*map_iterator_).first,
                                          std::tie((*map_iterator_).second)));

    /**
     * @brief The constructor.
     * @param map_iterator [in] - iterator.
     */
    explicit iterator_matrix(iterator_t map_iterator)
      : map_iterator_(map_iterator)
    {}

    /**
     * @brief  Increment operator, prefix form.
     * @return Increment data.
     */
    iterator_matrix & operator++() {
      ++map_iterator_;
      return *this;
    }

    /**
     * @brief  Increment operator, postfix form.
     * @return Increment data.
     */
    iterator_matrix operator++(int) {
      iterator_matrix retval = *this;
      ++(*this);
      return retval;
    }

    /**
     * @brief  Comparison operator.
     * @param  other [in] - iterator.
     * @return The result of the comparison (false/true).
     */
    bool operator==(iterator_matrix other) const {
      return map_iterator_ == other.map_iterator_;
    }

    /**
     * @brief  Inequality operator.
     * @param  other [in] - iterator.
     * @return The result of the comparison (false/true).
     */
    bool operator!=(iterator_matrix other) const {
      return !(*this == other);
    }

    /**
     * @brief  Dereference operator.
     * @return Result.
     */
    value_t operator*() {
      return std::tuple_cat((*map_iterator_).first,
                            std::tie((*map_iterator_).second));
    }
};


/**
 * Class that implements an const iterator for working with matrix data.
 */
template<typename T, T DEFAULT_VALUE, size_t MATRIX_SIZE>
class matrix<T, DEFAULT_VALUE, MATRIX_SIZE>::const_iterator_matrix
{
  private:
    /* Aliases */
    using iterator_t = typename matrix::contener_t::const_iterator;
    using iterator_category_t = std::input_iterator_tag;

    iterator_t map_iterator_;   /**< - iterator of the data. */


  public:
   using value_t = decltype(std::tuple_cat((*map_iterator_).first,
                                          std::tie((*map_iterator_).second)));

    /**
     * @brief The constructor.
     * @param map_iterator [in] - iterator.
     */
    explicit const_iterator_matrix(iterator_t map_iterator)
      : map_iterator_(map_iterator)
    {}

    /**
     * @brief  Increment operator, prefix form.
     * @return Increment data.
     */
    const_iterator_matrix & operator++() {
      ++map_iterator_;
      return *this;
    }

    /**
     * @brief  Increment operator, postfix form.
     * @return Increment data.
     */
    const_iterator_matrix operator++(int) {
      iterator_matrix retval = *this;
      ++(*this);
      return retval;
    }

    /**
     * @brief  Comparison operator.
     * @param  other [in] - iterator.
     * @return The result of the comparison (false/true).
     */
    bool operator==(const_iterator_matrix other) const {
      return map_iterator_ == other.map_iterator_;
    }

    /**
     * @brief  Inequality operator.
     * @param  other [in] - iterator.
     * @return The result of the comparison (false/true).
     */
    bool operator!=(const_iterator_matrix other) const {
      return !(*this == other);
    }

    /**
     * @brief  Dereference operator.
     * @return Result.
     */
    value_t operator*() const {
      return std::tuple_cat((*map_iterator_).first,
                            std::tie((*map_iterator_).second));
    }
};

} /* namespace jjd */

#endif /* JJD_MATRIX_HPP_ */
