// GUI - test 2 (github) *** notater i MMAP
#include "TestEnvironment_V1.h"  
struct app_window : Window {
	enum graph_enum { sin_g, poly_g };  // two types of graphs can be plotted 
	app_window(Point xy, int w, int h, const string& title);  
	void draw_graph();
private:
	Button quit_button, show_button, graph_type;  // Lines declaring misc GUI elements, Button PPP Sect. 16.4.2 
	Menu graph_type_menu; // PPP Sect. 16.4.4
	In_box a_coeff, b_coeff, c_coeff, from, to, steps; // PPP Section 16.4.3
	Out_box equation, logg; // PPP Section 16.4.3
	graph_enum g_type; // PPP Section 9.5
	Axis x_axis, y_axis; // PPP Section 16.4.3
	Open_polyline func_toPlot; // PPP Section 13.6
	
	// actions invoked by callbacks:
	void quit() { hide(); }  // PPP Section 16.5 (Page 657)
	void graph_type_menu_pressed() { 
		graph_type.hide();  // PPP Section 16.4.1
		graph_type_menu.show(); // PPP Section 16.4.4
	}
	void sin_graph(); void poly_graph();
	// callback functions, see PPP pages 556-558
	static void cb_quit(Address, Address pw) { reference_to<app_window>(pw).quit(); }
	static void cb_calculate(Address, Address pw) { reference_to<app_window>(pw).draw_graph();  }
	static void cb_graph_type_menu_pressed(Address, Address pw) { reference_to<app_window>(pw).graph_type_menu_pressed();}
	static void cb_sin_graph(Address, Address pw) { reference_to<app_window>(pw).sin_graph(); }
	static void cb_poly_graph(Address, Address pw) { reference_to<app_window>(pw).poly_graph(); }
};

app_window::app_window(Point xy, int w, int h, const string& title) : Window(xy, w, h, title),
quit_button(Point{ x_max() - 80, 10 }, 70, 20, "Quit", cb_quit),
show_button(Point{ x_max() - 80, 35 }, 70, 20, "Show", cb_calculate),
graph_type(Point{ 20, 10 }, 100, 20, "Graph Type", cb_graph_type_menu_pressed),
graph_type_menu(Point{ 20, 10 }, 100, 20, Menu::vertical, "Graph Type"),
equation(Point{ (x_max() / 2) - 100, 10 }, 250, 20, "Equation"), 
logg(Point{ (x_max() / 2), 100 }, 250, 200, "Logg"),
a_coeff(Point{ 100,  60 }, 40, 20, "Coefficient a"),
b_coeff(Point{ 100,  90 }, 40, 20, "Coefficient b"),
c_coeff(Point{ 100, 120 }, 40, 20, "Coefficient c"), 
from (Point{ 100,  160 }, 40, 20, "From (int)"),
to   (Point{ 100,  190 }, 40, 20, "To (int)"),
steps(Point{ 100, 220 }, 40, 20, "Steps"),
x_axis(Axis::x, Point{ orig.x - 200, orig.y }, 400, 20, "1 == 20"),  // TODO Legend not so readable, improve
y_axis(Axis::y, Point{ orig.x, orig.y + 200 }, 400, 20, "1 == 20"),
func_toPlot()
{
	attach(quit_button); attach(show_button); attach(graph_type);
	attach(equation); attach(logg);
	attach(a_coeff); attach(b_coeff); attach(c_coeff);
	attach(from); attach(to); attach(steps);
	// NOTE: Menu is using template-class Vector_ref see paragraph 13.10 // TODO --- les om - forkort kommentar
	// TODO try to hide it in a .h file (since early in course and it uses new ... )
	graph_type_menu.attach(new Button(Point{ 0, 0 }, 0, 0, "sin (red)", cb_sin_graph)); 
	graph_type_menu.attach(new Button(Point{ 0, 0 }, 0, 0, "poly (blue)", cb_poly_graph)); // TODO shows two buttons in menu, but poly is NOT impelmented
	attach(graph_type_menu);
	x_axis.set_color(Color::black); y_axis.set_color(Color::black);
	attach(x_axis); attach(y_axis); attach(func_toPlot);
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
void app_window::draw_graph() {
	if (g_type == sin_g){
		cout << " add code here for shape funct sin  \n";
		equation.put("y = sin(x) * ");
		func_toPlot.set_color(FL_RED);
		func_toPlot.add(Point{ 100,200 });
		func_toPlot.add(Point{ 300,300 });
	}
	else if (g_type == poly_g) {
		cout << " add code here for shape funct poly \n";
		equation.put(" y = " + a_coeff.get_string() + "x**2 + " + b_coeff.get_string() + "x + " + c_coeff.get_string());
		func_toPlot.set_color(FL_BLUE);
		vector<xyPair> values;
		int minX = -20; int maxX = 20; int steps = 10;
		assert(-minX == maxX); // krever symmetri
		values = calcPoly(minX, maxX, steps, a_coeff.get_int(), b_coeff.get_int(), c_coeff.get_int());  // TODO end use of magic values // Legg in assert p� fra -K til +K, alts� Symmetrisk
		double maxY = findMax_absolute_Yvalue(values);
		for (auto p : values) {
			int xPlot, yPlot; // from -20 to +20 in  10 tick
			xPlot = orig.x + static_cast<int>((p.x) / static_cast<double>(maxX) * 200); // viser behovet for cast  --- narrowing conversion
			yPlot = static_cast<int>(orig.y - static_cast<int>(p.y) / static_cast<double>(maxY) * 200);  // turn direction of y
			cout << "x:" << static_cast<int>(p.x) << "   y:" << static_cast<int>(p.y) << endl;
			cout << "xPlot:" << xPlot << "   yPlot:" << yPlot << endl;
			func_toPlot.add(Point{ xPlot, yPlot });
		}
	}
	redraw();
}
int main() {
	app_window win(Point{ 50, 50 }, xmax, ymax + 20, "Text and graphics demo");
	return gui_main();
}