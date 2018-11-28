//Justin Pimentel
//jfpiment@ucsc.edu
//1559599

#include <iostream>
#include <array>

#include "polygon.h"
#include "geom.h"
#include "sphere.h"
#include "reuleauxtetrahedron.h"

Cube::Cube(Point3D upperFace[4], Point3D lowerFace[4]) {
	for(int i = 0; i < 4; i++){
		upperFace_[i] = upperFace[i];
		lowerFace_[i] = lowerFace[i];
	}
}

std::array<Point3D,4> Cube::upperV(){
	return upperFace_;
}

std::array<Point3D,4> Cube::lowerV(){
	return lowerFace_;
}

RegularConvexPolygon Cube::decomposeCube(std::array<Point3D,4> vert){
	std::vector<Point2D> sqV;
	Point2D tP;
	
	for(int i = 0; i < 4; i++){
		tP.x = vert[i].x;
		tP.y = vert[i].y;
		sqV.push_back(tP);
	}

	RegularConvexPolygon rcp = RegularConvexPolygon(sqV);
	
	return rcp;
}

bool Cube::containedWithin(Sphere &sphere){
	std::array<Point3D,4> upper = upperV();
        std::array<Point3D,4> lower = lowerV();
        std::array<Point3D,4> sq;
	Point3D p = sphere.center();

        //First face of cube
        for(int i = 0; i < 2; i++){
                sq[i] = upper[i];
                sq[i+2] = lower[i];
        }
        Circle outer = sphere.decomposeSphere(p.x, p.y, sphere.radius());
        RegularConvexPolygon inner = decomposeCube(sq);

        if(!inner.containedWithin(outer)){
                return false;
        }

        //Second face of cube
        for(int i = 0; i < 2; i++){
                sq[i] = upper[i+1];
                sq[i+2] = lower[i+1];
        }
        outer = sphere.decomposeSphere(p.y, p.z, sphere.radius());
        inner = decomposeCube(sq);

        if(!inner.containedWithin(outer)){
                return false;
        }

        //Top face of cube
        outer = sphere.decomposeSphere(p.x, p.z, sphere.radius());
        inner = decomposeCube(upper);

        if(!inner.containedWithin(outer)){
                return false;
        }

        return true;
}

bool Cube::containedWithin(Cube &cube){
	std::array<Point3D,4> upperIn = upperV();
        std::array<Point3D,4> lowerIn = lowerV();

	std::array<Point3D,4> upperOut = cube.upperV();
        std::array<Point3D,4> lowerOut = cube.lowerV();

        std::array<Point3D,4> sqInner;
	std::array<Point3D,4> sqOuter;

        //First face of cube
        for(int i = 0; i < 2; i++){
                sqInner[i] = upperIn[i];
                sqInner[i+2] = lowerIn[i];
        }

	for(int i = 0; i < 2; i++){
		sqOuter[i] = upperOut[i];
                sqOuter[i+2] = lowerOut[i];
	}

	RegularConvexPolygon inner = decomposeCube(sqInner);
	RegularConvexPolygon outer = decomposeCube(sqOuter);

	if(!inner.containedWithin(outer)){
		return false;
	}

	for(int i = 0; i < 2; i++){
                sqInner[i] = upperIn[i+1];
                sqInner[i+2] = lowerIn[i+1];
        }

	for(int i = 0; i < 2; i++){
                sqOuter[i] = upperOut[i+1];
                sqOuter[i+2] = lowerOut[i+1];
        }

	inner = decomposeCube(sqInner);
	outer = decomposeCube(sqOuter);

	if(!inner.containedWithin(outer)){
		return false;
	}

	inner = decomposeCube(upperIn);
	outer = decomposeCube(upperOut);

	if(!inner.containedWithin(outer)){
		return false;
	}

	return true;
}

bool Cube::containedWithin(ReuleauxTetrahedron &rt){
	return true;
}
