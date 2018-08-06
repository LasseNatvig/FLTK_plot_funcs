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

template <class T> class myFct : public Shape {
public:
	myFct(Fcti* f, double r1, double r2, Point orig, int count = 100, double xscale = 25, double yscale = 25, T precision = 1.0);
	~myFct() {}

	void set_f(Fcti* p) { f = p; }
	void set_r1(double r) { r1 = r; }
	void set_r2(double r) { r2 = r; }
	void set_orig(Point o) { orig = o; }
	void set_count(int c) { count = c; }
	void set_xscale(double x) { xscale = x; }
	void set_yscale(double y) { yscale = y; }
	void set_precision(T p) { precision = p; }

	double get_r1() { return r1; }
	double get_r2() { return r2; }
	Point get_orig() { return orig; }
	int get_count() { return count; }
	double get_xscale() { return xscale; }
	double get_yscale() { return yscale; }
	T get_precision() { return precision; }

	void calc();
	void reset(Fcti* f, double r1, double r2, Point orig, int count = 100, double xscale = 25, double yscale = 25, T precision = 1.0);

private:
	Fcti * f;
	double r1;
	double r2;
	Point orig;
	int count;
	double xscale;
	double yscale;
	T precision;
};
template <class T> myFct<T>::myFct(Fcti* f, double r1, double r2, Point orig, int count, double xscale, double yscale, T precision)
	:f(f), r1(r1), r2(r2), orig(orig), count(count), xscale(xscale), yscale(yscale), precision(precision)
{
	calc();
}
template <class T> void myFct<T>::calc() {
	if (r2 - r1 <= 0) error("bad graphing range");
	if (count <= 0) error("non-positive graphing count");
	double dist = (r2 - r1) / count;
	double r = r1;

	for (int i = 0; i < count; ++i) {
		int x = orig.x + static_cast<int>(int(int(r * xscale) / precision) * precision);
		int y = orig.y - static_cast<int>(int(int(f(r) * yscale) / precision) * precision);
		add(Point{ x, y });
		r += dist;
	}
}
template <class T> void myFct<T>::reset(Fcti* f, double r1, double r2, Point orig, int count, double xscale, double yscale, T precision) {
	set_f(f);
	set_r1(r1);
	set_r2(r2);
	set_orig(orig);
	set_count(count);
	set_xscale(xscale);
	set_yscale(yscale);
	set_precision(precision);
	calc();
}

struct app_window : Window {
	enum graph_enum {
		sin_g, cos_g
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
	myFct<double> funct;

	//double x_manipulation(double x, int m) { return x * m; }
	double my_sin(double x) { return sin(x); }

	// actions invoked by callbacks
	void quit() { hide(); }

	void graph_type_menu_pressed() { 
		graph_type.hide(); 
		graph_type_menu.show(); 
	}
	void sin_graph();

	// callback functions
	static void cb_quit(Address, Address pw) { reference_to<app_window>(pw).quit(); }
	static void cb_calculate(Address, Address pw) { reference_to<app_window>(pw).draw_graph(); }
	static void cb_graph_type_menu_pressed(Address, Address pw) { reference_to<app_window>(pw).graph_type_menu_pressed(); }
	static void cb_sin_graph(Address, Address pw) { reference_to<app_window>(pw).sin_graph(); }

};

app_window::app_window(Point xy, int w, int h, const string& title) : Window(xy, w, h, title),
	quit_button(Point{ x_max() - 80, 10 }, 70, 20, "Quit", cb_quit),
	show_button(Point{ x_max() - 80, 35 }, 70, 20, "Show", cb_calculate),
	graph_type(Point{ 20, 0}, 100, 20, "Graph Type", cb_graph_type_menu_pressed),
	graph_type_menu(Point{ 20, 0 }, 100, 20, Menu::vertical, "Graph Type"),
	base_equation(Point{ x_max() / 2, 0 }, 90, 20, "equation"),
	x_parameter(Point{100, 40}, 20, 20, "t-paramter"),
	equation(Point{ x_max() / 2, 40 }, 90, 20, "equation"),
	x_axis(Axis::x, Point{ orig.x - 200, orig.y }, 400, 20, "1 == 20"),
	y_axis(Axis::y, Point{ orig.x, orig.y + 200 }, 400, 20, "1 == 20"),
	funct(mysin, r_min, r_max, orig, n_points, x_scale, y_scale) {
	attach(quit_button);
	attach(show_button);
	attach(graph_type);
	attach(base_equation);
	attach(x_parameter);
	attach(equation);

	graph_type_menu.attach(new Button(Point{ 0, 0 }, 0, 0, "sin", cb_sin_graph));

	attach(graph_type_menu);

	x_axis.set_color(Color::black);
	y_axis.set_color(Color::black);
	attach(x_axis);
	attach(y_axis);

	funct.set_color(FL_BLACK);
	attach(funct);
	funct.set_color(Color::invisible);

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
		// most painful lambda to function pointer work-around ever.
		// saved the day:
		// https://deviorel.wordpress.com/2015/01/27/obtaining-function-pointers-from-lambdas-in-c/

		int t = x_parameter.get_int();
		auto la = [=](double x) { return sin(x) * t; };

		static function< double(double) > static_variable;
		static_variable = la;
		double(*ptr)(double) = [](double x) { return static_variable(x); };

		funct.reset(
			[](double x) { return static_variable(x); }
			, r_min
			, r_max
			, orig
			, n_points
			, x_scale
			, y_scale
		);
	}
	funct.set_color(Color::visible);
	redraw();
}

int main() {
	app_window win(Point{ 100, 100 }, xmax, ymax + 20, "Text and graphics demo");
	return gui_main();
}