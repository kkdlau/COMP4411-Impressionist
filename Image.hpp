#if !defined(__IMAGE_H_)
#define __IMAGE_H_

#include "Bitmap.h"
#include "ImpBrush.h"
#include "gl_helper.h"
#include <cmath>
#include <functional>
#include <tuple>
#include <vector>

using namespace std;

typedef tuple<GLubyte, GLubyte, GLubyte, GLubyte> RGBA;

static RGBA operator+(RGBA c1, RGBA c2) {
  return {get<0>(c1) + get<0>(c2), get<1>(c1) + get<1>(c2),
          get<2>(c1) + get<2>(c2), get<3>(c1) + get<3>(c2)};
}

static RGBA operator/(RGBA c1, double d) {
  return {(int)(get<0>(c1) / d), (int)(get<1>(c1) / d), int(get<2>(c1) / d),
          int(get<3>(c1) / d)};
}

static RGBA operator*(RGBA c1, double d) {
  return {(int)(get<0>(c1) * d), (int)(get<1>(c1) * d), int(get<2>(c1) * d),
          int(get<3>(c1) * d)};
}

class Image {
  static const int NUM_CHANNEL = 4;

public:
  vector<GLubyte> bytes;

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

  void operator=(Image &img) {
    this->bytes = img.bytes;
    this->width = img.width;
    this->height = img.height;
  }

  void set(GLubyte *buf, int w, int h) {
    width = w, height = h;
    bytes = {};
    const int length = w * h * 3;
    short count = 0;
    for (int i = 0; i < length; i++) {
      bytes.push_back(buf[i]);
      if (++count == 3) {
        count = 0;
        bytes.push_back(255);
      }
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

  Point clip(const Point &p) { return clip(p.x, p.y); }

  Point clip(int x, int y) {
    if (x < 0)
      x = 0;
    if (y < 0)
      y = 0;
    if (x >= width)
      x = width - 1;
    if (y >= height)
      y = height - 1;
    return Point{x, y};
  }

  const tuple<GLubyte &, GLubyte &, GLubyte &, GLubyte &> operator()(int y,
                                                                     int x) {
    int i = convert_to_index(y, x);
    return {bytes[i], bytes[i + 1], bytes[i + 2], bytes[i + 3]};
  }

  int convert_to_index(int y, int x) { return NUM_CHANNEL * (y * width + x); }

  GLubyte *raw_fmt() { return bytes.data(); }
  GLubyte *paint_byte() { return bytes.data() + height * NUM_CHANNEL; }

  void set_pixel(int y, int x, const RGBA &rgb) {
    auto color = (*this)(y, x);
    get<0>(color) = get<0>(rgb);
    get<1>(color) = get<1>(rgb);
    get<2>(color) = get<2>(rgb);
  }

  void set_pixel(const Point &p, const RGBA &rgb) { set_pixel(p.y, p.x, rgb); }

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

  void set_alpha(float a) {
    for_each_pixel(
        [&](int y, int x) { get<3>((*this)(y, x)) = (GLubyte)(255 * a); });
  }

  void crop(size_t sx, size_t sy) {
    vector<GLubyte> buf{};
    for_each_pixel([&](int y, int x) {
      if (y >= sy || x >= sx)
        return;
      auto color = (*this)(y, x);
      buf.push_back(get<0>(color));
      buf.push_back(get<1>(color));
      buf.push_back(get<2>(color));
      buf.push_back(get<3>(color));
    });

    bytes = buf;
    width = width > sx ? sx : width;
    height = height > sy ? sy : height;
  }

  bool contain_content() const { return width != 0 && height != 0; }

  void clear() {
    this->bytes = {};
    this->width = this->height = 0;
  }
};
#endif // __IMAGE_H_
