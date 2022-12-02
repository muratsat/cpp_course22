#include "geometry.hpp"

#include <cstdint>

Point::Point() {}
Point::Point(const Vector& v) { base_ = v; }
Point::Point(int64_t x, int64_t y) { base_ = Vector(x, y); }
Point::~Point() {}

bool Point::ContainsPoint(const Point& point) const {
  return base_ == point.base_;
}

bool Point::CrossesSegment(const Segment& segment) const {
  const Point& p = *this;
  Vector va = segment.GetA() - p;
  Vector vb = segment.GetB() - p;
  return (va ^ vb) == 0 && (va * vb) <= 0;
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
  const Vector& d1 = direction_vector_;
  const Vector& d2 = segment.direction_vector_;

  if ((d1 ^ d2) == 0) {
    // segments are collinear;
    bool cross = ContainsPoint(segment.GetA()) ||
                 ContainsPoint(segment.GetB()) ||
                 segment.ContainsPoint(GetA()) || segment.ContainsPoint(GetB());
    return cross;
  }

  const Vector& b1 = base_;
  const Vector& b2 = segment.base_;
  // b1 + d1 * s1 = b2 + d2 * s2;
  // if segments intersect,
  // then 0 <= s1 <= 1 and 0 <= s2 <= 1

  double s1 = (double)((b2 - b1) ^ d2) / (double)(d1 ^ d2);
  double s2 = (double)((b1 - b2) ^ d1) / (double)(d2 ^ d1);

  return 0 <= s1 && s1 <= 1 && 0 <= s2 && s2 <= 1;
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
  Vector v = point - base_;
  return (v ^ direction_vector_) == 0;
}

bool Line::CrossesSegment(const Segment& segment) const {
  Point a = segment.GetA();
  Point b = segment.GetB();
  if (ContainsPoint(a) || ContainsPoint(b)) {
    return true;
  }

  const Vector& start = base_;
  const Vector& end = base_ + direction_vector_;

  int64_t cross_a = (end - a) ^ (start - a);
  int64_t cross_b = (end - b) ^ (start - b);

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
  Vector v = point - base_;
  return (v ^ direction_vector_) == 0 && (v * direction_vector_) >= 0;
}

bool Ray::CrossesSegment(const Segment& segment) const {
  const Vector& d1 = direction_vector_;
  const Vector& d2 = segment.GetB() - segment.GetA();

  if ((d1 ^ d2) == 0) {
    // segments are collinear;
    return ContainsPoint(segment.GetA()) || ContainsPoint(segment.GetB());
  }

  const Vector& b1 = base_;
  const Vector& b2 = segment.GetA() - Point(0, 0);
  // b1 + d1 * s1 = b2 + d2 * s2;
  // if ray can be continued to intersect the segment,
  // then 0 <= s1 and 0 <= s2 <= 1

  double s1 = (double)((b2 - b1) ^ d2) / (double)(d1 ^ d2);
  double s2 = (double)((b1 - b2) ^ d1) / (double)(d2 ^ d1);

  return 0 <= s1 && 0 <= s2 && s2 <= 1;
}

Ray* Ray::Clone() const { return new Ray(base_, base_ + direction_vector_); }

Point Ray::GetA() const { return base_; }

Vector Ray::GetDirection() const { return direction_vector_; }

Circle::Circle() {}
Circle::Circle(const Point& center, int64_t radius) : radius_(radius) {
  base_ = Vector(center.GetX(), center.GetY());
}
Circle::~Circle() {}

static int64_t DistSquared(const Point& p1, const Point& p2) {
  Vector v = p2 - p1;
  return v.GetX() * v.GetX() + v.GetY() * v.GetY();
}

bool Circle::ContainsPoint(const Point& point) const {
  int64_t dist_squared = DistSquared(GetCentre(), point);
  return dist_squared <= radius_ * radius_;
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
  Point a = segment.GetA();
  Point b = segment.GetB();
  Point o = GetCentre();
  int64_t oa_len = DistSquared(a, o);
  int64_t ob_len = DistSquared(b, o);
  int64_t r_sqr = GetRadius() * GetRadius();
  if (oa_len == r_sqr || ob_len == r_sqr) {
    return true;
  }
  bool is_a_inside = oa_len < r_sqr;
  bool is_b_inside = ob_len < r_sqr;
  if (is_a_inside && is_b_inside) {
    return false;
  }
  if (is_a_inside != is_b_inside) {
    return true;
  }
  Vector oa = a - o;
  Vector ob = b - o;
  int64_t double_area_squared = (oa ^ ob) * (oa ^ ob);
  int64_t ab_sqr = DistSquared(a, b);
  if (double_area_squared > r_sqr * ab_sqr) {
    return false;
  }
  Line line_ab(a, b);
  Vector ab_normal_vector(line_ab.GetA(), line_ab.GetB());
  Line normal_line(base_, base_ + ab_normal_vector);
  return normal_line.CrossesSegment(segment);
}

Circle* Circle::Clone() const { return new Circle(base_, radius_); }

Point Circle::GetCentre() const { return base_; }

int64_t Circle::GetRadius() const { return radius_; }
