#include "myModel.h"

bool Face::operator==(const Face& f)
{
	if (f.V[0] == V[0])
	{
		if (f.V[1] == V[1])
		{
			if (f.V[2] == V[2])
			{
				return true;
			}
		}
		else if (f.V[1] == V[2])
		{
			if (f.V[2] == V[1])
			{
				return true;
			}
		}
	}
	else if (f.V[0] == V[1])
	{
		if (f.V[1] == V[2])
		{
			if (f.V[2] == V[3])
			{
				return true;
			}
		}
		else if (f.V[1] == V[0])
		{
			if (f.V[2] == V[2])
			{
				return true;
			}
		}
	}
	else if (f.V[0] == V[2])
	{
		if (f.V[1] == V[1])
		{
			if (f.V[2] == V[0])
			{
				return true;
			}
		}
		else if (f.V[1] == V[0])
		{
			if (f.V[2] == V[1])
			{
				return true;
			}
		}
	}

	return false;
}

float PIC::GetMaxV()
{
	int vlength = V.size();
	int xmax = 0;
	int ymax = 0;
	int zmax = 0;

	for (int i = 0; i < vlength; i++)
	{
		if (xmax < V[i].P.x)
			xmax = V[i].P.x;
		if (ymax < V[i].P.y)
			ymax = V[i].P.y;
		if (zmax < V[i].P.z)
			zmax = V[i].P.z;
	}
	return sqrt(xmax * xmax + ymax * ymax + zmax * zmax);
}

void PIC::ReadPIC(std::string filename)
{
	int has_texture = 0;
	int buff;
	std::ifstream ifs(filename);
	std::string s;
	Face* f;
	Point* v;
	Normal* vn;
	while (getline(ifs, s)) {
		if (s.length() < 2) continue;

		if (s[0] == 'v') {
			if (s[1] == 'n') {
				std::istringstream in(s);
				vn = new Normal();
				std::string head;
				in >> head >> vn->N.x >> vn->N.y >> vn->N.z;
				vn->valid = true;
				VN.push_back(*vn);
			}
			else if (s[1] == 't')
				has_texture = 1;
			else {
				std::istringstream in(s);
				v = new Point();
				std::string head;
				in >> head >> v->P.x >> v->P.y >> v->P.z;
				v->valid = true;
				v->F = new std::vector<Face*>;
				v->F->reserve(20);
				v->F->clear();
				V.push_back(*v);
				int v_id = V.size() - 1;
				v = &V[v_id];
			}
		}
		else if (s[0] == 'f') {
			int k = s.size();
			for (int j = 0; j < k; j++) {
				if (s[j] == '/')
					s[j] = ' ';
			}

			std::istringstream in(s);
			f = new Face();
			std::string head;
			in >> head;
			int i = 0;
			while (i < 3) {
				if (V.size()) {
					in >> f->V[i];
					f->V[i]--;
				}
				if (has_texture)
					in >> buff;
				if (VN.size()) {
					in >> f->N[i];
					f->N[i]--;
				}
				i++;
			}
			f->valid = true;
			F.push_back(*f);
		}
	}
	for (int i = 0; i < F.size(); i++)
	{
		Face* f = &F[i];
		for (int j = 0; j < 3; j++)
			V[f->V[j]].F->push_back(f);
	}
	ifs.close();
	GetVertexNormal();
}

void PIC::WritePIC(std::string filename)
{
	std::ofstream ofs(filename);
	for (int i = 0; i < V.size(); i++)
	{
		ofs << "v " << V[i].P.x << " " << V[i].P.y << " " << V[i].P.z << std::endl;
	}
	for (int i = 0; i < VN.size(); i++)
	{
		ofs << "vn " << VN[i].N.x << " " << VN[i].N.y << " " << VN[i].N.z << std::endl;
	}
	for (int i = 0; i < F.size(); i++)
	{
		if (F[i].valid == true)
		{
			ofs << "f " << F[i].V[0] + 1 << "//" << F[i].N[0] + 1 << " "
				<< F[i].V[1] + 1 << "//" << F[i].N[1] + 1 << " "
				<< F[i].V[2] + 1 << "//" << F[i].N[2] + 1 << std::endl;
		}
	}
	ofs.close();
}

void PIC::GetVertexNormal()
{
	int vlength = V.size();
	int flength = F.size();
	std::vector<Normal> VN_old;
	VN_old.clear();
	VN_old.swap(VN);

	for (int i = 0; i < vlength; i++)
	{
		Vector3D p = V[i].P;
		Vector3D p1;
		Vector3D p2;
		Vector3D vn_old;
		Normal* vn = new Normal();
		vn->N.x = 0;
		vn->N.y = 0;
		vn->N.z = 0;

		for (int j = 0; j < V[i].F->size(); j++)
		{
			Face* f = (*V[i].F)[j];
			if (f->V[0] == i)
			{
				p1 = V[f->V[1]].P;
				p2 = V[f->V[2]].P;
				vn_old = VN_old[f->N[0]].N;
			}
			else if (f->V[1] == i)
			{
				p1 = V[f->V[0]].P;
				p2 = V[f->V[2]].P;
				vn_old = VN_old[f->N[1]].N;
			}
			else if (f->V[2] == i)
			{
				p1 = V[f->V[0]].P;
				p2 = V[f->V[1]].P;
				vn_old = VN_old[f->N[2]].N;
			}
			else
				continue;

			Vector3D e1 = p1 - p;
			Vector3D e2 = p2 - p;

			e1.normalization();
			e2.normalization();
			float angle = acos(e1 * e2);

			Vector3D fn;
			fn = e1 ^ e2;
			fn.normalization();
			fn.scalar(angle);
			if (fn * vn_old < 0)
				fn = -fn;

			vn->N = vn->N + fn;
		}
		vn->N.normalization();
		vn->valid = true;
		VN.push_back(*vn);
	}
	for (int i = 0; i < flength; i++)
	{
		F[i].N[0] = F[i].V[0];
		F[i].N[1] = F[i].V[1];
		F[i].N[2] = F[i].V[2];
	}
}

void PIC::GetFaceNormal()
{
	int i = 0;
	for (i = 0; i < F.size(); i++)
	{
		Face* f = &F[i];
		Point p0 = V[f->V[0]];
		Point p1 = V[f->V[1]];
		Point p2 = V[f->V[2]];

		Vector3D e1 = p0.P - p1.P;
		Vector3D e2 = p0.P - p2.P;
		f->FN = e1 ^ e2;
		if (f->FN * VN[f->N[0]].N < 0)
			f->FN = -f->FN;
	}
}

void PIC::GetFaceNormal(Face* f)
{
	Point p0 = V[f->V[0]];
	Point p1 = V[f->V[1]];
	Point p2 = V[f->V[2]];

	Vector3D e1 = p0.P - p1.P;
	Vector3D e2 = p0.P - p2.P;
	Vector3D fn_old = f->FN;
	f->FN = e1 ^ e2;
	if (f->FN * fn_old < 0)
		f->FN = -f->FN;
}

void PIC::GetFaceNormal(int i)
{
	Face* f = &F[i];
	Point p0 = V[f->V[0]];
	Point p1 = V[f->V[1]];
	Point p2 = V[f->V[2]];

	Vector3D e1 = p0.P - p1.P;
	Vector3D e2 = p0.P - p2.P;
	Vector3D fn_old = f->FN;
	f->FN = e1 ^ e2;
	if (f->FN * fn_old < 0)
		f->FN = -f->FN;
}