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
      if (img.valid_point(x + j, y + i)) {
        auto colors = img(x + j, y + i);
        const float grey =
            0.299F * colors[0] + 0.587F * colors[1] + 0.114F * colors[2];
        gx += sobel_x[i][j] * grey;
        gy += sobel_y[i][j] * grey;
      }
    }
  }
  return {gx, gy, atan2(gy, gx)};
}
} // namespace ImageUtils

#endif // __IMAGE_UTILS__
