#ifndef __VIDEO__

#define __VIDEO__

#include "Image.hpp"
namespace VideoUtils {
void get_video_info();

long num_frames();

void open_video(const char *video_path);

Image get_frame(int frame_index);

Image next_frame();
};     // namespace VideoUtils
#endif // !__VIDEO__
