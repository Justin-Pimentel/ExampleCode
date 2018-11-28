//Implementation file for Reuleaux Triangle
//Justin Pimentel
//jfpiment@ucsc.edu
//1559599

#include <iostream>
#include <cstring>

#include "reuleauxtriangle.h"
#include "geom.h"

ReuleauxTriangle::ReuleauxTriangle(Point2D vertices[3]){
	//std::copy(std::begin(vertices), std::end(vertices), std::begin(vertices_));
	//std::memcpy(vertices_, vertices, sizeof(vertices[3]));
	for(int i = 0; i < 3; i++){
		vertices_[i] = vertices[i];
	}
}

std::array<Point2D,3> ReuleauxTriangle::vertices(){
	return vertices_;
}

std::vector<Line> ReuleauxTriangle::edges(){
	std::vector<Line> edges;
	for(unsigned int i = 0; i < 2; i++){
		edges.push_back(Line(vertices_[i],vertices_[i+1]));
	}
	
	edges.push_back(Line(vertices_[3],vertices_[0]));
    
	return edges;
}

double ReuleauxTriangle::getRadius(){
	double dist = Geom::distance(vertices_[0], vertices_[1]);
	return dist;
}

//If any of the RT vertices are outside the triangle
//then it not entirely contained
bool ReuleauxTriangle::containedWithin(Circle &circle){
	for(int i = 0; i < 3; i++){
		//std::cout << vertices_[i] << std::endl;
		if(Geom::distance(vertices_[i], circle.center()) > circle.radius()){
			return false;
		}
	}
	return true;
}

//First check for any intersections between any of the circles in RT 
//and the line created from each vertex of the polygon to the center
//of the RT. If there are any intersections between that line and any
//circle then the RT is either fully contained or outside the polygon 
//If no intersection is detected then the polygon vertex is inside
//the RT and that means it is outside the polygon.
bool ReuleauxTriangle::containedWithin(RegularConvexPolygon &polygon){
        //Create polygon version of RT to get center
	RegularConvexPolygon innerRT =
		RegularConvexPolygon({vertices_[0], vertices_[1], vertices_[2]});
	
	//Get the center of the arena and the center of the inner RT
	Point2D innerRTCenter = Geom::center(innerRT);
	Point2D outerPCenter = Geom::center(polygon);
	
	Circle circ1 = Circle(vertices_[0], getRadius());
	Circle circ2 = Circle(vertices_[1], getRadius());
	Circle circ3 = Circle(vertices_[2], getRadius());

	for(Line const &edge: polygon.edges()){
		//Current line from the polygon vertex to the center of RT
		if(Geom::intersects(edge, circ1) 
		&& Geom::intersects(edge, circ2) 
		&& Geom::intersects(edge, circ3)){
			return false; 
		}
	}

	//If no intersection was found, then it is either fully inside
	//or fully outside the polygon
	//Check to see if it is outside the polygon
	std::vector<Point2D> vert = polygon.vertices();
	double distFromV = Geom::distance(vert[0], outerPCenter);
	double distFromRT = Geom::distance(innerRTCenter, outerPCenter);

	//If the distance from the center of the polygon to one of its
	//vertices is less than the distance from the polygon center to
	//the RT center that must mean that is fully outside the polygon
	//therefore return false
	if(distFromV < distFromRT){
		return false;
	}
	
	//Else it is fully enclosed, return true
	return true;	
}

//Similar to polygons, check if any of the edges of the inner RT intersect any
//of the circles of the outer RT. If it does then it is not fully contained.
//The only other two cases are if it's fully contained or fully outside the circle.
bool ReuleauxTriangle::containedWithin(ReuleauxTriangle &rt){
	std::array<Point2D,3> vertices = rt.vertices();
	double rtRad = rt.getRadius();
	
        for(int i = 0; i < 3; i++){
                Circle currCirc = Circle(vertices[i], rtRad);
                for(Line const &edge: rt.edges()){
                        if(Geom::intersects(edge, currCirc)){
                                return false;
                        }
                }
        }

	//Create the polygon versions of the RTs	
	RegularConvexPolygon outerRTPoly =
                RegularConvexPolygon({vertices[0], vertices[1], vertices[2]});

	RegularConvexPolygon innerRTPoly =
                RegularConvexPolygon({vertices_[0], vertices_[1], vertices_[2]});

	//Find the centers of both polygons
	Point2D outerCenter = Geom::center(outerRTPoly);
        Point2D innerCenter = Geom::center(innerRTPoly);

	//If the distance from centers is greater than the radius from the center
	//then it is outside the RT
	if(Geom::distance(innerCenter, outerCenter) > rtRad/2){
                return false;
        }

	//If it neither intersects nor is outside then it must be contained
	return true;
}
