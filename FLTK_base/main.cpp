// Based on: https://bumpyroadtocode.com/2018/04/08/visual-graphing-program-fltk/   (Copied in 313 lines)
/* 
*** TODO (prioritert):
	- reduser maksimalt, forstå alt, kunne legge ut som eksempel på test-opplegg for både txt og graphics i øving, ...
	  . forenkler; behold sinus, men gjør om cosinus til plynom (+ se bilde jeg har)
	  . fjerner alt med cosinus, fjerne enum etterpå, fjerner behov for template?
	- kombinert med å bruke til å teste funksjoner, har bilde
	- prøv følge graphing a function  i PPP kap. 15

*/
#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"
#include "std_lib_facilities.h"
#include <functional>

// layout
constexpr int xmax = 600;
constexpr int ymax = 600;
constexpr int x_orig = xmax / 2;
constexpr int y_orig = ymax / 2;
Point orig{ x_orig, y_orig };

constexpr int r_min = -10;
constexpr int r_max = 10;
constexpr int n_points = 40;   
constexpr int x_scale = 20;
constexpr int y_scale = 20;

// layout
constexpr int xoffset = 100;
constexpr int yoffset = 100;
constexpr int xspace = 100;
constexpr int yspace = 100;
constexpr int xlength = xmax - xoffset - xspace;
constexpr int ylength = ymax - yoffset - yspace;

typedef double Fcti(double);
double mysin(double x) { return sin(x); }

struct app_window : Window {
	enum graph_enum {
		sin_g, poly_g
	};
	app_window(Point xy, int w, int h, const string& title);
	void draw_graph();
private:
	Button quit_button;
	Button show_button; 
	Button graph_type;
	Menu graph_type_menu;
	In_box x_parameter;
	Out_box base_equation;
	Out_box equation;
	graph_enum g_type;
	Axis x_axis;
	Axis y_axis;
	Open_polyline func_toPlot;  // Changed by Lasse
	double my_sin(double x) { return sin(x); }
	// actions invoked by callbacks
	void quit() { hide(); }
	void graph_type_menu_pressed() { 
		graph_type.hide(); 
		graph_type_menu.show(); 
	}
	void sin_graph();
	// callback functions
	static void cb_quit(Address, Address pw) { 
		reference_to<app_window>(pw).quit(); 
	}
	static void cb_calculate(Address, Address pw) { 
		reference_to<app_window>(pw).draw_graph(); 
	}
	static void cb_graph_type_menu_pressed(Address, Address pw) { 
		reference_to<app_window>(pw).graph_type_menu_pressed(); 
	}
	static void cb_sin_graph(Address, Address pw) { 
		reference_to<app_window>(pw).sin_graph(); 
	}
};

app_window::app_window(Point xy, int w, int h, const string& title) : Window(xy, w, h, title),
quit_button(Point{ x_max() - 80, 10 }, 70, 20, "Quit", cb_quit),
show_button(Point{ x_max() - 80, 35 }, 70, 20, "Show", cb_calculate),
graph_type(Point{ 20, 10 }, 100, 20, "Graph Type", cb_graph_type_menu_pressed),
graph_type_menu(Point{ 20, 0 }, 100, 20, Menu::vertical, "Graph Type"),
base_equation(Point{ x_max() / 2, 0 }, 90, 20, "equation"),
x_parameter(Point{ 100, 40 }, 20, 20, "t-parameter"),
equation(Point{ x_max() / 2, 40 }, 90, 20, "equation"),
x_axis(Axis::x, Point{ orig.x - 200, orig.y }, 400, 20, "1 == 20"),
y_axis(Axis::y, Point{ orig.x, orig.y + 200 }, 400, 20, "1 == 20"),
// funct(mysin, r_min, r_max, orig, n_points, x_scale, y_scale) 
func_toPlot() // New code shape
{
	attach(quit_button);
	attach(show_button);
	attach(graph_type);
	attach(base_equation);
	attach(x_parameter);
	attach(equation);
	// NOTE: Menu is using template-class Vector_ref see paragraph 13.10
	// TODO hide it in a .h file (since early in course and it uses new ... )
	graph_type_menu.attach(new Button(Point{ 0, 0 }, 0, 0, "sin", cb_sin_graph)); 
	graph_type_menu.attach(new Button(Point{ 0, 0 }, 0, 0, "poly", cb_sin_graph)); // TODO shows two buttons in menu, but poly is NOT impelmented
	attach(graph_type_menu);
	x_axis.set_color(Color::black);
	y_axis.set_color(Color::black);
	attach(x_axis);
	attach(y_axis);
	func_toPlot.set_color(FL_BLACK);
	attach(func_toPlot);
	func_toPlot.set_color(Color::invisible);
	graph_type_menu.hide();
}

void app_window::sin_graph() {
	graph_type_menu.hide();
	graph_type.show();
	base_equation.put("y = sin(x) * t");
	g_type = sin_g;
}

void app_window::draw_graph() {
	if (g_type == sin_g){
		equation.put("y = sin(x) * " + x_parameter.get_string());
		... legg inn enkel polylineeksempel fra boka her 
		cout << " add code here for shape funct ... \n";
	}
	//funct.set_color(Color::visible);
	redraw();
}
int main() {
	app_window win(Point{ 100, 100 }, xmax, ymax + 20, "Text and graphics demo");
	return gui_main();
}