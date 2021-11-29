#pragma once
#include <math.h>

class Vector3D {
public:
	double x;
	double y;
	double z;

	Vector3D();
	Vector3D(double x0, double y0, double z0);

	void SetVector3D(double x0, double y0, double z0);
	void add(Vector3D p1, Vector3D p2);
	void substract(Vector3D p1, Vector3D p2);
	void cross(Vector3D p1, Vector3D p2);
	void scalar(float times);
	void normalization();
	double length();
	double lengthsquare();
	void reverse();

	Vector3D operator-(void);
	Vector3D operator+(const Vector3D& p1);
	Vector3D operator-(const Vector3D& p1);
	Vector3D operator^(const Vector3D& e1);
	Vector3D operator*(const double& scale);
	Vector3D operator/(const double& scale);
	double operator*(const Vector3D& e1);
	bool operator==(const Vector3D& p1);
};