#if !defined(__GL_HELPER__)
#define __GL_HELPER__

#include "gl_inc.hpp"
#include <functional>

using namespace std;

#ifdef PROJ_DEBUG
#define debugger(...)                                                          \
  printf(__VA_ARGS__);                                                         \
  printf("\n");                                                                \
  fflush(stdout);
#else
#define debugger(...)
#endif

static void gl_draw(int drawing_mode, function<void()> drawing_funcs) {
  glBegin(drawing_mode);

  drawing_funcs();

  glEnd();
}

static void gl_set_point(Point p) { glVertex2d(p.x, p.y); }

#endif // __GL_HELPER__
