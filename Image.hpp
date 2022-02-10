#if !defined(__IMAGE_H_)
#define __IMAGE_H_

#include "ImpBrush.h"
#include "gl_helper.h"
#include <tuple>
#include <vector>

using namespace std;

typedef tuple<GLubyte, GLubyte, GLubyte> RGB888;

class Image {
  vector<GLubyte> img;
  int width;
  int height;

public:
  Image(GLubyte *buf, int w, int h) { set(buf, w, h); }

  Image(const Image &another_img) : img{another_img.img} {
    this->width = another_img.width;
    this->height = another_img.height;
  }

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

  tuple<GLubyte &, GLubyte &, GLubyte &> operator()(int y, int x) {
    int i = 3 * (y * width + x);
    return tuple<GLubyte &, GLubyte &, GLubyte &>{img[i], img[i + 1],
                                                  img[i + 2]};
  }

  GLubyte *raw_fmt() { return img.data(); }

  void set_pixel(int y, int x, RGB888 rgb) {
    auto color = (*this)(y, x);
    debugger("r:%d g:%d b:%d", get<0>(color), get<1>(color), get<2>(color));
    get<0>(color) = 0;
    get<1>(color) = 0;
    get<2>(color) = 0;
  }
};

#endif // __IMAGE_H_
