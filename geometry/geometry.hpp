#pragma once
#include <cstdint>

// Класс Vector для вектора на плоскости
class Vector {
 public:
  // Конструктор по умолчанию (создает нулевой вектор)
  Vector() {}
  // Конструктор от двух целочисленных переменных
  // (создает вектор с соответствующими координатами)
  Vector(int64_t x_coord, int64_t y_coord) : x_(x_coord), y_(y_coord) {}
  ~Vector() {}

  // Оператор * для скалярного произведения
  int64_t operator*(const Vector& vector) const {
    return x_ * vector.x_ + y_ * vector.y_;
  }

  // Оператор ^ для векторного произведения
  // this(x1, y1),  v(x2, y2)
  //  | x1 x2 |
  //  | y1 y2 |
  // det = x1 * y2 - y1 * x2;
  //
  int64_t operator^(const Vector& vector) const {
    return x_ * vector.y_ - y_ * vector.x_;
  }

  // Операторы сложения/разности с другим вектором,
  // соответственно реализовать операторы += и -=
  Vector& operator+=(const Vector& vector) {
    x_ += vector.x_;
    y_ += vector.y_;
    return *this;
  }

  Vector& operator-=(const Vector& vector) {
    x_ -= vector.x_;
    y_ -= vector.y_;
    return *this;
  }

  friend Vector operator+(const Vector& left, const Vector& right) {
    Vector res = left;
    res += right;
    return res;
  }

  friend Vector operator-(const Vector& left, const Vector& right) {
    Vector res = left;
    res -= right;
    return res;
  }

  // Оператор умножения на число (сделать так, чтобы были допустимы как
  // умножение вектора на число, так и числа на вектор), реализовать оператор *=
  Vector& operator*=(int64_t scalar) {
    x_ *= scalar;
    y_ *= scalar;
    return *this;
  }

  friend Vector operator*(const Vector& vector, int64_t scalar) {
    Vector res = vector;
    res *= scalar;
    return res;
  }

  friend Vector operator*(int64_t scalar, const Vector& vector) {
    Vector res = vector;
    res *= scalar;
    return res;
  }

  // Унарный оператор минус для получения вектора в противоположном направлении
  Vector& operator-() {
    x_ = -x_;
    y_ = -y_;
    return *this;
  }
  Vector operator-() const { return Vector(-x_, -y_); }

  // Методы GetX() и GetY() для получения соответствующих координат
  int64_t GetX() const { return x_; }
  int64_t GetY() const { return y_; }

  bool operator==(const Vector& to_cmp) const {
    return (x_ == to_cmp.x_ && y_ == to_cmp.y_);
  }

 private:
  int64_t x_ = 0;
  int64_t y_ = 0;
};

class Point;

class Segment;

class IShape {
 public:
  IShape() {}
  virtual ~IShape() = default;

  // сдвиг фигуры на соответствующий вектор
  void Move(const Vector& shift) { base_ += shift; }

  // проверка, содержит ли фигура точку
  virtual bool ContainsPoint(const Point& point) const = 0;

  // проверка, пересекает ли отрезок фигуру
  virtual bool CrossSegment(const Segment& segment) const = 0;

  // вернуть указатель на копию фигуры
  virtual IShape* Clone() const = 0;

 protected:
  Vector base_;
};

class Point : public IShape {
 public:
  Point();
  Point(const Vector& vector);
  Point(int64_t x_coord, int64_t y_coord);
  ~Point();

  // проверка, содержит ли фигура точку
  bool ContainsPoint(const Point& point) const override;

  // проверка, пересекает ли отрезок фигуру
  bool CrossSegment(const Segment& segment) const override;

  // вернуть указатель на копию фигуры
  Point* Clone() const override;

  int64_t GetX() const;
  int64_t GetY() const;

  friend Vector operator-(const Point& first, const Point& second);
};

class Segment : public IShape {
 public:
  Segment();
  Segment(const Point& start, const Point& end);
  ~Segment();

  // проверка, содержит ли фигура точку
  bool ContainsPoint(const Point& point) const override;

  // проверка, пересекает ли отрезок фигуру
  bool CrossSegment(const Segment& segment) const override;

  // вернуть указатель на копию фигуры
  Segment* Clone() const override;

  // get start of segment
  Point GetA() const;

  // get end of segment
  Point GetB() const;

 private:
  Vector direction_vector_;
};

class Line : public IShape {
 public:
  Line();
  Line(const Point& start, const Point& end);
  ~Line();

  // проверка, содержит ли фигура точку
  bool ContainsPoint(const Point& point) const override;

  // проверка, пересекает ли отрезок фигуру
  bool CrossSegment(const Segment& segment) const override;

  // вернуть указатель на копию фигуры
  Line* Clone() const override;

  // коэффициент A уравнения прямой Ax + By + C
  int64_t GetA() const;

  // коэффициент B уравнения прямой Ax + By + C
  int64_t GetB() const;

  // коэффициент C уравнения прямой Ax + By + C
  int64_t GetC() const;

 private:
  Vector direction_vector_;
};

class Ray : public IShape {
 public:
  Ray();
  Ray(const Point& start, const Point& end);
  ~Ray();

  // проверка, содержит ли фигура точку
  bool ContainsPoint(const Point& point) const override;

  // проверка, пересекает ли отрезок фигуру
  bool CrossSegment(const Segment& segment) const override;

  // вернуть указатель на копию фигуры
  Ray* Clone() const override;

  // get endpoint
  Point GetA() const;

  // get direction vector
  Vector GetVector() const;

 private:
  Vector direction_vector_;
};

class Circle : public IShape {
 public:
  Circle();
  Circle(const Point& center, int64_t radius);
  ~Circle();

  // проверка, содержит ли фигура точку
  bool ContainsPoint(const Point& point) const override;

  // проверка, пересекает ли отрезок фигуру
  bool CrossSegment(const Segment& segment) const override;

  // вернуть указатель на копию фигуры
  Circle* Clone() const override;

  Point GetCentre() const;
  int64_t GetRadius() const;

 private:
  int64_t radius_ = 0;
};
