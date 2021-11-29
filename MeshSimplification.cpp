#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include "myHeap.h"
#include "myModel.h"

#define INPUT_FILE "./dragon.obj"
#define OUTPUT_FILE "./output.obj"
#define TEMP_FILE "./temp.obj"
#define CUTOFF 500

void GetScore(PIC* pic, Heap* heap, HeapNode** array)
{
	for (int i = 0; i < pic->F.size(); i++)
	{
		Face* f = &pic->F[i];
		Face* f0 = NULL;
		Face* f1 = NULL;
		Face* f2 = NULL;
		int p0_id = f->V[0];
		int p1_id = f->V[1];
		int p2_id = f->V[2];
		Point* p0 = &pic->V[f->V[0]];
		Point* p1 = &pic->V[f->V[1]];
		Point* p2 = &pic->V[f->V[2]];

		// find neighbor faces
		int j;
		for (j = 0; j < p0->F->size(); j++)
		{
			if ((*p0->F)[j] == f)
				continue;
			if ((*p0->F)[j]->V[0] == p1_id || (*p0->F)[j]->V[1] == p1_id || (*p0->F)[j]->V[2] == p1_id)
			{
				f0 = (*p0->F)[j];
				break;
			}
		}
		for (j = 0; j < p1->F->size(); j++)
		{
			if ((*p1->F)[j] == f)
				continue;
			if ((*p1->F)[j]->V[0] == p2_id || (*p1->F)[j]->V[1] == p2_id || (*p1->F)[j]->V[2] == p2_id)
			{
				f1 = (*p1->F)[j];
				break;
			}
		}
		for (j = 0; j < p2->F->size(); j++)
		{
			if ((*p2->F)[j] == f)
				continue;
			if ((*p2->F)[j]->V[0] == p0_id || (*p2->F)[j]->V[1] == p0_id || (*p2->F)[j]->V[2] == p0_id)
			{
				f2 = (*p2->F)[j];
				break;
			}
		}

		// calculate the score
		if (f0 && f1 && f2)
		{
			double data = 0.0;
			double length = f->FN.length();
			data += (length - (f->FN * f0->FN / f0->FN.length())) / f0->FN.length();
			data += (length - (f->FN * f1->FN / f1->FN.length())) / f1->FN.length();
			data += (length - (f->FN * f2->FN / f2->FN.length())) / f2->FN.length();
			array[i] = PushHeap(heap, data, i);
		}
		else
			array[i] = NULL;
	}
}

void GetFaceScore(PIC* pic, Face* f, HeapNode* node)
{	
	Face* f0 = NULL;
	Face* f1 = NULL;
	Face* f2 = NULL;
	int p0_id = f->V[0];
	int p1_id = f->V[1];
	int p2_id = f->V[2];
	Point* p0 = &pic->V[f->V[0]];
	Point* p1 = &pic->V[f->V[1]];
	Point* p2 = &pic->V[f->V[2]];

	// find neighbor faces
	int j;
	for (j = 0; j < p0->F->size(); j++)
	{
		if ((*p0->F)[j]->V[0] == p1_id || (*p0->F)[j]->V[1] == p1_id || (*p0->F)[j]->V[2] == p1_id)
		{
			if ((*p0->F)[j] == f)
				continue;
			f0 = (*p0->F)[j];
			break;
		}
	}
	for (j = 0; j < p1->F->size(); j++)
	{
		if ((*p1->F)[j]->V[0] == p2_id || (*p1->F)[j]->V[1] == p2_id || (*p1->F)[j]->V[2] == p2_id)
		{
			if ((*p1->F)[j] == f)
				continue;
			f1 = (*p1->F)[j];
			break;
		}
	}
	for (j = 0; j < p2->F->size(); j++)
	{
		if ((*p2->F)[j]->V[0] == p0_id || (*p2->F)[j]->V[1] == p0_id || (*p2->F)[j]->V[2] == p0_id)
		{
			if ((*p2->F)[j] == f)
				continue;
			f2 = (*p2->F)[j];
			break;
		}
	}

	// update the score
	if (f0 && f1 && f2)
	{
		double data = 0.0;
		double length = f->FN.length();
		data += (length - (f->FN * f0->FN / f0->FN.length())) / f0->FN.length();
		data += (length - (f->FN * f1->FN / f1->FN.length())) / f1->FN.length();
		data += (length - (f->FN * f2->FN / f2->FN.length())) / f2->FN.length();
		node->data = data;
		InitHeapNode(node);
	}
}

void MeshSimplify(Heap* heap, PIC* pic, int cutoff, HeapNode** array)
{
	while (cutoff)
	{
		HeapNode* head = PopHeap(heap);
		int id = head->id;
		if (head->valid == true)
		{
			// find the shrinking face
			Face* f = &pic->F[head->id];
			if (f->valid == false)
				continue;
			Point* p0 = &pic->V[f->V[0]];
			Point* p1 = &pic->V[f->V[1]];
			Point* p2 = &pic->V[f->V[2]];
			
			// find the neighbor faces
			Face* f0 = NULL;
			Face* f1 = NULL;
			Face* f2 = NULL;
			int p0_id = f->V[0];
			int p1_id = f->V[1];
			int p2_id = f->V[2];
			for (int i = 0; i < p0->F->size(); i++)
			{
				if ((*p0->F)[i]->V[0] == p1_id || (*p0->F)[i]->V[1] == p1_id || (*p0->F)[i]->V[2] == p1_id)
				{
					if ((*p0->F)[i] != f)
					{
						f0 = (*p0->F)[i];
						break;
					}
				}
			}
			for (int i = 0; i < p1->F->size(); i++)
			{
				if ((*p1->F)[i]->V[0] == p2_id || (*p1->F)[i]->V[1] == p2_id || (*p1->F)[i]->V[2] == p2_id)
				{
					if ((*p1->F)[i] != f)
					{
						f1 = (*p1->F)[i];
						break;
					}
				}
			}
			for (int i = 0; i < p2->F->size(); i++)
			{
				if ((*p2->F)[i]->V[0] == p0_id || (*p2->F)[i]->V[1] == p0_id || (*p2->F)[i]->V[2] == p0_id)
				{
					if ((*p2->F)[i] != f)
					{
						f2 = (*p2->F)[i];
						break;
					}
				}
			}
			if (!(f0 && f1 && f2))
				break;

			// add the new point
			Point* np = new Point;
			np->P = (p0->P + p1->P + p2->P) / 3;
			np->valid = true;
			np->F = new std::vector<Face*>;
			np->F->clear();
			pic->V.push_back(*np);			// NOTICE: this may change the value of p0, p1 and p2 !!!
			int np_id = pic->V.size() - 1;
			np = &pic->V[np_id];

			p0 = &pic->V[f->V[0]];
			p1 = &pic->V[f->V[1]];
			p2 = &pic->V[f->V[2]];

			// replace the point and update the face normal
			int nfnum = p0->F->size() + p1->F->size() + p2->F->size() - 9;
			Face** rpfarray = new Face*[nfnum];
			if (nfnum)
			{
				int id = 0;
				for (int i = 0; i < p0->F->size(); i++)
				{
					Face* rpf = (*p0->F)[i];
					if (rpf != f && rpf != f0 && rpf != f1 && rpf != f2)
					{
						for (int j = 0; j < 3; j++)
						{
							if (rpf->V[j] == p0_id)
							{
								rpf->V[j] = np_id;
								rpf->N[j] = np_id;
								np->F->push_back(rpf);
								break;
							}
						}
						rpfarray[id++] = rpf;
						pic->GetFaceNormal(rpf);
					}
				}
				for (int i = 0; i < p1->F->size(); i++)
				{
					Face* rpf = (*p1->F)[i];
					if (rpf != f && rpf != f0 && rpf != f1 && rpf != f2)
					{
						for (int j = 0; j < 3; j++)
						{
							if (rpf->V[j] == p1_id)
							{
								rpf->V[j] = np_id;
								rpf->N[j] = np_id;
								np->F->push_back(rpf);
								break;
							}
						}
						rpfarray[id++] = rpf;
						pic->GetFaceNormal(rpf);
					}
				}
				for (int i = 0; i < p2->F->size(); i++)
				{
					Face* rpf = (*p2->F)[i];
					if (rpf != f && rpf != f0 && rpf != f1 && rpf != f2)
					{
						for (int j = 0; j < 3; j++)
						{
							if (rpf->V[j] == p2_id)
							{
								rpf->V[j] = np_id;
								rpf->N[j] = np_id;
								np->F->push_back(rpf);
								break;
							}
						}
						rpfarray[id++] = rpf;
						pic->GetFaceNormal(rpf);
					}
				}
				
			}

			// check the point to avoid a same face in a model twice
			for (int i = 0; i < nfnum - 1; i++)
			{
				for (int j = i + 1; j < nfnum; j++)
				{
					if (*rpfarray[i] == *rpfarray[j])
						rpfarray[i]->valid = rpfarray[j]->valid = false;
				}
			}
			
			// add the new point normal
			Vector3D np1, np2;
			Normal* vn = new Normal;
			vn->N.SetVector3D(0.0, 0.0, 0.0);
			for (int i = 0; i < np->F->size(); i++)
			{
				Face* f = (*np->F)[i];
				if (f->V[0] == np_id)
				{
					np1 = pic->V[f->V[1]].P;
					np2 = pic->V[f->V[2]].P;
				}
				else if (f->V[1] == np_id)
				{
					np1 = pic->V[f->V[0]].P;
					np2 = pic->V[f->V[2]].P;
				}
				else if (f->V[2] == np_id)
				{
					np1 = pic->V[f->V[0]].P;
					np2 = pic->V[f->V[1]].P;
				}

				Vector3D e1 = np1 - np->P;
				Vector3D e2 = np2 - np->P;

				e1.normalization();
				e2.normalization();
				float angle = acos(e1 * e2);

				Vector3D fn;
				fn = e1 ^ e2;
				fn.normalization();
				fn.scalar(angle);
				if (fn * f->FN < 0)
					fn = -fn;

				vn->N = vn->N + fn;
			}
			vn->N.normalization();
			vn->valid = true;
			pic->VN.push_back(*vn);
			delete vn;

			// delete vertex and face
			p0->valid = false;
			p1->valid = false;
			p2->valid = false;
			f->valid = false;
			f0->valid = false;
			f1->valid = false;
			f2->valid = false;
			array[head->id] = NULL;

			// update face score
			for (int i = 0; i < np->F->size(); i++)
			{
				Face* f = (*np->F)[i];
				pic->GetFaceNormal(f);
				int f_id = 0;
				for (f_id = 0; f_id < pic->F.size(); f_id++)
				{
					if (&pic->F[f_id] == f)
						break;
				}
				if (f_id == pic->F.size())
						std::cout << "???" << std::endl;
				if (array[f_id])
				{
					if (pic->F[f_id].valid == true)
						GetFaceScore(pic, f, array[f_id]);
				}
			}
		}
		UpdateHeapHead(heap);
		delete head;
		cutoff--;
	}
}

int main()
{
	int cutoff = CUTOFF;
	int start = 1;
	while (cutoff > 0)
	{
		PIC* m_pic = new PIC;
		if (start != 0)
		{
			start = 0;
			m_pic->ReadPIC(INPUT_FILE);
		}
		else 
			m_pic->ReadPIC(TEMP_FILE);
		m_pic->GetFaceNormal();
		Heap* sheap = new Heap;
		InitHeap(sheap);
		HeapNode** nodearray = new HeapNode * [m_pic->F.size()];
		GetScore(m_pic, sheap, nodearray);
		int _cutoff = 10;
		if (cutoff <= _cutoff)
			MeshSimplify(sheap, m_pic, cutoff, nodearray);
		else
			MeshSimplify(sheap, m_pic, _cutoff, nodearray);

		if (cutoff <= _cutoff)
			m_pic->WritePIC(OUTPUT_FILE);
		else
			m_pic->WritePIC(TEMP_FILE);

		cutoff -= _cutoff;
		delete m_pic;
		delete sheap;
	}
	return 0;
}

