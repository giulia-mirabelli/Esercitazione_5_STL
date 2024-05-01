#include "Utils.hpp"
#include <iostream>
#include<fstream>
#include<sstream>
#include<cmath>
#include<iomanip>
#include <Eigen/Eigen>
using namespace std;
using namespace Eigen;

namespace PolygonalLibrary{

bool Import_Mesh(const string& path, PolygonalMesh& mesh)
{
    if (!ImportCell0D(path + "/Cell0Ds.csv",mesh))
    { return 1;}
    else
    {
        cout << "Cell0D marker: "<<endl;
        for (auto it = mesh.VerticesMarker.begin(); it!=mesh.VerticesMarker.end(); it++)
        {
            cout << "key:\t"<<it->first <<"\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;
            cout << endl;
        }
    }


    if (!ImportCell1D(path + "/Cell1Ds.csv",mesh))
    {return 2;}

    else
    {
        cout << "/Cell1D marker:" << endl;
        for(auto it = mesh.EdgesMarker.begin(); it != mesh.EdgesMarker.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }



    if (!ImportCell2D(path + "/Cell2Ds.csv",mesh))
        {return 5;}



    return 0;
}






bool ImportCell0D(const string &path, PolygonalMesh& mesh)
{

    ifstream file;
    file.open(path);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumberOfCell0Ds = listLines.size();

    if (mesh.NumberOfCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.IdCell0Ds.reserve(mesh.NumberOfCell0Ds);
    mesh.CoordinatesCell0Ds.reserve(mesh.NumberOfCell0Ds);

    for (const string& line : listLines)
    {
        istringstream convert(line);

        unsigned int id;
        char c;             // to store ';'
        unsigned int marker;
        Vector2d coord;

        convert >> id >>c >> marker >>c >> coord(0) >> c >> coord(1);

        mesh.IdCell0Ds.push_back(id);
        mesh.CoordinatesCell0Ds.push_back(coord);


        if(marker !=0)
        {
            auto ret = mesh.VerticesMarker.insert({marker,{id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }
        return true;

}



bool ImportCell1D(const string &path, PolygonalMesh& mesh)
{

    ifstream file;
    file.open(path);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumberOfCell1Ds = listLines.size();

    if (mesh.NumberOfCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.IdCell1Ds.reserve(mesh.NumberOfCell1Ds);
    mesh.VerticesCell1Ds.reserve(mesh.NumberOfCell1Ds);

    for (const string& line : listLines)
    {
        istringstream convert(line);

        unsigned int id;
        char d;
        unsigned int marker;
        Vector2i vert;

        convert >> id >>d >> marker >> d >> vert(0) >> d >> vert(1);

        mesh.IdCell1Ds.push_back(id);
        mesh.VerticesCell1Ds.push_back(vert);

        if(marker !=0)
        {
            auto ret = mesh.EdgesMarker.insert({marker,{id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }


        Vector2d coo1 = mesh.CoordinatesCell0Ds[vert(0)];
        Vector2d coo2 = mesh.CoordinatesCell0Ds[vert(1)];

        double edges_length = sqrt(pow((coo1(0)-coo2(0)),2) + pow((coo1(1)-coo2(1)),2));

        if (edges_length == 0)
        {
            cerr << "Error: edge length is zero"<<endl;
            return -1;
        }

    }
    return true;
}

bool ImportCell2D(const string &path, PolygonalMesh& mesh)
{

    ifstream file;
    file.open(path);

    if(file.fail())
        return false;


    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumberOfCell2Ds = listLines.size();

    if (mesh.NumberOfCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.IdCell2Ds.reserve(mesh.NumberOfCell2Ds);
    mesh.VerticesCell1Ds.reserve(mesh.NumberOfCell2Ds);
    mesh.EdgesCell2Ds.reserve(mesh.NumberOfCell2Ds);

    for (const string& line : listLines)
    {
        istringstream convert(line);

        unsigned int id, marker;
        char e;
        unsigned int numVert, numEdg;
        string rest1,rest2;

        convert >> id >> e >> marker >> e >> numVert>>e>>rest1;

        istringstream c1(rest1);

        VectorXi vert(numVert);

        for (unsigned int i=0; i<numVert; i++)
        {
            c1 >> vert[i] >>e;
        }
        c1>>numEdg>>e;

        VectorXi edges(numEdg);
        for (unsigned int i=0; i<numEdg; i++)
        {
            c1 >> edges[i]>>e;
        }

        mesh.IdCell2Ds.push_back(id);
        mesh.VerticesCell2Ds.push_back(vert);
        mesh.NumVertices.push_back(numVert);
        mesh.NumEdges.push_back(numEdg);
        mesh.EdgesCell2Ds.push_back(edges);

    }

    double area=0, sum=0;

    for (unsigned int i=0; i<mesh.NumberOfCell2Ds; i++)
    {
        VectorXi ids;
        ids= mesh.VerticesCell2Ds[i];

        for (unsigned int j=1; j<mesh.NumVertices[i];j++)
        {
            Vector2d p,q;
            double xp,yp,xq,yq;
            p=mesh.CoordinatesCell0Ds[ids(j-1)];
            q=mesh.CoordinatesCell0Ds[ids(j)];
            xp=p(0);
            yp=p(1);
            xq=q(0);
            yq=q(1);

            sum = sum+ (xp*yq - xq*yp);
        }

        area = 0.5*abs(sum);

        if(area ==0)
        {
            cerr << "Error: area is zero"<<endl;
            return -1;
        }
    }
    return true;
}

}
