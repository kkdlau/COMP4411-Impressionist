#if !defined(__IMAGE_H_)
#define __IMAGE_H_

#include "ImpBrush.h"
#include "gl_helper.h"
#include <functional>
#include <tuple>
#include <vector>

using namespace std;

typedef tuple<GLubyte, GLubyte, GLubyte> RGB888;

class Image {
  vector<GLubyte> img;

public:
  int width;
  int height;
  Image(GLubyte *buf, int w, int h) { set(buf, w, h); }

  void set(GLubyte *buf, int w, int h) {
    width = w, height = h;
    img = {};
    const int length = w * h * 3;
    for (int i = 0; i < length; i++) {
      img.push_back(buf[i]);
    }
  }

  bool valid_point(int x, int y) const {
    if (x < 0)
      return false;
    if (y < 0)
      return false;
    if (x >= width)
      return false;
    if (y >= height)
      return false;
    return true;
  }

  const tuple<GLubyte &, GLubyte &, GLubyte &> operator()(int y, int x) {
    int i = 3 * (y * width + x);
    return {img[i], img[i + 1], img[i + 2]};
  }

  GLubyte *raw_fmt() { return img.data(); }

  void set_pixel(int y, int x, const RGB888 &rgb) {
    auto color = (*this)(y, x);
    get<0>(color) = get<0>(rgb);
    get<1>(color) = get<1>(rgb);
    get<2>(color) = get<2>(rgb);
  }

  void set_pixel(const Point &p, const RGB888 &rgb) {
    set_pixel(p.y, p.x, rgb);
  }

  void for_range_pixel(const Point &s, const Point &e,
                       function<void(int, int)> handler) {
    for (int y = s.y; y <= e.y; y++) {
      for (int x = s.x; x <= e.x; x++) {
        handler(y, x);
      }
    }
  }
};
#endif // __IMAGE_H_
