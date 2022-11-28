#pragma once

#include <algorithm>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 public:
  // Конструктор по умолчанию заполняет матрицу T().
  Matrix() {
    this->data_.resize(N);
    for (std::vector<T>& row : this->data_) {
      row.resize(M);
    }
  }

  // Конструктор от std::vector<std::vector<T>>,
  // заполняющий матрицу элементами вектора.
  // Гарантируется, что размеры вектора будут
  // совпадать с размерами в шаблонах.
  Matrix(const std::vector<std::vector<T>>& init_vector) {
    this->data_.resize(N);
    for (size_t i = 0; i < N; i++) {
      this->data_[i] = init_vector[i];
    }
  }

  // Конструктор от T elem. Заполняет всю матрицу elem.
  Matrix(const T& elem) {
    this->data_.resize(N);
    for (std::vector<T>& row : this->data_) {
      row.resize(M, elem);
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
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < M; j++) {
        this->data_[i][j] += to_add.data_[i][j];
      }
    }
    return *this;
  }

  // Сложение
  Matrix operator+(const Matrix& to_add) const {
    Matrix result(this->data_);
    result += to_add;
    return result;
  }

  // Операторы -=
  Matrix operator-(const Matrix& to_sub) const {
    Matrix result(this->data_);
    result -= to_sub;
    return result;
  }

  // Вычитание
  Matrix& operator-=(const Matrix& to_sub) {
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < M; j++) {
        this->data_[i][j] -= to_sub.data_[i][j];
      }
    }
    return *this;
  }

  // Умножение на элемент типа T
  // (гарантируется, что оператор * определен для T)
  Matrix& operator*=(const T& factor) {
    for (std::vector<T>& row : this->data_) {
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
  template <size_t K>
  Matrix<N, K, T> operator*(const Matrix<M, K, T>& factor) const {
    Matrix<N, K, T> result;
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < K; j++) {
        for (size_t l = 0; l < M; l++) {
          result[i][j] += this->data_[i][l] * factor[l][j];
        }
      }
    }
    return result;
  }

  template <size_t K>
  Matrix operator*=(const Matrix<M, K, T>& factor) {
    Matrix result = *this * factor;

    this->data_.resize(N);
    for (size_t i = 0; i < N; i++) {
      this->data_[i] = result[i];
    }

    return *this;
  }

  // Метод Transposed(), возвращающий транспонированную матрицу.
  Matrix<M, N, T> Transposed() const {
    Matrix<M, N, T> transposed;
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < M; j++) {
        transposed[j][i] = this->data_[i][j];
      }
    }
    return transposed;
  }

  // Метод Trace() - вычислить след матрицы.
  // Вычисление следа от неквадратной
  // матрицы не должно компилироваться.
  template <typename = std::enable_if<N == M>>
  T Trace() const {
    T result;
    for (size_t i = 0; i < N; i++) {
      result += this->data_[i][i];
    }
    return result;
  }

  // Оператор (i, j), возвращающий элемент матрицы в i-й строке и в j-м столбце.
  // Необходимо уметь менять значение для неконстантных матриц.
  T& operator()(size_t i, size_t j) { return this->data_[i][j]; }
  const T& operator()(size_t i, size_t j) const { return this->data_[i][j]; }
  std::vector<T>& operator[](size_t i) { return this->data_[i]; }
  const std::vector<T>& operator[](size_t i) const { return this->data_[i]; }

  // Оператор проверки на равенство.
  bool operator==(const Matrix& to_cmp) const {
    return this->data_ == to_cmp.data_;
  }

 private:
  std::vector<std::vector<T>> data_;
};