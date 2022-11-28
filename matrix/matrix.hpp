#pragma once

#include <algorithm>
#include <vector>

template <size_t n, size_t m, typename T = int64_t>
class Matrix {
 public:
  // Конструктор по умолчанию заполняет матрицу T().
  Matrix() {
    data_.resize(n);
    for (std::vector<T>& row : data_) {
      row.resize(m);
    }
  }

  // Конструктор от std::vector<std::vector<T>>,
  // заполняющий матрицу элементами вектора.
  // Гарантируется, что размеры вектора будут
  // совпадать с размерами в шаблонах.
  Matrix(const std::vector<std::vector<T>>& init_vector) {
    data_.resize(n);
    for (size_t i = 0; i < n; i++) {
      data_[i] = init_vector[i];
    }
  }

  // Конструктор от T elem. Заполняет всю матрицу elem.
  Matrix(const T& elem) {
    data_.resize(n);
    for (std::vector<T>& row : data_) {
      row.resize(m, elem);
    }
  }

  ~Matrix() {}

  /*
   * Сложение, вычитание, операторы +=, -=.
   * Сложение и вычитание матриц несоответствующих
   * размеров не должно компилироваться.
   * Гарантируется, что типы в матрицах в тестах
   * будут поддерживать соответствующие операции.
   */

  // Операторы +=
  Matrix& operator+=(const Matrix& to_add) {
    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < m; j++) {
        data_[i][j] += to_add.data_[i][j];
      }
    }
    return *this;
  }

  // Сложение
  Matrix operator+(const Matrix& to_add) const {
    Matrix result(data_);
    result += to_add;
    return result;
  }

  // Операторы -=
  Matrix operator-(const Matrix& to_sub) const {
    Matrix result(data_);
    result -= to_sub;
    return result;
  }

  // Вычитание
  Matrix& operator-=(const Matrix& to_sub) {
    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < m; j++) {
        data_[i][j] -= to_sub.data_[i][j];
      }
    }
    return *this;
  }

  // Умножение на элемент типа T
  // (гарантируется, что оператор * определен для T)
  Matrix& operator*=(const T& factor) {
    for (std::vector<T>& row : data_) {
      for (T& element : row) {
        element *= factor;
      }
    }
    return *this;
  }

  // Умножение на элемент типа T
  // (гарантируется, что оператор * определен для T)
  friend Matrix operator*(const Matrix& matrix, const T& factor) {
    Matrix result(matrix.data_);
    result *= factor;
    return result;
  }

  // Умножение на элемент типа T
  // (гарантируется, что оператор * определен для T)
  friend Matrix operator*(const T& factor, const Matrix& matrix) {
    Matrix result(matrix.data_);
    result *= factor;
    return result;
  }

  // Умножение двух матриц.
  // Попытка перемножить матрицы несоответствующих размеров
  // должна приводить к ошибке компиляции.
  template <size_t k>
  Matrix<n, k, T> operator*(const Matrix<m, k, T>& factor) const {
    Matrix<n, k, T> result;
    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < k; j++) {
        for (size_t l = 0; l < m; l++) {
          result[i][j] += data_[i][l] * factor[l][j];
        }
      }
    }
    return result;
  }

  template <size_t k>
  Matrix operator*=(const Matrix<m, k, T>& factor) {
    Matrix result = *this * factor;

    data_.resize(n);
    for (size_t i = 0; i < n; i++) {
      data_[i] = result[i];
    }

    return *this;
  }

  // Метод Transposed(), возвращающий транспонированную матрицу.
  Matrix<m, n, T> Transposed() const {
    Matrix<m, n, T> transposed;
    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < m; j++) {
        transposed[j][i] = data_[i][j];
      }
    }
    return transposed;
  }

  // Метод Trace() - вычислить след матрицы.
  // Вычисление следа от неквадратной
  // матрицы не должно компилироваться.
  // template <typename = std::enable_if<n == m>>
  T Trace() const requires(n == m) {
    T result;
    for (size_t i = 0; i < n; i++) {
      result += data_[i][i];
    }
    return result;
  }

  // Оператор (i, j), возвращающий элемент матрицы в i-й строке и в j-м столбце.
  // Необходимо уметь менять значение для неконстантных матриц.
  T& operator()(size_t i, size_t j) { return data_[i][j]; }
  const T& operator()(size_t i, size_t j) const { return data_[i][j]; }
  std::vector<T>& operator[](size_t i) { return data_[i]; }
  const std::vector<T>& operator[](size_t i) const { return data_[i]; }

  // Оператор проверки на равенство.
  bool operator==(const Matrix& to_cmp) const { return data_ == to_cmp.data_; }

 private:
  std::vector<std::vector<T>> data_;
};