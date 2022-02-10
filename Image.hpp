#if !defined(__IMAGE_H_)
#define __IMAGE_H_

#include "Bitmap.h"
#include "ImpBrush.h"
#include "gl_helper.h"
#include <functional>
#include <tuple>
#include <vector>

using namespace std;

typedef tuple<GLubyte, GLubyte, GLubyte> RGB888;

static RGB888 operator+(RGB888 c1, RGB888 c2) {
  return {get<0>(c1) + get<0>(c2), get<1>(c1) + get<1>(c2),
          get<2>(c1) + get<2>(c2)};
}

static RGB888 operator/(RGB888 c1, double d) {
  return {(int)(get<0>(c1) / d), (int)(get<1>(c1) / d), int(get<2>(c1) / d)};
}

static RGB888 operator*(RGB888 c1, double d) {
  return {(int)(get<0>(c1) * d), (int)(get<1>(c1) * d), int(get<2>(c1) * d)};
}

class Image {
  vector<GLubyte> img;

public:
  int width;
  int height;
  Image() : Image{nullptr, 0, 0} {}
  Image(GLubyte *buf, int w, int h) { set(buf, w, h); }

  static Image from(char *path) {
    unsigned char *data;
    int width, height;

    data = readBMP(path, width, height);
    return Image(data, width, height);
  }

  void set(GLubyte *buf, int w, int h) {
    width = w, height = h;
    img = {};
    const int length = w * h * 3;
    for (int i = 0; i < length; i++) {
      img.push_back(buf[i]);
    }
  }

  bool valid_point(int y, int x) const {
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
  GLubyte *paint_byte() { return img.data() + height * 3; }

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

  void for_each_pixel(function<void(int, int)> handler) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        handler(y, x);
      }
    }
  }
};
#endif // __IMAGE_H_
