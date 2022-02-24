#ifndef __VIDEO__

#define __VIDEO__


namespace VideoUtils {
	void get_video_info();

	long num_frames();

	void open_video(const char* video_path);

	auto get_frame(int frame_index);
};
#endif // !__VIDEO__
