#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"
#include <functional>
#include "std_lib_facilities.h"
#include <cassert>

// layout
constexpr int xmax = 800;
constexpr int ymax = 500;
constexpr int x_orig = (xmax / 2);
constexpr int y_orig = 40 + (ymax / 2);
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

