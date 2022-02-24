#ifndef __VIDEO__

#define __VIDEO__

#include <Vfw.h>
#pragma comment(lib, "Vfw32.lib")

namespace VideoUtils {

	extern PAVIFILE pfile;
	extern PAVISTREAM stream;
	extern AVIFILEINFOA info;
	extern long current_frame;
	extern long max_frames;

	void get_video_info();

	long num_frames();

	void open_video(const char* video_path);

	auto get_frame(int frame_index);
};
#endif // !__VIDEO__
