#if !defined(__IMAGE_UTILS__)
#define __IMAGE_UTILS__
#include "Image.hpp"
#include <filesystem>
#include <functional>
#include <tuple>
#include <vector>
namespace fs = std::filesystem;

using namespace std;

namespace ImageUtils {
static const short sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

static const short sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

static Image median_filter(Image &img) {
  Image filtered = img;
  return filtered;
}

static void filterCustom(Image &img, const Point source,
                         std::vector<int> &color,
                         const std::vector<float> &filter_vals,
                         const int row_num, const int col_num,
                         const int divisor = 1) {

  short row_offset = row_num / 2, col_offset = col_num / 2;
  int r = 0, g = 0, b = 0;
  short filter_index = 0;
  for (int i = 0; i < row_num; ++i) {
    for (int j = 0; j < col_num; ++j) {
      float filter_val = filter_vals[filter_index++];
      int x = source.x + i - row_offset;
      int y = source.y + j - col_offset;
      if (x < 0)
        x = 0;
      else if (x >= img.width)
        x = img.width - 1;
      if (y < 0)
        y = 0;
      else if (y >= img.height)
        y = img.height - 1;
      auto pixel = img(y, x);
      r += (filter_val * get<0>(pixel));
      g += (filter_val * get<1>(pixel));
      b += (filter_val * get<2>(pixel));
    }
  }
  color[0] = r / divisor;
  color[1] = g / divisor;
  color[2] = b / divisor;
}

static void filterMean(Image &img, const Point source, std::vector<int>& color, const int filter_dim) {
    if (filter_dim < 1) {
        auto pixel = img(source.y, source.x);
        color[0] = get<0>(pixel);
        color[1] = get<1>(pixel);
        color[2] = get<2>(pixel);
        return;
    }
    short offset = (filter_dim - 1) / 2;

    int r = 0, g = 0, b = 0;
    for (int i = 0; i < filter_dim; i++) {
        for (int j = 0; j < filter_dim; j++) {
            int x = source.x - i + offset;
            int y = source.y - j + offset;
            if (x < 0) x = 0;
            else if (x >= img.width) x = img.width - 1;
            if (y < 0) y = 0;
            else if (y >= img.height) y = img.height - 1;
            auto pixel = img(y, x);
            r += get<0>(pixel);
            g += get<1>(pixel);
            b += get<2>(pixel);
        }
    }
    int total_pixels = filter_dim * filter_dim;
    color[0] = r / total_pixels;
    color[1] = g / total_pixels;
    color[2] = b / total_pixels;
}

static void applyBlurFilter(Image& original, Image& result, const int filter_dim) {
    for (int i = 1; i < result.width; ++i) {
        for (int j = 1; j < result.height; ++j) {
            //Point source(i, original.height - j);
            Point source(i, j);
            std::vector<int> color{ 0, 0, 0 };

            filterMean(original, source, color, filter_dim);
            RGBA final_color = make_tuple(color[0], color[1], color[2], 255);
            result.set_pixel(source, final_color);
        }
    }
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

static float
luma_cal(tuple<GLubyte &, GLubyte &, GLubyte &, GLubyte &> &color) {
  return 0.299 * get<0>(color) + 0.587 * get<1>(color) + 0.114 * get<2>(color);
}

/**
 * @brief structural similarity index measure method.
 * This function is designed for measuring the similarity between two images.
 *
 * @param src source image
 * @param target target image
 * @return float similarity ranging from 0 to 1.
 */
static float structural_similarity(Image &src, Image &target) {
  target.resize(src.width, src.height);
  float MSSIM = 0;

  const int win_width_size = src.width / 2;
  const int win_height_size = src.height / 2;
  const int num_win_pixel = win_width_size * win_height_size;

  const int num_window = 2;

  const float c1 = 0.0049;
  const float c2 = 0.0441;

  static auto for_each_pixel_window = [&](int wy, int wx,
                                          function<void(int, int)> handler) {
    for (int y = 0; y < win_height_size; y++)
      for (int x = 0; x < win_width_size; x++) {
        const int x_coord = x + win_width_size * wx;
        const int y_coord = y + win_height_size * wy;
        handler(y_coord, x_coord);
      }
  };

  for (int wy = 0; wy < num_window; wy++) {
    for (int wx = 0; wx < num_window; wx++) {
      float src_luma_avg = 0;
      float tar_luma_avg = 0;
      float src_var = 0;
      float tar_var = 0;
      float cvar = 0;

      for_each_pixel_window(wy, wx, [&](int y, int x) {
        auto src_color = src(y, x);
        auto tar_color = target(y, x);

        // TODO: optimize the algorithm by storing the result
        src_luma_avg += luma_cal(src_color) / 255;
        tar_luma_avg += luma_cal(tar_color) / 255;
      });

      src_luma_avg /= num_win_pixel;
      tar_luma_avg /= num_win_pixel;

      for_each_pixel_window(wy, wx, [&](int y, int x) {
        auto src_color = src(y, x);
        auto tar_color = target(y, x);

        float src_luma = luma_cal(src_color) / 255;
        float tar_luma = luma_cal(tar_color) / 255;

        const float nom_src_var = src_luma - src_luma_avg;
        const float nom_tar_var = tar_luma - tar_luma_avg;

        src_var += nom_src_var * nom_src_var;
        tar_var += nom_tar_var * nom_tar_var;
        cvar += nom_src_var * nom_tar_var;
      });

      src_var /= num_win_pixel;
      tar_var /= num_win_pixel;
      cvar /= num_win_pixel;

      MSSIM +=
          ((2 * src_luma_avg * tar_luma_avg + c1) * (2 * cvar + c2)) /
          ((src_luma_avg * src_luma_avg + tar_luma_avg * tar_luma_avg + c1) *
           (src_var + tar_var + c2));
    }

    return MSSIM / num_window;
  }
}

static Image mosaics(Image &img) {
  // load images dataset
  static vector<Image> dataset{};
  static bool init = false;
  if (!init) {
    std::string path =
        "/Users/dannylau/Program/COMP4411-Impressionist/squared_images";
    for (const auto &entry : fs::directory_iterator(path)) {
      auto path = entry.path();
      dataset.push_back(Image::from(path.u8string().c_str()));
    }
  }
  return {};
}
} // namespace ImageUtils

#endif // __IMAGE_UTILS__
