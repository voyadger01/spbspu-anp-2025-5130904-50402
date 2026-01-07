#include "storageShape.hpp"
#include <stdexcept>
karpovich::storageShape::storageShape():
  shapes_ (nullptr),
  size_(0)
{}
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
karpovich::Shape& karpovich::storageShape::first() const
{
  return *shapes_[0];
}
karpovich::Shape& karpovich::storageShape::last() const
{
  return *shapes_[size_ - 1];
}
const karpovich::Shape& karpovich::storageShape::firstConst() const
{
  return *shapes_[0];
}
const karpovich::Shape& karpovich::storageShape::lastConst() const
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
karpovich::Shape& karpovich::storageShape::get(size_t idx) const
{
  return *shapes_[idx];
}
const karpovich::Shape& karpovich::storageShape::atConst(size_t idx) const
{
  return at(idx);
}
const karpovich::Shape& karpovich::storageShape::getConst(size_t idx) const
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
void karpovich::storageShape::clear()
{
  for (size_t i = 0; i < size_; ++i) {
    delete shapes_[i];
  }
  delete[] shapes_;
  size_ = 0;
}
size_t karpovich::storageShape::size()
{
  return size_;
}
bool karpovich::storageShape::empty()
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
size_t karpovich::storageShape::capacity() const
{
  return cap_;
}
