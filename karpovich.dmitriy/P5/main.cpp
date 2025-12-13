#include <iostream>
#include <algorithm>
#include <cmath>

namespace karpovich
{
  const double PI = acos(-1.0);
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
    virtual ~Shape() = default;
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(point_t p) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double k) = 0;
  };
  struct Rectangle: Shape
  {
    Rectangle(double width, double height, point_t centr);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(double dx, double dy) override;
    void move(point_t p) override;
    void scale(double k) override;
    private:
      double width_, height_;
      point_t centr_;
  };
  struct Rubber: Shape
  {
    Rubber(double radius1, double radius2, point_t centr1, point_t centr2);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(double dx, double dy) override;
    void move(point_t p) override;
    void scale(double k) override;
    private:
      double radius1_, radius2_;
      point_t centr1_, centr2_;
  };
  struct Ellipse: Shape
  {
    Ellipse(double semiax1, double semiax2, point_t centr);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(double dx, double dy) override;
    void move(point_t p) override;
    void scale(double k) override;
    private:
      double semiax1_, semiax2_;
      point_t centr_;
  };
  void scalefrompt(Shape* shapes[], size_t size, double k, point_t pt);
  rectangle_t computeTotalFrame(Shape* shapes[], size_t size);
  void output(Shape* shapes[], size_t size);
}

int main()
{
  namespace karp = karpovich;
  karp::Rectangle rect(4.0, 2.0, {1.0, 1.0});
  karp::Ellipse ell(3.0, 2.0, {-2.0, 0.0});
  karp::Rubber rub(5.0, 2.0, {0.0, 0.0}, {1.0, 1.0});
  const size_t n = 3;
  karp::Shape* shapes[n] = {&rect, &ell, &rub};
  karp::output(shapes, n);

  karp::point_t pt;
  double k;
  if (!(std::cin >> pt.x >> pt.y >> k)) {
    std::cerr << "err: bad arguments\n";
    return 1;
  }
  if (k <= 0.0) {
    std::cerr << "err: k must be positive\n";
    return 1;
  }

  karp::scalefrompt(shapes, n, k, pt);
  karp::output(shapes, n);
  return 0;
}

karpovich::Rectangle::Rectangle(double width, double height, point_t centr):
  karpovich::Shape(),
  width_(width),
  height_(height),
  centr_(centr)
{}
double karpovich::Rectangle::getArea() const
{
  return height_ * width_;
}
karpovich::rectangle_t karpovich::Rectangle::getFrameRect() const
{
  rectangle_t frame;
  frame.pos = centr_;
  frame.height = height_;
  frame.width = width_;
  return frame;
}
void karpovich::Rectangle::move(point_t p)
{
  centr_ = p;
}
void karpovich::Rectangle::move(double dx, double dy)
{
  centr_.x += dx;
  centr_.y += dy;
}
void karpovich::Rectangle::scale(double k)
{
  width_ *= k;
  height_ *= k;
}
karpovich::Ellipse::Ellipse(double semiax1, double semiax2, point_t centr):
  karpovich::Shape(),
  semiax1_(semiax1),
  semiax2_(semiax2),
  centr_(centr)
{}
double karpovich::Ellipse::getArea() const
{
  return PI * semiax1_ * semiax2_;
}
karpovich::rectangle_t karpovich::Ellipse::getFrameRect() const
{
  rectangle_t frame;
  frame.pos = centr_;
  frame.height = 2 * semiax2_;
  frame.width = 2 * semiax1_;
  return frame;
}
void karpovich::Ellipse::move(point_t p)
{
  centr_ = p;
}
void karpovich::Ellipse::move(double dx, double dy)
{
  centr_.x += dx;
  centr_.y += dy;
}
void karpovich::Ellipse::scale(double k)
{
  semiax1_ *= k;
  semiax2_ *= k;
}
karpovich::Rubber::Rubber(double radius1, double radius2, point_t centr1, point_t centr2):
  karpovich::Shape(),
  radius1_(radius1),
  radius2_(radius2),
  centr1_(centr1),
  centr2_(centr2)
{}
double karpovich::Rubber::getArea() const
{
  return PI * (radius1_ * radius1_ - radius2_ * radius2_);
}
karpovich::rectangle_t karpovich::Rubber::getFrameRect() const
{
  double left = std::min(centr1_.x - radius1_, centr2_.x - radius2_);
  double right = std::max(centr1_.x + radius1_, centr2_.x + radius2_);
  double bottom = std::min(centr1_.y - radius1_, centr2_.y - radius2_);
  double top = std::max(centr1_.y + radius1_, centr2_.y + radius2_);

  rectangle_t frame;
  frame.width = right - left;
  frame.height = top - bottom;
  frame.pos.x = (left + right) / 2.0;
  frame.pos.y = (bottom + top) / 2.0;
  return frame;
}
void karpovich::Rubber::move(point_t p)
{
  double dx = p.x - centr2_.x;
  double dy = p.y - centr2_.y;
  centr1_.x += dx;
  centr1_.y += dy;
  centr2_ = p;
}
void karpovich::Rubber::move(double dx, double dy)
{
  centr1_.x += dx;
  centr1_.y += dy;
  centr2_.x += dx;
  centr2_.y += dy;
}
void karpovich::Rubber::scale(double k)
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
karpovich::rectangle_t karpovich::computeTotalFrame(karpovich::Shape* shapes[], size_t size)
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
    if (l < left) {
      left = l;
    }
    if (r > right) {
      right = r;
    }
    if (b < bottom) {
      bottom = b;
    }
    if (t > top) {
      top = t;
    }
  }
  karpovich::rectangle_t total;
  total.width = right - left;
  total.height = top - bottom;
  total.pos.x = (left + right) / 2.0;
  total.pos.y = (bottom + top) / 2.0;
  return total;
}
void karpovich::output(Shape* shapes[], size_t size)
{
  double total_area = 0.0;
  for (size_t i = 0; i < size; ++i) {
    double area = shapes[i]->getArea();
    total_area += area;
    std::cout << area << "\n";
    rectangle_t frame = shapes[i]->getFrameRect();
    std::cout << frame.pos.x << "\n";
    std::cout << frame.pos.y << "\n";
    std::cout << frame.width << "\n";
    std::cout << frame.height << "\n";
  }
  rectangle_t total_frame = computeTotalFrame(shapes, size);
  std::cout << total_area << "\n";
  std::cout << total_frame.pos.x << "\n";
  std::cout << total_frame.pos.y << "\n";
  std::cout << total_frame.width << "\n";
  std::cout << total_frame.height << "\n";
}
