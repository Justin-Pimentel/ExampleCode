//Justin Pimentel
//jfpiment@ucsc.edu
//1559599

#include <iostream>

#include "circle.h"
#include "sphere.h"
#include "cube.h"

Sphere::Sphere(Point3D center, double radius) {
    setCenter(center);
    setRadius(radius);
}

Point3D Sphere::center() {
    return center_;
}

Circle Sphere::decomposeSphere(double c1, double c2, double rad){
	Point2D cent;
	cent.x = c1;
	cent.y = c2;

	Circle c = Circle(cent, rad);

	return c;
}

void Sphere::setCenter(const Point3D &center) {
    center_.x = center.x;
    center_.y = center.y;
    center_.z = center.z;
}

double Sphere::radius() {
    return radius_;
}

void Sphere::setRadius(double radius) {
    radius_ = radius;
}

/*
 * If distance between centers is less than difference between the radius of containing 
 * circle and the radius of this circle, this circle is contained
 */
bool Sphere::containedWithin(Sphere &sphere){
    //return Geom::distance(center(), circle.center()) <= (circle.radius() - radius());
	Point3D sphereCent = sphere.center();
	Circle inner = decomposeSphere(center_.x, center_.y, radius());
	Circle outer = decomposeSphere(sphereCent.x, sphereCent.y, sphere.radius());

	if(!inner.containedWithin(outer)){
		return false;
	}

	inner = decomposeSphere(center_.y, center_.z, radius());
	outer = decomposeSphere(sphereCent.y, sphereCent.z, sphere.radius());

	if(!inner.containedWithin(outer)){
		return false;
	}

	inner = decomposeSphere(center_.x, center_.z, radius());
        outer = decomposeSphere(sphereCent.x, sphereCent.z, sphere.radius());
	
	if(!inner.containedWithin(outer)){
		return false;
	}

	return true;
}

/*
 * If circle intersects with any polygon edge, circle is not contained       
 * If line between centers intersects with any polygon edge, circle is not contained
 * Only other possibility is that the circle is contained
 */
bool Sphere::containedWithin(Cube &cube){
	std::array<Point3D,4> upper = cube.upperV();
	std::array<Point3D,4> lower = cube.lowerV();
	std::array<Point3D,4> sq;

	//First face of cube
	for(int i = 0; i < 2; i++){
		sq[i] = upper[i];
		sq[i+2] = lower[i];
	}
	Circle inner = decomposeSphere(center_.x, center_.y, radius());
	RegularConvexPolygon outer = cube.decomposeCube(sq);

	if(!inner.containedWithin(outer)){
		return false;
	}

	//Second face of cube
	for(int i = 0; i < 2; i++){
		sq[i] = upper[i+1];
		sq[i+2] = lower[i+1];
        }
        inner = decomposeSphere(center_.y, center_.z, radius());
        outer = cube.decomposeCube(sq);

	if(!inner.containedWithin(outer)){
		return false;
	}

	//Top face of cube
        inner = decomposeSphere(center_.x, center_.z, radius());
        outer = cube.decomposeCube(upper);

        if(!inner.containedWithin(outer)){
                return false;
        }

	return true;
}

//If the distance from the center of any of the circles created by the vertices plus the radius
//of the contained circle is greater than the radius of the circles created by the RT then the 
//circle is partially outside or completely outside the RT
bool Sphere::containedWithin(ReuleauxTetrahedron &rt) {
        //Get the radius of the circles from the RT
        /*double rtRadius = rt.getRadius();

        std::array<Point2D,3> vertices = rt.vertices();

        for(int i = 0; i < 3; i++){
                if(Geom::distance(vertices[i], center()) + radius() > rtRadius){
                        return false;
                }
        }*/

        return true;
}
