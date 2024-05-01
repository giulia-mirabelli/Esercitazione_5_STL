#pragma once
#include<iostream>
#include"PolygonalMesh.hpp"
using namespace std;

namespace PolygonalLibrary {

bool Import_Mesh(const string &path, PolygonalMesh& mesh);

bool ImportCell0D(const string &path, PolygonalMesh& mesh);

bool ImportCell1D(const string &path, PolygonalMesh& mesh);

bool ImportCell2D(const string &path, PolygonalMesh& mesh);

}
