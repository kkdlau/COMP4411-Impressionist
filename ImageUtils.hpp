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

static void filterMean(Image &img, const Point source, std::vector<int> &color,
                       const int filter_dim) {
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
      if (x < 0)
        x = 0;
      else if (x >= img.width)
        x = img.width - 1;
      if (y < 0)
        y = 0;
      else if (y >= img.height)
        y = img.height - 1;
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

static void applyBlurFilter(Image &original, Image &result,
                            const int filter_dim) {
  for (int i = 1; i < result.width; ++i) {
    for (int j = 1; j < result.height; ++j) {
      // Point source(i, original.height - j);
      Point source(i, j);
      std::vector<int> color{0, 0, 0};

      filterMean(original, source, color, filter_dim);
      RGBA final_color = make_tuple(color[0], color[1], color[2], 255);
      result.set_pixel(source, final_color);
    }
  }
}
static float colorDifference(RGBA c1, RGBA c2) {
    float r = pow((get<0>(c1) - get<0>(c2)), 2);
    float g = pow((get<1>(c1) - get<1>(c2)), 2);
    float b = pow((get<2>(c1) - get<2>(c2)), 2);
    float dist = sqrt(r + g + b);
    return sqrt(r + g + b);
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
  return {gx, gy, atan2(gy, gx)}; // 3rd is the gradient
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

static tuple<float, float, float>
to_ybr(const tuple<GLubyte &, GLubyte &, GLubyte &, GLubyte &> &color) {
  auto r = get<0>(color);
  auto g = get<1>(color);
  auto b = get<2>(color);

  auto y = 16 + 65.738 * r / 256 + 129.057 * g / 256 + 25.064 * b / 256;
  auto cb = 128 + 37.945 * r / 256 + 74.494 * g / 256 + 112.439 * b / 256;
  auto cr = 128 + 112.439 * r / 256 + 94.154 * g / 256 + 18.285 * b / 256;

  return {y, cb, cr};
}

static float structural_similarity(Image &src, Image &target) {
  float MSSIM[3] = {0}; // Y, Cr, Cb

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
      float src_luma_avg[3] = {0};
      float tar_luma_avg[3] = {0};
      float src_var[3] = {0};
      float tar_var[3] = {0};
      float cvar[3] = {0};

      for_each_pixel_window(wy, wx, [&](int y, int x) {
        auto src_color = to_ybr(src(y, x));
        auto tar_color = to_ybr(target(y, x));

        // TODO: optimize the algorithm by storing the result
        src_luma_avg[0] += get<0>(src_color) / 235;
        src_luma_avg[1] += get<1>(src_color) / 240;
        src_luma_avg[2] += get<2>(src_color) / 240;

        tar_luma_avg[0] += get<0>(tar_color) / 235;
        tar_luma_avg[1] += get<1>(tar_color) / 240;
        tar_luma_avg[2] += get<2>(tar_color) / 240;
      });

      src_luma_avg[0] /= num_win_pixel;
      src_luma_avg[1] /= num_win_pixel;
      src_luma_avg[2] /= num_win_pixel;

      tar_luma_avg[0] /= num_win_pixel;
      tar_luma_avg[1] /= num_win_pixel;
      tar_luma_avg[2] /= num_win_pixel;

      for_each_pixel_window(wy, wx, [&](int y, int x) {
        auto src_color = to_ybr(src(y, x));
        auto tar_color = to_ybr(target(y, x));

        float src_luma[3];
        float tar_luma[3];

        src_luma[0] = get<0>(src_color) / 235;
        src_luma[1] = get<1>(src_color) / 240;
        src_luma[2] = get<2>(src_color) / 240;

        tar_luma[0] = get<0>(tar_color) / 235;
        tar_luma[1] = get<1>(tar_color) / 240;
        tar_luma[2] = get<2>(tar_color) / 240;

        for (int i = 0; i < 3; i++) {
          float nom_src_var = src_luma[i] - src_luma_avg[i];
          float nom_tar_var = tar_luma[i] - tar_luma_avg[i];
          src_var[i] += nom_src_var * nom_src_var;
          tar_var[i] += nom_tar_var * nom_tar_var;
          cvar[i] += nom_src_var * nom_tar_var;
        }
      });

      for (int i = 0; i < 3; i++) {
        src_var[i] /= num_win_pixel;
        tar_var[i] /= num_win_pixel;
        cvar[i] /= num_win_pixel;
        MSSIM[i] += ((2 * src_luma_avg[i] * tar_luma_avg[i] + c1) *
                     (2 * cvar[i] + c2)) /
                    ((src_luma_avg[i] * src_luma_avg[i] +
                      tar_luma_avg[i] * tar_luma_avg[i] + c1) *
                     (src_var[i] + tar_var[i] + c2));
      }
    }

    return MSSIM[0] / num_window * 0.8 + MSSIM[1] / num_window * 0.1 +
           MSSIM[2] / num_window * 0.1;
  }
}

static Image mosaics(Image &img, const char *directory) {
  // load images dataset
  static vector<Image> dataset{};
  static vector<string> alias{};
  static bool init = false;

  const int DWIDTH = 5;
  const int DHEIGHT = DWIDTH;
  int count = 0;

  if (!init) {
    std::string path{directory};
    for (const auto &entry : fs::directory_iterator(path)) {
      auto path = entry.path().u8string();
      if (path.find("bmp") != std::string::npos) {
        alias.push_back(string{path});
        dataset.push_back(Image::from(path.c_str()));

        count++;
      }
    }
  }

  debugger("loading done: %d", count);

  vector<decltype(dataset.begin())> replacement{};

  auto best_entry = dataset.begin();

  for (int x = 0; x < img.width - DWIDTH; x += DWIDTH) {
    for (int y = 0; y < img.height - DHEIGHT; y += DHEIGHT) {

      Point start{x, y};
      Point end = start.shift_x(DWIDTH).shift_y(DHEIGHT);
      debugger("crop %d %d  to %d %d", start.x, start.y, end.x, end.y);

      Image cropped = img.crop(start, end);

      double best = 0;
      int i = 0;
      for (auto entry = dataset.begin(); entry != dataset.end(); entry++) {

        float sim = structural_similarity(cropped, *entry);
        // debugger("%s - similar: %.3f", alias[i++].c_str(), sim);

        if (sim > best) {
          best = sim;
          best_entry = entry;
        }
      }
      img.paint(*best_entry, start);
    }
  }

  return {};
}
} // namespace ImageUtils

#endif // __IMAGE_UTILS__
