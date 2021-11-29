#include "myVector.h"

Vector3D::Vector3D()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3D::Vector3D(double x0, double y0, double z0)
{
	x = x0;
	y = y0;
	z = z0;
}

void Vector3D::SetVector3D(double x0, double y0, double z0)
{
	x = x0;
	y = y0;
	z = z0;
}

void Vector3D::add(Vector3D p1, Vector3D p2)
{
	x = p1.x + p2.x;
	y = p1.y + p2.y;
	z = p1.z + p2.z;
}

void Vector3D::substract(Vector3D p1, Vector3D p2)
{
	x = p1.x - p2.x;
	y = p1.y - p2.y;
	z = p1.z - p2.z;
}

void Vector3D::cross(Vector3D p1, Vector3D p2)
{
	x = p1.y * p2.z - p1.z * p2.y;
	y = p1.z * p2.x - p1.x * p2.z;
	z = p1.x * p2.y - p1.y * p2.x;
}

void Vector3D::scalar(float times)
{
	x = x * times;
	y = y * times;
	z = z * times;
}

void Vector3D::normalization()
{
	double L = sqrt(x * x + y * y + z * z);
	x = x / L;
	y = y / L;
	z = z / L;
}

double Vector3D::length()
{
	return sqrt(x * x + y * y + z * z);
}

double Vector3D::lengthsquare()
{
	return (x * x + y * y + z * z);
}

void Vector3D::reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

Vector3D Vector3D::operator-(void)
{
	Vector3D p;
	p.x = -x;
	p.y = -y;
	p.z = -z;
	return p;
}

Vector3D Vector3D::operator+(const Vector3D& p1)
{
	Vector3D p;
	p.x = x + p1.x;
	p.y = y + p1.y;
	p.z = z + p1.z;
	return p;
}

Vector3D Vector3D::operator-(const Vector3D& p1)
{
	Vector3D p;
	p.x = x - p1.x;
	p.y = y - p1.y;
	p.z = z - p1.z;
	return p;
}

Vector3D Vector3D::operator^(const Vector3D& e1)
{
	Vector3D e;
	e.x = y * e1.z - z * e1.y;
	e.y = z * e1.x - x * e1.z;
	e.z = x * e1.y - y * e1.x;
	return e;
}

Vector3D Vector3D::operator*(const double& scale)
{
	Vector3D p;
	p.x = x * scale;
	p.y = y * scale;
	p.z = z * scale;
	return p;
}

Vector3D Vector3D::operator/(const double& scale)
{
	Vector3D p;
	p.x = x / scale;
	p.y = y / scale;
	p.z = z / scale;
	return p;
}

double Vector3D::operator*(const Vector3D& e1)
{
	return e1.x * x + e1.y * y + e1.z * z;
}

bool Vector3D::operator==(const Vector3D& p1)
{
	if (p1.x == x && p1.y == y && p1.z == z)
		return true;
	else
		return false;
}