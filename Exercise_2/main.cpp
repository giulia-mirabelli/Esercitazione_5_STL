#include "Utils.hpp"
#include"PolygonalMesh.hpp"
#include <iostream>
#include "Eigen/Eigen"
using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;


int main()
{

    PolygonalMesh mesh;

    const string path = "PolygonalMesh";
    if(!Import_Mesh(path,mesh))
    {
        return 4;
    }



    //cout<<mesh.NumberOfCell2Ds<<endl;


    return 0;
}
