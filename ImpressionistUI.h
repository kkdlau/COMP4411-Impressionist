//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H> // FLTK file chooser
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"

class ImpressionistUI {
public:
  ImpressionistUI();

  // The FLTK widgets
  Fl_Window *m_mainWindow;
  Fl_Menu_Bar *m_menubar;

  PaintView *m_paintView;
  OriginalView *m_origView;

  // for brush dialog
  Fl_Window *m_brushDialog;
  Fl_Choice *m_BrushTypeChoice;
  Fl_Choice *m_StrokeDirection;
  

  Fl_Slider *m_BrushSizeSlider;
  Fl_Slider *m_BrushWidthSlider;
  Fl_Slider *m_BrushAngleSlider;
  Fl_Slider *m_BrushAlphaSlider;
  Fl_Slider* m_BrushBlurSlider; 
  Fl_Button *m_ClearCanvasButton;
  Fl_Check_Button* m_ColorBlending;
  Fl_Button* m_AutoPaint;

  // for color blending dialog
  Fl_Window* m_ColorDialog;
  Fl_Color_Chooser* m_ColorChooser;

  // Member functions
  void setDocument(ImpressionistDoc *doc);
  ImpressionistDoc *getDocument();

  void show();
  void resize_windows(int w, int h);

  // Interface to get attribute

  int getSize();
  void setSize(int size);

  int getWidth();
  void setWidth(int width);

  int getAngle();
  void setAngle(int angle);

  float getAlpha();
  void setAlpha(float a);

  int getColorBlending();
  void setColorBlending(int a);

  StrokeDirection get_direction();
  void set_direction(StrokeDirection d);

  vector<double> getUserColor();

  int getBlurValue();
  void setBlurValue(int a);

private:
  ImpressionistDoc
      *m_pDoc; // pointer to document to communicate with the document

  // All attributes here
  int m_nSize = 10;
  int m_nWidth = 1;
  int m_nAngle = 0;
  float m_fAlpha = 1.0;
  int m_fColorBlending = 0;
  StrokeDirection m_direction = SLIDER_RIGHT_MOUSE;
  int m_fBlur = 0;

  // Static class members
  static Fl_Menu_Item menuitems[];
  static Fl_Menu_Item brushTypeMenu[NUM_BRUSH_TYPE + 1];
  static Fl_Menu_Item strokeDirectionMenu[NUM_STROKE_DIRECTION_METHODS + 1];

  static ImpressionistUI *whoami(Fl_Menu_ *o);

  // All callbacks here.  Callbacks are declared
  // static
  static void cb_load_image(Fl_Menu_ *o, void *v);
  static void cb_save_image(Fl_Menu_ *o, void *v);
  static void cb_brushes(Fl_Menu_ *o, void *v);
  static void cb_clear_canvas(Fl_Menu_ *o, void *v);
  static void cb_exit(Fl_Menu_ *o, void *v);
  static void cb_about(Fl_Menu_ *o, void *v);
  static void cb_undo(Fl_Menu_ *o, void *v);
  static void cb_swap_content(Fl_Menu_ *o, void *v);
  static void cb_dissolve_iamge(Fl_Menu_ *o, void *v);
  static void cb_color_blending(Fl_Menu_* o, void* v);
  static void cb_brushChoice(Fl_Widget *o, void *v);
  static void cb_strokeDirectionChoice(Fl_Widget *o, void *v);
  static void cb_clear_canvas_button(Fl_Widget *o, void *v);
  static void cb_sizeUpdate(Fl_Widget *o, void *v);
  static void cb_widthUpdate(Fl_Widget *o, void *v);
  static void cb_angleUpdate(Fl_Widget *o, void *v);
  static void cb_alphaUpdate(Fl_Widget *o, void *v);
  static void cb_toggleOriginalView(Fl_Widget *o, void *v);
  static void cb_colorBlendingUpdate(Fl_Widget* o, void* v);
  static void cb_blurUpdate(Fl_Widget* o, void* v);
  static void cb_autoPaint(Fl_Widget* o, void* v);
};

#endif
