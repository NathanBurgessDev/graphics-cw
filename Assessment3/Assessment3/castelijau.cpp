#include "castelijau.h"

point evaluate(float t, std::vector<point> P)
{
	std::vector<point> Q = P;

	while (Q.size() > 1) {
		std::vector<point> R;

		std::vector<point>::iterator p1;
		std::vector<point>::iterator p2;

		p1 = Q.begin();
		p2 = Q.begin();
		++p2;

		do {

			point p = ((1 - t) * (*p1) + (t * (*p2)));
			R.push_back(p);

			if (p2 != Q.end()) {
				++p1;
				++p2;
			}

		} while (p2 != Q.end());
		Q.clear();
 		Q = R;
	}
	return Q.front();
}

point evaluateDerivative(float t, const std::vector<point>& P) {
	std::vector<point> dP;
	int n = P.size() - 1;

	for (int i = 0; i < n; ++i) {
		point dp = (P[i + 1] - P[i]) * float(n);
		dP.push_back(dp);
	}

	return evaluate(t, dP);
}

std::vector<point> EvaluateBezierCurve(std::vector<point>ctrl_points, int num_evaluations)
{
	std::vector<point> ps(ctrl_points.begin(), ctrl_points.end());
	std::vector<point> curve;

	float offset = 1.0 / num_evaluations;
	curve.push_back(ctrl_points.front());

	for (int i = 0; i < num_evaluations; i++) {
		point p = evaluate((offset * (i + 1.0)), ps);
		curve.push_back(p);
	}
	return curve;
}


std::vector<float> MakeFloatsFromVector(std::vector<point> curve, int& num_verts, int& num_floats, float r, float g, float b)
{

	std::vector<float> verts;

	num_verts = curve.size();
	if (num_verts == 0) {
		return verts;
	}


	for (point p : curve) {
		verts.push_back(p.x);
		verts.push_back(p.y);
		verts.push_back(p.z);
		verts.push_back(r);
		verts.push_back(g);
		verts.push_back(b);
	}

	num_floats = verts.size();


	return verts;
}