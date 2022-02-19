#if !defined(__IMAGE_UTILS__)
#define __IMAGE_UTILS__
#include "Image.hpp"
#include <tuple>
#include <vector>

using namespace std;

namespace ImageUtils {
static const short sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

static const short sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

static Image median_filter(Image &img) {
  Image filtered = img;
  return filtered;
}

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

static Image generate_edge_image(Image &img) {
  Image return_img = img;
  return_img.for_each_pixel([&](int y, int x) {
    auto result = sobel(img, y, x);
    float gx = get<0>(result);
    float gy = get<1>(result);
    float gradient2 = gx * gx + gy * gy;

    auto color = return_img(y, x);

    if (gradient2 > 127 * 127) {
      // white
      get<0>(color) = 255;
      get<1>(color) = 255;
      get<2>(color) = 255;
      get<3>(color) = 255;
    } else {
      // black
      get<0>(color) = 0;
      get<1>(color) = 0;
      get<2>(color) = 0;
      get<3>(color) = 255;
    }
  });

  return return_img;
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
      get<3>(color) = get<3>(avg_color);
    }
  });

  return output;
}

static Point check_if_hit_edge(const Point &from, const Point &to,
                               Image &edge_img) {
  Point prev = to;
  if (from.x == to.x) {
    for (int y = from.y; y <= to.y; y++) {
      if (edge_img.has_color(y, from.x)) {
        // has color on current pixel
        prev.y = y - 1;
        break;
      }
    }
  } else if (from.y == to.y) {
    for (int x = from.x; x <= to.x; x++) {
      if (edge_img.has_color(from.y, x)) {
        // has color on current pixel
        prev.x = x - 1;
        break;
      }
    }
  } else {
    const float m = to / from;
    const float intercept = from.y - m * from.x;
    for (int x = from.x; x <= to.x; x++) {
      int y = (int)(x * m + intercept);
      if (edge_img.has_color(y, x)) {
        // has color on current pixel
        prev.x = x - 1;
        prev.y = (int)(prev.x * m + intercept);

        break;
      }
    }
  }

  return prev;
}
} // namespace ImageUtils

#endif // __IMAGE_UTILS__
