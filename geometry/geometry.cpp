#include "geometry.hpp"

#include <cstdint>

Point::Point() {}
Point::Point(const Vector& vector) { base_ = vector; }
Point::Point(int64_t x_coord, int64_t y_coord) {
  base_ = Vector(x_coord, y_coord);
}
Point::~Point() {}

bool Point::ContainsPoint(const Point& point) const {
  return base_ == point.base_;
}

bool Point::CrossesSegment(const Segment& segment) const {
  Vector vector_a = segment.GetA() - *this;
  Vector vector_b = segment.GetB() - *this;
  return (vector_a ^ vector_b) == 0 && (vector_a * vector_b) <= 0;
}

Point* Point::Clone() const { return new Point(base_); }

int64_t Point::GetX() const { return base_.GetX(); }
int64_t Point::GetY() const { return base_.GetY(); }

Vector operator-(const Point& first, const Point& second) {
  return Vector(first.GetX() - second.GetX(), first.GetY() - second.GetY());
}

Segment::Segment() {}
Segment::Segment(const Point& start, const Point& end) {
  base_ = Vector(start.GetX(), start.GetY());
  direction_vector_ = end - start;
}
Segment::~Segment() {}

bool Segment::ContainsPoint(const Point& point) const {
  return point.CrossesSegment(*this);
}

bool Segment::CrossesSegment(const Segment& segment) const {
  const Vector& dir1 = direction_vector_;
  const Vector& dir2 = segment.direction_vector_;

  if ((dir1 ^ dir2) == 0) {
    // segments are collinear;
    bool cross = ContainsPoint(segment.GetA()) ||
                 ContainsPoint(segment.GetB()) ||
                 segment.ContainsPoint(GetA()) || segment.ContainsPoint(GetB());
    return cross;
  }

  const Vector& base1 = base_;
  const Vector& base2 = segment.base_;
  // base1 + dir1 * scale1 = base2 + dir2 * scale2;
  // if segments intersect,
  // then 0 <= scale1 <= 1 and 0 <= scale2 <= 1

  double scale1 = (double)((base2 - base1) ^ dir2) / (double)(dir1 ^ dir2);
  double scale2 = (double)((base1 - base2) ^ dir1) / (double)(dir2 ^ dir1);

  return 0 <= scale1 && scale1 <= 1 && 0 <= scale2 && scale2 <= 1;
}

Segment* Segment::Clone() const { return new Segment(GetA(), GetB()); }

// get start of segment
Point Segment::GetA() const { return base_; }

// get end of segment
Point Segment::GetB() const { return base_ + direction_vector_; }

Line::Line() {}
Line::Line(const Point& start, const Point& end) {
  base_ = Vector(start.GetX(), start.GetY());
  direction_vector_ = end - start;
}
Line::~Line() {}

bool Line::ContainsPoint(const Point& point) const {
  Vector vector = point - base_;
  return (vector ^ direction_vector_) == 0;
}

bool Line::CrossesSegment(const Segment& segment) const {
  Point point_a = segment.GetA();
  Point point_b = segment.GetB();
  if (ContainsPoint(point_a) || ContainsPoint(point_b)) {
    return true;
  }

  const Vector& start = base_;
  const Vector& end = base_ + direction_vector_;

  int64_t cross_a = (end - point_a) ^ (start - point_a);
  int64_t cross_b = (end - point_b) ^ (start - point_b);

  // if orientations are different,
  // then they have opposite signs
  return cross_a * cross_b < 0;
}

Line* Line::Clone() const { return new Line(base_, base_ + direction_vector_); }

int64_t Line::GetA() const {
  Point end = base_ + direction_vector_;
  return base_.GetY() - end.GetY();
}

int64_t Line::GetB() const {
  Point end = base_ + direction_vector_;
  return end.GetX() - base_.GetX();
}

int64_t Line::GetC() const {
  Point end = base_ + direction_vector_;
  return base_.GetX() * end.GetY() - end.GetX() * base_.GetY();
}

Ray::Ray() {}
Ray::Ray(const Point& start, const Point& end) {
  base_ = Vector(start.GetX(), start.GetY());
  direction_vector_ = end - start;
}
Ray::~Ray() {}

bool Ray::ContainsPoint(const Point& point) const {
  Vector vector = point - base_;
  return (vector ^ direction_vector_) == 0 && (vector * direction_vector_) >= 0;
}

bool Ray::CrossesSegment(const Segment& segment) const {
  const Vector& dir1 = direction_vector_;
  const Vector& dir2 = segment.GetB() - segment.GetA();

  if ((dir1 ^ dir2) == 0) {
    // segments are collinear;
    return ContainsPoint(segment.GetA()) || ContainsPoint(segment.GetB());
  }

  const Vector& base1 = base_;
  const Vector& base2 = segment.GetA() - Point(0, 0);
  // base1 + dir1 * scale1 = base2 + dir2 * scale2;
  // if ray can be continued to intersect the segment,
  // then 0 <= scale1 and 0 <= scale2 <= 1

  double scale1 = (double)((base2 - base1) ^ dir2) / (double)(dir1 ^ dir2);
  double scale2 = (double)((base1 - base2) ^ dir1) / (double)(dir2 ^ dir1);

  return 0 <= scale1 && 0 <= scale2 && scale2 <= 1;
}

Ray* Ray::Clone() const { return new Ray(base_, base_ + direction_vector_); }

Point Ray::GetA() const { return base_; }

Vector Ray::GetDirection() const { return direction_vector_; }

Circle::Circle() {}
Circle::Circle(const Point& center, int64_t radius) : radius_(radius) {
  base_ = Vector(center.GetX(), center.GetY());
}
Circle::~Circle() {}

static int64_t DistSquared(const Point& start, const Point& end) {
  Vector vector = end - start;
  return vector.GetX() * vector.GetX() + vector.GetY() * vector.GetY();
}

bool Circle::ContainsPoint(const Point& point) const {
  int64_t dist_squared = DistSquared(GetCentre(), point);
  return dist_squared <= radius_ * radius_;
}

static bool IsOnCircle(const Circle& circle, const Point& point) {
  int64_t radius = circle.GetRadius();
  return DistSquared(circle.GetCentre(), point) == radius * radius;
}

static bool IsInsideCircle(const Circle& circle, const Point& point) {
  int64_t radius = circle.GetRadius();
  return DistSquared(circle.GetCentre(), point) < radius * radius;
}

// Circle crosses segment if
// the segment touches the circle
// or distance from center to segment is less than radius
// let O be the center of the square
// then H * AB = 2S,
// where H is distance from O to the segment AB
// and S is the area of triangle AOB
// if we square the expression, we will get:
// H^2 * AB^2 = 4S^2  ==>  H^2 = 4S^2 / AB^2
// we need H^2 <= R^2 or:
// (2S)^2 / AB^2 < R^2
// (2S)^2 <= R^2 * AB^2
// if the distance is less or equal to radius,
// then we check whether AB crosses the line
// that is perpendicular to AB and contains the center of the circle
bool Circle::CrossesSegment(const Segment& segment) const {
  Point point_a = segment.GetA();
  Point point_b = segment.GetB();
  Point centre = GetCentre();
  int64_t r_sqr = GetRadius() * GetRadius();
  if (IsOnCircle(*this, point_a) || IsOnCircle(*this, point_b)) {
    return true;
  }
  bool is_a_inside = IsInsideCircle(*this, point_a);
  bool is_b_inside = IsInsideCircle(*this, point_b);
  if (is_a_inside && is_b_inside) {
    return false;
  }
  if (is_a_inside != is_b_inside) {
    return true;
  }

  Vector vector_a = point_a - centre;
  Vector vector_b = point_b - centre;
  int64_t double_area_squared = (vector_a ^ vector_b) * (vector_a ^ vector_b);
  int64_t ab_sqr = DistSquared(point_a, point_b);
  if (double_area_squared > r_sqr * ab_sqr) {
    return false;
  }

  Line line_ab(point_a, point_b);
  Vector ab_normal_vector(line_ab.GetA(), line_ab.GetB());
  Line normal_line(base_, base_ + ab_normal_vector);
  return normal_line.CrossesSegment(segment);
}

Circle* Circle::Clone() const { return new Circle(base_, radius_); }

Point Circle::GetCentre() const { return base_; }

int64_t Circle::GetRadius() const { return radius_; }
