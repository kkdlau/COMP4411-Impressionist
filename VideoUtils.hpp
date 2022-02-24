#pragma once
#include <Vfw.h>


namespace VideoUtils {
	static void open_video(const char* video_path, PAVISTREAM& stream, PAVIFILE& pfile) {
		AVIFileInit();
		AVIFileOpen(&pfile, video_path, OF_READ, NULL);
		HRESULT hr = AVIStreamOpenFromFile(&stream, video_path, streamtypeVIDEO, 0, OF_READ, NULL);
	}

	static void get_video_info(PAVIFILE& pfile, AVIFILEINFO& info) {
		AVIFileInfo(pfile, &info, sizeof(AVIFILEINFO));
	}

	static auto get_frame_pointer(PAVIFILE& pfile, PAVISTREAM& stream, AVIFILEINFO& info) {
		BITMAPINFOHEADER bi;
		long format_length = sizeof(bi);
		AVIStreamReadFormat(stream, 0, &bi, &format_length);
		bi.biBitCount = 24;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		PGETFRAME get_frame = AVIStreamGetFrameOpen(stream, &bi);
		void* buffer = AVIStreamGetFrame(get_frame, 0);
		const long num_bytes = info.dwWidth * info.dwHeight * 3;
		unsigned char* bitmap = new unsigned char[num_bytes];

		CopyMemory(bitmap, (char*)buffer + ((BITMAPINFOHEADER*)buffer)->biSize, num_bytes);

		return bitmap;
	}
};