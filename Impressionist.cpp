//
// impressionist.cpp
//
// The main driver program for the other parts. We have two major components,
// UI and Doc.
// They do have a link to each other as their member such that they can
// communicate.
//

#include <stdio.h>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "Image.hpp"
#include "VideoUtils.hpp"
#include <chrono>
#include <thread>
#include <functional>

ImpressionistUI* impUI;
ImpressionistDoc* impDoc;

void timer_start(std::function<void(void)> func, unsigned int interval)
{
	std::thread([func, interval]()
		{
			while (true)
			{
				auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
				func();
				std::this_thread::sleep_until(x);
			}
		}).detach();
}

void task() {
	if (impDoc->app_mode == VIDEO && impUI->m_paintView->finish_painting_flag) {
		Image frame = VideoUtils::next_frame();
		if (frame.contain_content()) {
			impUI->m_origView->set_current_img(frame);
			impUI->m_paintView->cur = frame;
			impUI->m_paintView->auto_paint_flag = true;
			printf("request draw\n");
			fflush(stdout);
			impUI->m_paintView->refresh();
		}
	} else if (!impUI->m_paintView->finish_painting_flag) {
		printf("skip this loop\n");
		fflush(stdout);
	}
}

int main(int argc, char **argv) {
  impDoc = new ImpressionistDoc();

  // Create the UI
  impUI = new ImpressionistUI();

  // Set the impDoc which is used as the bridge between UI and brushes
  impUI->setDocument(impDoc);
  impDoc->setUI(impUI);
  Fl::visual(FL_DOUBLE | FL_INDEX);


  timer_start(task, 50);

  impUI->show();

  return Fl::run();
}

// global functions
float frand() { return (float)rand() / RAND_MAX; }

int irand(int max) { return rand() % max; }
