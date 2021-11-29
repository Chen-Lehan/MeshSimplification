#pragma once
#include <vector>
#include <math.h>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>
#include "myVector.h"

class Face {
public:
	bool valid;
	int V[3];
	int N[3];
	Vector3D FN;

	bool operator==(const Face& f);
};

class Point {
public:
	bool valid;
	Vector3D P;
	std::vector<Face*>* F;
};

class Normal {
public:
	bool valid;
	Vector3D N;
};

class PIC {
public:
	std::vector<Point> V;		// vertex
	std::vector<Normal> VN;		// vertex normal
	std::vector<Face> F;		// face

	float GetMaxV();
	void ReadPIC(std::string filename);
	void WritePIC(std::string filename);
	void GetVertexNormal();
	void GetFaceNormal();
	void GetFaceNormal(Face* f);
	void GetFaceNormal(int i);
};