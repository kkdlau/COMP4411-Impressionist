//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"
#include <FL/FL_Text_Display.H>
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H> // FLTK file chooser

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
  Fl_Slider *m_BrushBlurSlider;
  Fl_Button *m_ClearCanvasButton;
  Fl_Check_Button *m_ColorBlending;
  Fl_Check_Button *m_another_gradient_checkbox;

  Fl_Slider *m_BrushSpacingSlider;
  Fl_Check_Button *m_AutoPaintRandomize;
  Fl_Button *m_AutoPaint;

  Fl_Slider *m_CanvasTransparencySlider;

  // for color blending dialog
  Fl_Window *m_ColorDialog;
  Fl_Color_Chooser *m_ColorChooser;

  // for arbitrary sized filters
  Fl_Window *m_FilterInterface;
  Fl_Text_Buffer *m_FilterBuff;
  Fl_Text_Display *m_FilterDisp;
  Fl_Int_Input *m_FilterRowNum;
  Fl_Int_Input *m_FilterColNum;
  Fl_Multiline_Input *m_FilterValues;
  Fl_Button *m_FilterApply;
  Fl_Check_Button *m_FilterNormalize;

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

  int getSpacing();
  void setSpacing(int a);

  int getAutoPaintRandomize();
  void setAutoPaintRandomize(int a);

  // arbitrary filter
  int getRowNum();
  void setRowNum(int a);

  int getColNum();
  void setColNum(int a);

  void getFilterValues(char *a);
  void setFilterValues();

  bool getNormalize();
  void setNormalize(int a);

  float getTransparency();
  void setTransparency(float a);

  bool get_use_another_gradient();

  void set_use_another_gradient(bool v);

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
  int m_nSpacing = 5;            // spacing for autopaint
  int m_nAutoPaintRandomize = 0; // no randomization of brush attribute
  // arbitrary filter
  int af_rownum = 0;
  int af_colnum = 0;
  int af_normalize = 1;
  char af_values[400]{'0'};  // MAX size of filter is 10x10 (?)
  float m_cTransparency = 0; // set alpha as one (?)
  volatile bool use_another_gradient;

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
  static void cb_mural_image(Fl_Menu_ *o, void *v);
  static void cb_load_another_image(Fl_Menu_ *o, void *v);
  static void cb_color_blending(Fl_Menu_ *o, void *v);
  static void cb_arbitrary_filter(Fl_Menu_ *o, void *v);
  static void cb_brushChoice(Fl_Widget *o, void *v);
  static void cb_strokeDirectionChoice(Fl_Widget *o, void *v);
  static void cb_clear_canvas_button(Fl_Widget *o, void *v);
  static void cb_sizeUpdate(Fl_Widget *o, void *v);
  static void cb_widthUpdate(Fl_Widget *o, void *v);
  static void cb_angleUpdate(Fl_Widget *o, void *v);
  static void cb_alphaUpdate(Fl_Widget *o, void *v);
  static void cb_toggleOriginalView(Fl_Widget *o, void *v);
  static void cb_colorBlendingUpdate(Fl_Widget *o, void *v);
  static void cb_another_gradient(Fl_Widget *o, void *v);
  static void cb_blurUpdate(Fl_Widget *o, void *v);
  static void cb_autoPaint(Fl_Widget *o, void *v);
  static void cb_spacingUpdate(Fl_Widget *o, void *v);
  static void cb_autoPaintRandomize(Fl_Widget *o, void *v);
  static void cb_arbFilterApply(Fl_Widget *o, void *v);
  static void cb_arbFilterNormalize(Fl_Widget *o, void *v);
  static void cb_transparencyUpdate(Fl_Widget *o, void *v);
};

#endif
