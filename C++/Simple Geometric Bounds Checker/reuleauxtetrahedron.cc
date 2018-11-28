//Justin Pimentel
//jfpiment@ucsc.edu
//1559599

#include <iostream>

#include "reuleauxtriangle.h"
#include "cube.h"
#include "sphere.h"

ReuleauxTetrahedron::ReuleauxTetrahedron(Point3D vertices[4]){
        //std::copy(std::begin(vertices), std::end(vertices), std::begin(vertices_));
        //std::memcpy(vertices_, vertices, sizeof(vertices[3]));
        for(int i = 0; i < 4; i++){
                vertices_[i] = vertices[i];
        }
}

std::array<Point3D,4> ReuleauxTetrahedron::vertices(){
        return vertices_;
}

/*double ReuleauxTetrahedron::getRadius(){
        double dist = Geom::distance(vertices_[0], vertices_[1]);
        return dist;
}*/

bool ReuleauxTetrahedron::containedWithin(Sphere &sphere){
	return true;
}

bool ReuleauxTetrahedron::containedWithin(Cube &cube){
	return true;
}

bool ReuleauxTetrahedron::containedWithin(ReuleauxTetrahedron &rt){
	return true;
}
