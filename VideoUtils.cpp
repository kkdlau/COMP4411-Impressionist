#ifndef __APPLE__

#include "VideoUtils.hpp"
#include <Vfw.h>
#include <windows.h>
#pragma comment(lib, "Vfw32.lib")
using namespace VideoUtils;

PAVIFILE pfile;
PAVISTREAM stream;
AVIFILEINFOA info;
long current_frame = 0;
long max_frames = 0;

void VideoUtils::get_video_info() {
  AVIFileInfo(pfile, &info, sizeof(AVIFILEINFO));
}

long VideoUtils::num_frames() { return AVIStreamLength(stream); }

void VideoUtils::open_video(const char *video_path) {
  AVIFileInit();
  current_frame = 0;
  max_frames = 0;
  AVIFileOpen(&pfile, video_path, OF_READ, NULL);
  HRESULT hr = AVIStreamOpenFromFile(&stream, video_path, streamtypeVIDEO, 0,
                                     OF_READ, NULL);
  get_video_info();
  max_frames = num_frames();
}

Image VideoUtils::get_frame(int frame_index) {
  current_frame = frame_index;
  BITMAPINFOHEADER bih;
  ZeroMemory(&bih, sizeof(BITMAPINFOHEADER));
  bih.biBitCount = 24;
  bih.biClrImportant = 0;
  bih.biClrUsed = 0;
  bih.biCompression = BI_RGB;
  bih.biPlanes = 1;
  bih.biSize = 40;
  bih.biXPelsPerMeter = 0;
  bih.biYPelsPerMeter = 0;
  bih.biSizeImage = (((bih.biWidth * 3) + 3) & 0xFFFC) * bih.biHeight;

  PGETFRAME pframe = AVIStreamGetFrameOpen(stream, NULL);
  BYTE *pDIB = (BYTE *)AVIStreamGetFrame(pframe, current_frame);
  RtlMoveMemory(&bih.biSize, pDIB, sizeof(BITMAPINFOHEADER));
  BYTE *Bits = new BYTE[bih.biSizeImage];
  RtlMoveMemory(Bits, pDIB + sizeof(BITMAPINFOHEADER), bih.biSizeImage);

  //   return Bits;
  Image tmp;
  tmp.set(Bits, bih.biWidth, bih.biHeight);
  return tmp;
}

Image VideoUtils::next_frame() {
  current_frame = (current_frame + 1) % max_frames;
  return get_frame(current_frame);
}

#endif