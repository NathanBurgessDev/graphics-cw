#include <list>
#include <vector>
#include <algorithm>

#include "point.h"

point evaluate(float t, std::vector<point> P);

point evaluateDerivative(float t, const std::vector<point>& P);

std::vector<point> EvaluateBezierCurve(std::vector<point>ctrl_points, int num_evaluations);

std::vector<float> MakeFloatsFromVector(std::vector<point> curve, int& num_verts, int& num_floats, float r, float g, float b);


