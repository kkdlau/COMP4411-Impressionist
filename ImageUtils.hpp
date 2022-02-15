#if !defined(__IMAGE_UTILS__)
#define __IMAGE_UTILS__
#include "Image.hpp"
#include <tuple>
#include <vector>

using namespace std;

namespace ImageUtils {
static const short sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

static const short sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

static tuple<float, float, float> sobel(Image &img, int y, int x) {
  float gx = 0;
  float gy = 0;
  y--;
  x--;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (img.valid_point(y + i, x + j)) {
        auto color = img(y + i, x + j);
        const float grey = 0.299F * get<0>(color) + 0.587F * get<1>(color) +
                           0.114F * get<2>(color);
        gx += sobel_x[i][j] * grey;
        gy += sobel_y[i][j] * grey;
      }
    }
  }
  return {gx, gy, atan2(gy, gx)};
}

static Image dissolve(Image &source, Image &target) {
  Image output = target;

  output.for_each_pixel([&](int y, int x) {
    auto color = output(y, x);
    if (source.valid_point(y, x)) {
      RGBA avg_color = (source(y, x) + color) / 2;
      get<0>(color) = get<0>(avg_color);
      get<1>(color) = get<1>(avg_color);
      get<2>(color) = get<2>(avg_color);
    }
  });

  return output;
}
} // namespace ImageUtils

#endif // __IMAGE_UTILS__
