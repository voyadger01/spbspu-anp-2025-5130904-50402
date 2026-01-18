#include "storageShape.hpp"
#include <stdexcept>
karpovich::storageShape::storageShape() noexcept:
  shapes_ (nullptr),
  size_(0),
  cap_(0)
{}
karpovich::storageShape::storageShape(const storageShape& other):
  shapes_(nullptr),
  size_(other.size_),
  cap_(other.cap_)
{
  if (cap_ > 0) {
    shapes_ = new Shape*[cap_];
    for (size_t i = 0; i < size_; ++i) {
      shapes_[i] = other.shapes_[i]->clone();
    }
  }
}
karpovich::storageShape& karpovich::storageShape::operator=(const storageShape& other)
{
  if (this != &other) {
    clear();
    size_ = other.size_;
    cap_ = other.cap_;
    if (cap_ > 0) {
      shapes_ = new Shape*[cap_];
      for (size_t i = 0; i < size_; ++i) {
        shapes_[i] = other.shapes_[i]->clone();
      }
    } else {
      shapes_ = nullptr;
    }
  }
  return *this;
}
karpovich::storageShape::storageShape(storageShape&& other) noexcept:
  shapes_(other.shapes_),
  size_(other.size_),
  cap_(other.cap_)
{
  other.shapes_ = nullptr;
  other.size_ = 0;
  other.cap_ = 0;
}
karpovich::storageShape& karpovich::storageShape::operator=(storageShape&& other) noexcept
{
  if (this != &other) {
    clear();
    shapes_ = other.shapes_;
    size_ = other.size_;
    cap_ = other.cap_;
    other.shapes_ = nullptr;
    other.size_ = 0;
    other.cap_ = 0;
  }
  return *this;
}
karpovich::Shape* karpovich::storageShape::clone() const
{
  throw std::logic_error("ts is not clonable");
}
void karpovich::storageShape::doScale(double k) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    shapes_[i]->scale(k);
  }
}
double karpovich::storageShape::getArea() const noexcept
{
  double total = 0;
  for (size_t i = 0; i < size_; ++i) {
    total += shapes_[i]->getArea();
  }
  return total;
}
karpovich::rectangle_t karpovich::storageShape::getFrameRect() const noexcept
{
  return karpovich::computeTotalFrame(shapes_, size_);
}
void karpovich::storageShape::move(point_t p) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    shapes_[i]->move(p);
  }
}
void karpovich::storageShape::move(double dx, double dy) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    shapes_[i]->move(dx, dy);
  }
}

void karpovich::storageShape::append(Shape* app)
{
  add(app, size_);
}
void karpovich::storageShape::preappend(Shape* app)
{
  add(app, 0);
}
void karpovich::storageShape::add(Shape* app, size_t idx)
{
  if (cap_ == size_) {
    cap_ = (cap_ == 0) ? 10 : cap_ * 2;
    reserve(cap_);
  }
  if (!empty()) {
    for (size_t i = size_; i > idx; --i) {
      shapes_[i] = shapes_[i - 1];
    }
    shapes_[idx] = app;
    ++size_;
  }
}
karpovich::Shape& karpovich::storageShape::first() const noexcept
{
  return *shapes_[0];
}
karpovich::Shape& karpovich::storageShape::last() const noexcept
{
  return *shapes_[size_ - 1];
}
const karpovich::Shape& karpovich::storageShape::firstConst() const noexcept
{
  return *shapes_[0];
}
const karpovich::Shape& karpovich::storageShape::lastConst() const noexcept
{
  return *shapes_[size_ - 1];
}
karpovich::Shape& karpovich::storageShape::at(size_t idx) const
{
  if (idx >= size_) {
    throw std::invalid_argument("out of bounds");
  }
  return *shapes_[idx];
}
karpovich::Shape& karpovich::storageShape::get(size_t idx) const noexcept
{
  return *shapes_[idx];
}
const karpovich::Shape& karpovich::storageShape::atConst(size_t idx) const
{
  if (idx >= size_) {
    throw std::invalid_argument("out of bounds");
  }
  return *shapes_[idx];
}
const karpovich::Shape& karpovich::storageShape::getConst(size_t idx) const noexcept
{
  return *shapes_[idx];
}
void karpovich::storageShape::remove(size_t k)
{
  if (k >= size_) {
    throw std::out_of_range("out of bounds");
  }
  for (size_t i = k; i < size_ - 1; ++i) {
    shapes_[i] = shapes_[i + 1];
  }
  --size_;
}
void karpovich::storageShape::dropFirst()
{
  remove(0);
}
void karpovich::storageShape::dropLast()
{
  remove(size_ - 1);
}
void karpovich::storageShape::clear() noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    delete shapes_[i];
  }
  delete[] shapes_;
  size_ = 0;
}
size_t karpovich::storageShape::size() noexcept
{
  return size_;
}
bool karpovich::storageShape::empty() noexcept
{
  return !size_;
}
karpovich::storageShape::~storageShape()
{
  for (size_t i = 0; i < size_; ++i) {
    delete shapes_[i];
  }
  delete[] shapes_;
}
void karpovich::storageShape::reserve(size_t k)
{
  if (k < size_) {
    throw std::invalid_argument("too small new capacity");
  }
  Shape** temp = new Shape*[k];
  for (size_t i = 0; i < size_; ++i) {
    temp[i] = shapes_[i];
  }
  delete[] shapes_;
  shapes_ = temp;
  cap_ = k;
}
void karpovich::storageShape::shrink()
{
  reserve(size_);
}
size_t karpovich::storageShape::capacity() const noexcept
{
  return cap_;
}
karpovich::Shape** karpovich::storageShape::returnShps()
{
  return shapes_;
}
