/*Based on: https://bumpyroadtocode.com/2018/04/08/visual-graphing-program-fltk/   (Copied in 313 lines)
*** TODO (prioritert):
	- reduser maksimalt, forstå alt, kunne legge ut som eksempel på test-opplegg for både txt og graphics i øving, ...
	  . forenkler; behold sinus, men gjør om cosinus til plynom (+ se bilde jeg har)
	  . fjerner alt med cosinus, fjerne enum etterpå, fjerner behov for template?
	- kombinert med å bruke til å teste funksjoner, har bilde
	- prøv følge graphing a function  i PPP kap. 15
	- coeff a, b og c brukes bare for poly, la knappene bare være synlig da, og enklere for sinus ? --- se side 573 i PPP, popup-menu ikke så lett, vente med det
	- se Lecture 1 ppt  ideer 
	TODO ONSDAG, sinus-verdier, funksjon returnerer vector av double
		- calc FuncValues
		- enkel fil myMath (kvadratisk likning)
*/
#include "TestEnvironment_V1.h"  // More will be moved here
struct app_window : Window {
	enum graph_enum { sin_g, poly_g };
	app_window(Point xy, int w, int h, const string& title);
	void draw_graph();
private:
	Button quit_button;
	Button show_button; 
	Button graph_type;
	Menu graph_type_menu;
	In_box a_coeff, b_coeff, c_coeff;
	In_box from, to, steps;
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
	void poly_graph();
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
	static void cb_sin_graph(Address, Address pw) { reference_to<app_window>(pw).sin_graph(); }
	static void cb_poly_graph(Address, Address pw) { reference_to<app_window>(pw).poly_graph(); }
};
app_window::app_window(Point xy, int w, int h, const string& title) : Window(xy, w, h, title),
quit_button(Point{ x_max() - 80, 10 }, 70, 20, "Quit", cb_quit),
show_button(Point{ x_max() - 80, 35 }, 70, 20, "Show", cb_calculate),
graph_type(Point{ 20, 10 }, 100, 20, "Graph Type", cb_graph_type_menu_pressed),
graph_type_menu(Point{ 20, 10 }, 100, 20, Menu::vertical, "Graph Type"),
equation(Point{ (x_max() / 2) - 50, 10 }, 150, 20, "Equation"),
// single variable quadratic function
// https://en.wikipedia.org/wiki/Quadratic_function
a_coeff(Point{ 100,  60 }, 40, 20, "Coefficient a"),
b_coeff(Point{ 100,  90 }, 40, 20, "Coefficient b"),
c_coeff(Point{ 100, 120 }, 40, 20, "Coefficient c"), 
from (Point{ 100,  160 }, 40, 20, "From (int)"),
to   (Point{ 100,  190 }, 40, 20, "To (int)"),
steps(Point{ 100, 220 }, 40, 20, "Steps"),
x_axis(Axis::x, Point{ orig.x - 200, orig.y }, 400, 20, "1 == 20"),
y_axis(Axis::y, Point{ orig.x, orig.y + 200 }, 400, 20, "1 == 20"),
func_toPlot() // New code shape
{
	attach(quit_button);
	attach(show_button);
	attach(graph_type);
	attach(equation);
	attach(a_coeff); attach(b_coeff); attach(c_coeff);
	attach(from); attach(to); attach(steps);

	// NOTE: Menu is using template-class Vector_ref see paragraph 13.10
	// TODO try to hide it in a .h file (since early in course and it uses new ... )
	graph_type_menu.attach(new Button(Point{ 0, 0 }, 0, 0, "sin", cb_sin_graph)); 
	graph_type_menu.attach(new Button(Point{ 0, 0 }, 0, 0, "poly", cb_poly_graph)); // TODO shows two buttons in menu, but poly is NOT impelmented
	attach(graph_type_menu);
	x_axis.set_color(Color::black);
	y_axis.set_color(Color::black);
	attach(x_axis);
	attach(y_axis);
	func_toPlot.set_color(FL_RED); // TODO why different color from 3 lines above?, does not work?
	attach(func_toPlot);
	// func_toPlot.set_color(Color::invisible);
	graph_type_menu.hide();
}
void app_window::sin_graph() {
	graph_type_menu.hide();
	graph_type.show();
	equation.put(" y = sin(x) * t .... ");
	g_type = sin_g;
}
void app_window::poly_graph() {
	graph_type_menu.hide();
	graph_type.show();
	equation.put("y = poly ...... ");
	g_type = poly_g;
}

// pair se side 782
typedef pair<double, double> xyPair;
vector<xyPair> calcPoly(int from, int to, int steps, int aCoeff, int bCoeff, int cCoeff) {
	assert(to > from);
	vector<xyPair> values;
	for (int i = 0; i <= steps; i++) {
		double x = from + (i*((to - from) / steps));   // Sjekk at sist eblir med, teste med få steg ...  // antar trenger cast
		xyPair p;
		p.first = x;
		p.second = (aCoeff * x * x) + (bCoeff * x) + cCoeff;
		values.push_back(p);
	}
	return values;
}

void app_window::draw_graph() {
	if (g_type == sin_g){
		cout << " add code here for shape funct sin  \n";
		equation.put("y = sin(x) * ");
		func_toPlot.set_color(FL_GREEN);
		func_toPlot.add(Point{ 100,200 });
		func_toPlot.add(Point{ 200,220 });
		func_toPlot.add(Point{ 300,300 });
	}
	else if (g_type == poly_g) {
		cout << " add code here for shape funct poly \n";
		equation.put(" y = " + a_coeff.get_string() + "x**2 + " + b_coeff.get_string() + "x + " + c_coeff.get_string());
		func_toPlot.set_color(FL_BLUE);
		vector<xyPair> values;
		values = calcPoly(-20, 30, 10, 4, 5, -20);
		for (auto p : values) {
			// PUNKT må plasseres smartere inne i grafen
			func_toPlot.add(Point{ 100 + static_cast<int>(p.first), 100 +static_cast<int>(p.second) }); // viser behovet for cast  --- narrowing conversion
		}
	}
	func_toPlot.set_color(Color::visible);
	redraw();
}
int main() {
	app_window win(Point{ 100, 100 }, xmax, ymax + 20, "Text and graphics demo");

	return gui_main();
}