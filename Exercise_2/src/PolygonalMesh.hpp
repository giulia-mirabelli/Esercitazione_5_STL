#pragma once
#include<vector>
#include<iostream>
#include<Eigen/Eigen>
using namespace Eigen;
using namespace std;

namespace PolygonalLibrary{

struct PolygonalMesh{


    //Cell0D
    unsigned int NumberOfCell0Ds = 0;
    vector<unsigned int> IdCell0Ds = {};
    vector<Vector2d> CoordinatesCell0Ds;
    map<unsigned int, list<unsigned int> > VerticesMarker;

    //Cell1D
    unsigned int NumberOfCell1Ds = 0;
    vector<unsigned int> IdCell1Ds = {};
    vector<Vector2i> VerticesCell1Ds={};
    vector<unsigned int> MarkerCell1Ds = {};
    map<unsigned int, list<unsigned int> > EdgesMarker;

    //Cell2D
    unsigned int NumberOfCell2Ds = 0;
    vector<unsigned int> IdCell2Ds = {};
    vector<unsigned int> NumVertices = {};
    vector<VectorXi> VerticesCell2Ds={};
    vector<unsigned int>  NumEdges ={};
    vector<VectorXi> EdgesCell2Ds={};

};
}


