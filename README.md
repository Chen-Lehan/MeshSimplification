# MeshSimplification
This is a mesh simplification algorithm by shrinking faces.  
  
The input file, the temperate file and the output file can be changed only by changing the macro definition of INPUT_FILE, TEMP_FILE, OUTPUT_FILE in MeshSimplification.cpp.  
  
There may still exist some bugs which may cause errors about the heap, but most of them can be avoided by simply reducing the macro definition of CUTOFF in MeshSimplification.cpp.  
  
The vertex normals are required in this project. To those meshes whose vertex normals don't exist, one possible way is to run the following code to get the vertex normals.  
```C++
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Handles.hh>
typedef OpenMesh::TriMesh_ArrayKernelT <> Mesh;

int main()
{
	Mesh mesh;
	OpenMesh::IO::read_mesh(mesh, INPUT_FILE);
	mesh.request_face_normals();
	mesh.update_face_normals();
	mesh.request_vertex_normals();
	mesh.update_vertex_normals();
	OpenMesh::IO::Options wop = OpenMesh::IO::Options::VertexNormal;
	OpenMesh::IO::write_mesh(mesh, OUTPUT_FILE, wop);
	return 0;
}
```
