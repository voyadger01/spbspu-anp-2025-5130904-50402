#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace karpovich
{
  const double PI = std::acos(-1.0);
  struct point_t
  {
    double x, y;
  };
  struct rectangle_t
  {
    double width, height;
    point_t pos;
  };
  struct Shape
  {
    virtual ~Shape() noexcept = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(point_t p) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    void scale(double k);
    void uncheckedScale(double k) noexcept;
    protected:
      virtual void doScale(double k) noexcept = 0;
  };
  struct Rectangle final: Shape
  {
    Rectangle(double width, double height, point_t centr) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(point_t p) noexcept override;
    private:
      double width_, height_;
      point_t centr_;
      void doScale(double k) noexcept override;
  };
  struct Rubber final: Shape
  {
    Rubber(double radius1, double radius2, point_t centr1, point_t centr2) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(point_t p) noexcept override;
    private:
      double radius1_, radius2_;
      point_t centr1_, centr2_;
      void doScale(double k) noexcept override;
  };
  struct Ellipse final: Shape
  {
    Ellipse(double semiax1, double semiax2, point_t centr) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(point_t p) noexcept override;
    private:
      double semiax1_, semiax2_;
      point_t centr_;
      void doScale(double k) noexcept override;
  };
  void scalefrompt(Shape* shapes[], size_t size, double k, point_t pt);
  rectangle_t computeTotalFrame(const Shape* const shapes[], size_t size);
  void output(const Shape* const shapes[], size_t size);
  void outputRectangle(const rectangle_t& rect);
}

int main()
{
  namespace karp = karpovich;
  karp::Rectangle rect{4.0, 2.0, {1.0, 1.0}};
  karp::Ellipse ell{3.0, 2.0, {-2.0, 0.0}};
  karp::Rubber rub{5.0, 2.0, {0.0, 0.0}, {1.0, 1.0}};
  const size_t n = 3;
  karp::Shape* shapes[n] = {std::addressof(rect), std::addressof(ell), std::addressof(rub)};
  karp::output(shapes, n);

  karp::point_t pt;
  double k = 0;
  if (!(std::cin >> pt.x >> pt.y >> k)) {
    std::cerr << "err: failed to read input\n";
    return 1;
  }

  try {
    scalefrompt(shapes, n, k, pt);
  } catch (const std::invalid_argument& e) {
    std::cerr << e.what();
    return 1;
  }

  karp::output(shapes, n);
  return 0;
}

void karpovich::Shape::scale(double k)
{
  if (k <= 0.0) {
    throw std::invalid_argument("err: k must be positive\n");
  }
  doScale(k);
}
void karpovich::Shape::uncheckedScale(double k) noexcept
{
  doScale(k);
}
karpovich::Rectangle::Rectangle(double width, double height, point_t centr) noexcept:
  width_(width),
  height_(height),
  centr_(centr)
{}
double karpovich::Rectangle::getArea() const noexcept
{
  return height_ * width_;
}
karpovich::rectangle_t karpovich::Rectangle::getFrameRect() const noexcept
{
  return {width_, height_, centr_};
}
void karpovich::Rectangle::move(point_t p) noexcept
{
  centr_ = p;
}
void karpovich::Rectangle::move(double dx, double dy) noexcept
{
  centr_.x += dx;
  centr_.y += dy;
}
void karpovich::Rectangle::doScale(double k) noexcept
{
  width_ *= k;
  height_ *= k;
}
karpovich::Ellipse::Ellipse(double semiax1, double semiax2, point_t centr) noexcept:
  semiax1_(semiax1),
  semiax2_(semiax2),
  centr_(centr)
{}
double karpovich::Ellipse::getArea() const noexcept
{
  return PI * semiax1_ * semiax2_;
}
karpovich::rectangle_t karpovich::Ellipse::getFrameRect() const noexcept
{
  return {2 * semiax1_, 2 * semiax2_, centr_};
}
void karpovich::Ellipse::move(point_t p) noexcept
{
  centr_ = p;
}
void karpovich::Ellipse::move(double dx, double dy) noexcept
{
  centr_.x += dx;
  centr_.y += dy;
}
void karpovich::Ellipse::doScale(double k) noexcept
{
  semiax1_ *= k;
  semiax2_ *= k;
}
karpovich::Rubber::Rubber(double radius1, double radius2, point_t centr1, point_t centr2) noexcept:
  radius1_(radius1),
  radius2_(radius2),
  centr1_(centr1),
  centr2_(centr2)
{}
double karpovich::Rubber::getArea() const noexcept
{
  return PI * (radius1_ * radius1_ - radius2_ * radius2_);
}
karpovich::rectangle_t karpovich::Rubber::getFrameRect() const noexcept
{
  double left = std::min(centr1_.x - radius1_, centr2_.x - radius2_);
  double right = std::max(centr1_.x + radius1_, centr2_.x + radius2_);
  double bottom = std::min(centr1_.y - radius1_, centr2_.y - radius2_);
  double top = std::max(centr1_.y + radius1_, centr2_.y + radius2_);

  return {{right - left}, {top - bottom}, {(left + right) / 2.0, (bottom + top) / 2.0}};
}
void karpovich::Rubber::move(point_t p) noexcept
{
  double dx = p.x - centr2_.x;
  double dy = p.y - centr2_.y;
  centr1_.x += dx;
  centr1_.y += dy;
  centr2_ = p;
}
void karpovich::Rubber::move(double dx, double dy) noexcept
{
  centr1_.x += dx;
  centr1_.y += dy;
  centr2_.x += dx;
  centr2_.y += dy;
}
void karpovich::Rubber::doScale(double k) noexcept
{
  radius1_ *= k;
  radius2_ *= k;
}
void karpovich::scalefrompt(Shape* shapes[], size_t size, double k, point_t pt)
{
  for (size_t i = 0; i < size; ++i) {
    shapes[i]->move(-pt.x, -pt.y);
    shapes[i]->scale(k);
    shapes[i]->move(pt.x, pt.y);
  }
}
karpovich::rectangle_t karpovich::computeTotalFrame(const Shape* const shapes[], size_t size)
{
  if (size == 0) {
    return {0.0, 0.0, {0.0, 0.0}};
  }
  karpovich::rectangle_t first = shapes[0]->getFrameRect();
  double left = first.pos.x - first.width / 2.0;
  double right = first.pos.x + first.width / 2.0;
  double bottom = first.pos.y - first.height / 2.0;
  double top = first.pos.y + first.height / 2.0;
  for (size_t i = 1; i < size; ++i) {
    karpovich::rectangle_t frame = shapes[i]->getFrameRect();
    double l = frame.pos.x - frame.width / 2.0;
    double r = frame.pos.x + frame.width / 2.0;
    double b = frame.pos.y - frame.height / 2.0;
    double t = frame.pos.y + frame.height / 2.0;
    left = std::min(l, left);
    right = std::max(r, right);
    bottom = std::min(b, bottom);
    top = std::max(t, top);
  }
  return {{right - left}, {top - bottom}, {(left + right) / 2.0, (bottom + top) / 2.0}};
}
void karpovich::output(const Shape* const shapes[], size_t size)
{
  double total_area = 0.0;
  for (size_t i = 0; i < size; ++i) {
    double area = shapes[i]->getArea();
    total_area += area;
    std::cout << area << "\n";
    outputRectangle(shapes[i]->getFrameRect());
  }
  rectangle_t total_frame = computeTotalFrame(shapes, size);
  std::cout << total_area << "\n";
  outputRectangle(total_frame);
}

void karpovich::outputRectangle(const rectangle_t& rect)
{
  std::cout << rect.pos.x << "\n";
  std::cout << rect.pos.y << "\n";
  std::cout << rect.width << "\n";
  std::cout << rect.height << "\n";
}
