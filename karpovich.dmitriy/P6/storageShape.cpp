#include "storageShape.hpp"
karpovich::storageShape::storageShape():
  shapes_ (nullptr),
  size_(0)
{}
double karpovich::storageShape::getArea() const noexcept
{
  size_t total = 0;
  for (size_t i = 0; i < size_; ++i) {
    total += shapes_[i]->getArea();
  }
}
karpovich::rectangle_t karpovich::storageShape::getFrameRect() const noexcept
{
  
}
void karpovich::storageShape::move(point_t p) noexcept
{

}
void karpovich::storageShape::move(double dx, double dy) noexcept
{

}

void karpovich::storageShape::append(const Shape* app)
{

}
void karpovich::storageShape::preappend(const Shape* app)
{

}
void karpovich::storageShape::add(const Shape* app, size_t idx)
{

}
karpovich::Shape& karpovich::storageShape::first() const
{

}
karpovich::Shape& karpovich::storageShape::last() const
{

}
const karpovich::Shape& karpovich::storageShape::firstConst() const
{

}
const karpovich::Shape& karpovich::storageShape::lastConst() const
{

}
karpovich::Shape& karpovich::storageShape::at(size_t idx) const
{

}
karpovich::Shape& karpovich::storageShape::get(size_t idx) const
{

}
const karpovich::Shape& karpovich::storageShape::atConst(size_t idx) const
{

}
const karpovich::Shape& karpovich::storageShape::getConst(size_t idx) const
{

}
void karpovich::storageShape::remove(size_t k)
{

}
void karpovich::storageShape::dropFirst()
{

}
void karpovich::storageShape::dropLast()
{

}
void karpovich::storageShape::clear()
{

}
size_t karpovich::storageShape::size()
{

}
bool karpovich::storageShape::empty()
{

}
karpovich::storageShape::~storageShape()
{

}
