#ifndef STORAGE_SHAPE_HPP
#define STORAGE_SHAPE_HPP
#include <podshape.hpp>
namespace karpovich
{
  struct storageShape final: Shape
  {
    storageShape();
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t p) noexcept override;
    void move(double dx, double dy) noexcept override;

    void append(Shape* app);
    void preappend(Shape* app);
    void add(Shape* app, size_t idx);
    Shape& first() const;
    Shape& last() const;
    const Shape& firstConst() const;
    const Shape& lastConst() const;
    Shape& at(size_t idx) const;
    Shape& get(size_t idx) const;
    const Shape& atConst(size_t idx) const;
    const Shape& getConst(size_t idx) const;
    void remove(size_t k);
    void dropFirst();
    void dropLast();
    void clear();
    size_t size();
    bool empty();
    ~storageShape();
    size_t capacity() const;
    void shrink();
    void reserve(size_t k);

    private:
      void doScale(double k) noexcept override;
      Shape** shapes_;
      size_t size_;
      size_t cap_;
  };
}
#endif