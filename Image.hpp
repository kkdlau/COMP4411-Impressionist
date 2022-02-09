#if !defined(__IMAGE_H_)
#define __IMAGE_H_

#include "ImpBrush.h"
#include "gl_helper.h"
#include <vector>

using namespace std;

class Image {
  vector<GLubyte> img;
  int width;
  int height;

public:
  Image(GLubyte *buf, int w, int h) { set(buf, w, h); }

  void set(GLubyte *buf, int w, int h) {
    width = w, height = h;
    img = {};
    const int length = w * h * 3;
    for (int i = 0; i < length; i++) {
      img.push_back(buf[i]);
    }
  }

  bool valid_point(int x, int y) {
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

  vector<GLubyte> operator()(int x, int y) {
    int i = 3 * (y * width + x);
    return vector<GLubyte>{img[i], img[i + 1], img[i + 2]};
  }

  vector<GLubyte> operator[](int y) {
    return vector<GLubyte>{img.begin() + y * width * 3,
                           img.begin() + (y + 1) * width * 3};
  }

  GLubyte *raw_fmt() { return img.data(); }
};

#endif // __IMAGE_H_
