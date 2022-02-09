#if !defined(__IMAGE_UTILS__)
#define __IMAGE_UTILS__
#include <tuple>
#include <vector>

using namespace std;

class ImageUtils {
  static const short sobel_x[3][3];

  static const short sobel_y[3][3];
  static ImageUtils *instance;

  ImageUtils() {}

public:
  static ImageUtils &I() {
    if (instance == nullptr)
      instance = new ImageUtils();

    return *instance;
  }
  tuple<int, int, float> sobel(const vector<vector<GLubyte *>> &colors) {
    int gx = .0f;
    int gy = .0f;

    for (int y = 0; y < 3; y++) {
      for (int x = 0; x < 3; x++) {
        GLubyte *color = colors[y][x];
        int grey = (int)(0.2126 * color[0] / 255.0 + 0.7152 * color[1] / 255.0 +
                         0.0722 * color[2] / 255.0);
        gx += grey * sobel_x[y][x];
        gy += grey * sobel_y[y][x];
      }
    }

    return {gx, gy, atan2(gy, gx)};
  }
  ~ImageUtils() {}
};

const short ImageUtils::sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

const short ImageUtils::sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

ImageUtils *ImageUtils::instance = nullptr;

#endif // __IMAGE_UTILS__
