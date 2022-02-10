//
// impressionistUI.h
//
// The user interface part for the program.
//

#include <FL/fl_ask.h>

#include <math.h>

#include "ImageUtils.hpp"
#include "impressionistDoc.h"
#include "impressionistUI.h"

/*
//------------------------------ Widget Examples
------------------------------------------------- Here is some example code for
all of the widgets that you may need to add to the project.  You can copy and
paste these into your code and then change them to make them look how you want.
Descriptions for all of the widgets here can be found in links on the fltk help
session page.

//---------Window/Dialog and Menubar-----------------------------------

        //----To install a window--------------------------
        Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
                myWindow->user_data((void*)(this));	// record self to be
used by static callback functions

                // install menu bar
                myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
                Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
                        { "&File",		0, 0, 0, FL_SUBMENU },
                                { "&Load...",	FL_ALT + 'l', (Fl_Callback
*)ImpressionistUI::cb_load }, { "&Save...",	FL_ALT + 's', (Fl_Callback
*)ImpressionistUI::cb_save }.
                                { "&Quit",			FL_ALT + 'q',
(Fl_Callback *)ImpressionistUI::cb_exit }, { 0 }, { "&Edit",		0, 0, 0,
FL_SUBMENU }, { "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy,
(void *)COPY }, { "&Cut",	FL_ALT + 'x', (Fl_Callback
*)ImpressionistUI::cb_cut, (void *)CUT }, { "&Paste",	FL_ALT + 'v',
(Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE }, { 0 }, { "&Help",
0, 0, 0, FL_SUBMENU }, { "&About",	FL_ALT + 'a', (Fl_Callback
*)ImpressionistUI::cb_about }, { 0 }, { 0 }
                };
                myMenubar->menu(myMenuItems);
    m_mainWindow->end();

        //----The window callback--------------------------
        // One of the callbacks
        void ImpressionistUI::cb_load(Fl_Menu_* o, void* v)
        {
                ImpressionistDoc *pDoc=whoami(o)->getDocument();

                char* newfile = fl_file_chooser("Open File?", "*.bmp",
pDoc->getImageName() ); if (newfile != NULL) { pDoc->loadImage(newfile);
                }
        }


//------------Slider---------------------------------------

        //----To install a slider--------------------------
        Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My
Value"); mySlider->user_data((void*)(this));	// record self to be used by
static callback functions mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
        mySlider->minimum(1);
        mySlider->maximum(40);
        mySlider->step(1);
        mySlider->value(m_nMyValue);
        mySlider->align(FL_ALIGN_RIGHT);
        mySlider->callback(cb_MyValueSlides);

        //----The slider callback--------------------------
        void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
        {
                ((ImpressionistUI*)(o->user_data()))->m_nMyValue=int(
((Fl_Slider *)o)->value() ) ;
        }


//------------Choice---------------------------------------

        //----To install a choice--------------------------
        Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
        myChoice->user_data((void*)(this));	 // record self to be used by
static callback functions Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
          {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void
*)ONE},
          {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void
*)TWO},
          {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice,
(void *)THREE}, {0}
        };
        myChoice->menu(myChoiceMenu);
        myChoice->callback(cb_myChoice);

        //-----The choice callback-------------------------
        void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
        {
                ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
                ImpressionistDoc* pDoc=pUI->getDocument();

                int type=(int)v;

                pDoc->setMyType(type);
        }


//------------Button---------------------------------------

        //---To install a button---------------------------
        Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
        myButton->user_data((void*)(this));   // record self to be used by
static callback functions myButton->callback(cb_myButton);

        //---The button callback---------------------------
        void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
        {
                ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
                ImpressionistDoc* pDoc = pUI->getDocument();
                pDoc->startPainting();
        }


//---------Light Button------------------------------------

        //---To install a light button---------------------
        Fl_Light_Button* myLightButton = new
Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
        myLightButton->user_data((void*)(this));   // record self to be used by
static callback functions myLightButton->callback(cb_myLightButton);

        //---The light button callback---------------------
        void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
        {
                ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

                if (pUI->myBool==TRUE) pUI->myBool=FALSE;
                else pUI->myBool=TRUE;
        }

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
        Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
        myInput->user_data((void*)(this));   // record self to be used by static
callback functions myInput->callback(cb_myInput);

        //---The int input callback------------------------
        void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
        {
                ((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int(
((Fl_Int_Input *)o)->value() );
        }

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions
//--------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI *ImpressionistUI::whoami(Fl_Menu_ *o) {
  return ((ImpressionistUI *)(o->parent()->user_data()));
}

//--------------------------------- Callback Functions
//--------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();
#if PROJ_DEBUG
  char *newfile =
      "/Users/dannylau/Program/COMP4411-Impressionist/images/babyraptor.bmp";
#else
  char *newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
#endif

  if (newfile != NULL) {
    pDoc->loadImage(newfile);
  }
}

//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  char *newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp");
  if (newfile != NULL) {
    pDoc->saveImage(newfile);
  }
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_ *o, void *v) {
  whoami(o)->m_brushDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  pDoc->clearCanvas();
}

void ImpressionistUI::cb_undo(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  pDoc->undo_painting();
}

void ImpressionistUI::cb_dissolve_iamge(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

#ifdef PROJ_DEBUG
  char *newfile =
      "/Users/dannylau/Program/COMP4411-Impressionist/images/bean.bmp";
#else
  char *newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
#endif
  Image src = Image::from(newfile);
  debugger("w:%d h:%d", src.width, src.height);
  Image output = ImageUtils::dissolve(src, pDoc->m_pUI->m_paintView->cur);
  pDoc->m_pUI->m_paintView->set_current_img(output);
}

void ImpressionistUI::cb_swap_content(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  pDoc->swap_content();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_ *o, void *v) {
  whoami(o)->m_mainWindow->hide();
  whoami(o)->m_brushDialog->hide();
}

//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_ *o, void *v) {
  fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

//------- UI should keep track of the current for all the controls for answering
// the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush
// choice.
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));
  ImpressionistDoc *pDoc = pUI->getDocument();

  int type = (int)(size_t)v;

  pDoc->setBrushType(type);
  pDoc->m_pCurrentBrush->select();
}

void ImpressionistUI::cb_strokeDirectionChoice(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));
  pUI->m_direction = (StrokeDirection)(size_t)v;
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget *o, void *v) {
  ImpressionistDoc *pDoc = ((ImpressionistUI *)(o->user_data()))->getDocument();

  pDoc->clearCanvas();
}

//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeUpdate(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))
      ->setSize(int(((Fl_Slider *)o)->value()));
}

void ImpressionistUI::cb_widthUpdate(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))
      ->setWidth(int(((Fl_Slider *)o)->value()));
}

void ImpressionistUI::cb_angleUpdate(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))
      ->setAngle(int(((Fl_Slider *)o)->value()));
}

void ImpressionistUI::cb_alphaUpdate(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))
      ->setAlpha(float(((Fl_Slider *)o)->value()));
}

void ImpressionistUI::cb_toggleOriginalView(Fl_Widget *o, void *v) {
  ImpressionistDoc *pDoc = ((ImpressionistUI *)(o->user_data()))->getDocument();
  pDoc->toggleOriginalView();
}

//---------------------------------- per instance functions
//--------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc *ImpressionistUI::getDocument() { return m_pDoc; }

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
  m_mainWindow->show();
  m_paintView->show();
  m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
  m_paintView->size(w, h);
  m_origView->size(w, h);
}

//------------------------------------------------
// Set the ImpressionistDoc used by the UI to
// communicate with the brushes
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc *doc) {
  m_pDoc = doc;

  m_origView->m_pDoc = doc;
  m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize() { return m_nSize; }
int ImpressionistUI::getWidth() { return m_nWidth; }
int ImpressionistUI::getAngle() { return m_nAngle; }
float ImpressionistUI::getAlpha() { return m_fAlpha; }

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize(int size) {
  m_nSize = size;

  if (size <= 40)
    m_BrushSizeSlider->value(m_nSize);
}

void ImpressionistUI::setWidth(int width) {
  m_nWidth = width;

  if (width <= 40)
    m_BrushWidthSlider->value(m_nWidth);
}

void ImpressionistUI::setAngle(int angle) {
  if (angle < 0) {
    angle += 360;
  }
  m_nAngle = angle;

  if (angle <= 360)
    m_BrushAngleSlider->value(m_nAngle);
}

void ImpressionistUI::setAlpha(float a) {
  m_fAlpha = a;

  if (a <= 1.0)
    m_BrushAlphaSlider->value(m_fAlpha);
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
    {"&File", 0, 0, 0, FL_SUBMENU},
    {"&Load Image...", FL_ALT + 'l',
     (Fl_Callback *)ImpressionistUI::cb_load_image},
    {"&Save Image...", FL_ALT + 's',
     (Fl_Callback *)ImpressionistUI::cb_save_image},
    {"&Brushes...", FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes},
    {"&Undo", FL_ALT + 'u', (Fl_Callback *)ImpressionistUI::cb_undo, 0},
    {"&Swap", FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_swap_content, 0},
    {"&Dissolve", FL_ALT + 'd',
     (Fl_Callback *)ImpressionistUI::cb_dissolve_iamge, 0},
    {"&Clear Canvas", FL_ALT + 'c',
     (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER},
    {"&Quit", FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit},
    {0},

    {"&Help", 0, 0, 0, FL_SUBMENU},
    {"&About", FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about},
    {0},

    {0}};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE + 1] = {
    {"Points", FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_POINTS},
    {"Lines", FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_LINES},
    {"Circles", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_CIRCLES},
    {"Scattered Points", FL_ALT + 'q',
     (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_SCATTERED_POINTS},
    {"Scattered Lines", FL_ALT + 'm',
     (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_SCATTERED_LINES},
    {"Scattered Circles", FL_ALT + 'd',
     (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_SCATTERED_CIRCLES},
    {0}};

Fl_Menu_Item
    ImpressionistUI::strokeDirectionMenu[NUM_STROKE_DIRECTION_METHODS + 1] = {
        {"Slider/RIght Mouse", FL_ALT + 's',
         (Fl_Callback *)ImpressionistUI::cb_strokeDirectionChoice,
         (void *)SLIDER_RIGHT_MOUSE},
        {"Gradient", FL_ALT + 'g',
         (Fl_Callback *)ImpressionistUI::cb_strokeDirectionChoice,
         (void *)GRADIENT_DIRECTION},
        {"Brush Direction", FL_ALT + 'b',
         (Fl_Callback *)ImpressionistUI::cb_strokeDirectionChoice,
         (void *)BRUSH_DIRECTION},
        {0}};

//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
  // Create the main window
  m_mainWindow = new Fl_Window(600, 300, "Impressionist");
  m_mainWindow->user_data(
      (void *)(this)); // record self to be used by static callback functions
  // install menu bar
  m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
  m_menubar->menu(menuitems);

  // Create a group that will hold two sub windows inside the main
  // window
  Fl_Group *group = new Fl_Group(0, 25, 600, 275);

  // install paint view window
  // install original view window
  m_origView =
      new OriginalView(0, 25, 300, 275, "This is the orig view"); // 300jon
  m_origView->box(FL_DOWN_FRAME);
  m_origView->deactivate();
  m_paintView =
      new PaintView(300, 25, 300, 275, "This is the paint view"); // 0jon
  m_paintView->box(FL_DOWN_FRAME);

  group->end();
  Fl_Group::current()->resizable(group);
  m_mainWindow->end();

  // brush dialog definition
  m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
  // Add a brush type choice to the dialog
  m_BrushTypeChoice = new Fl_Choice(50, 10, 150, 25, "&Brush");
  m_BrushTypeChoice->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_BrushTypeChoice->menu(brushTypeMenu);
  m_BrushTypeChoice->callback(cb_brushChoice);

  // stroke direction
  m_StrokeDirection = new Fl_Choice(115, 40, 150, 25, "&Stroke Direction");
  m_StrokeDirection->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_StrokeDirection->menu(strokeDirectionMenu);
  m_StrokeDirection->callback(cb_strokeDirectionChoice);

  m_ClearCanvasButton = new Fl_Button(240, 10, 150, 25, "&Clear Canvas");
  m_ClearCanvasButton->user_data((void *)(this));
  m_ClearCanvasButton->callback(cb_clear_canvas_button);

  // Add brush size slider to the dialog
  int y = 80;
  m_BrushSizeSlider = new Fl_Value_Slider(10, y, 300, 20, "Size");
  m_BrushSizeSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
  m_BrushSizeSlider->labelfont(FL_COURIER);
  m_BrushSizeSlider->labelsize(12);
  m_BrushSizeSlider->minimum(1);
  m_BrushSizeSlider->maximum(40);
  m_BrushSizeSlider->step(1);
  m_BrushSizeSlider->value(m_nSize);
  m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
  m_BrushSizeSlider->callback(cb_sizeUpdate);

  m_BrushWidthSlider = new Fl_Value_Slider(10, y += 30, 300, 20, "Line Width");
  m_BrushWidthSlider->user_data((void *)(this));
  m_BrushWidthSlider->type(FL_HOR_NICE_SLIDER);
  m_BrushWidthSlider->labelfont(FL_COURIER);
  m_BrushWidthSlider->labelsize(12);
  m_BrushWidthSlider->minimum(1);
  m_BrushWidthSlider->maximum(40);
  m_BrushWidthSlider->step(1);
  m_BrushWidthSlider->value(m_nWidth);
  m_BrushWidthSlider->align(FL_ALIGN_RIGHT);
  m_BrushWidthSlider->callback(cb_widthUpdate);
  m_BrushWidthSlider->deactivate(); // match Point as default brush

  m_BrushAngleSlider = new Fl_Value_Slider(10, y += 30, 300, 20, "Line Angle");
  m_BrushAngleSlider->user_data((void *)(this));
  m_BrushAngleSlider->type(FL_HOR_NICE_SLIDER);
  m_BrushAngleSlider->labelfont(FL_COURIER);
  m_BrushAngleSlider->labelsize(12);
  m_BrushAngleSlider->minimum(0);
  m_BrushAngleSlider->maximum(360);
  m_BrushAngleSlider->step(1);
  m_BrushAngleSlider->value(m_nAngle);
  m_BrushAngleSlider->align(FL_ALIGN_RIGHT);
  m_BrushAngleSlider->callback(cb_angleUpdate);
  m_BrushAngleSlider->deactivate(); // match Point as default brush

  m_BrushAlphaSlider = new Fl_Value_Slider(10, y += 30, 300, 20, "Alpha");
  m_BrushAlphaSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
  m_BrushAlphaSlider->labelfont(FL_COURIER);
  m_BrushAlphaSlider->labelsize(12);
  m_BrushAlphaSlider->minimum(0.0);
  m_BrushAlphaSlider->maximum(1.0);
  m_BrushAlphaSlider->step(0.01);
  m_BrushAlphaSlider->value(m_nWidth);
  m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);
  m_BrushAlphaSlider->callback(cb_alphaUpdate);

  m_brushDialog->end();
}

StrokeDirection ImpressionistUI::get_direction() { return m_direction; }
void ImpressionistUI::set_direction(StrokeDirection d) {
  m_StrokeDirection->value(d);
}
