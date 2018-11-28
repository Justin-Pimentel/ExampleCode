/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include <iostream>

#include "polygon.h"
#include "geom.h"
#include "reuleauxtriangle.h"

RegularConvexPolygon::RegularConvexPolygon(std::vector<Point2D> vertices) {
    vertices_ = vertices;
}

std::vector<Point2D> RegularConvexPolygon::vertices() {
    return vertices_;
}

std::vector<Line> RegularConvexPolygon::edges() {
    std::vector<Line> edges;
    for (unsigned int i = 0; i < vertices_.size()-1; i++) 
        edges.push_back(Line(vertices_[i],vertices_[i+1]));
    edges.push_back(Line(vertices_[vertices_.size()-1],vertices_[0]));
    return edges;
}

int RegularConvexPolygon::vertexCount() {
    return vertices_.size();
}

//If any of the vertices are outside the circle 
bool RegularConvexPolygon::containedWithin(Circle &circle){
	//Check if any vertices are farther than the radius of the circle
	for(Point2D const &vertex: vertices()){
		if(Geom::distance(vertex, circle.center()) > circle.radius()){
			//std::cout << Geom::distance(vertex, circle.center());
			return false;
		}
	}
	//If not return true
	return true;
}

//If any of the polygon's edges intersect then return false
//If one of the outer polygon's edges intersect the line
//then the polygon is not contained
//If neither case, it is contained
bool RegularConvexPolygon::containedWithin(RegularConvexPolygon &polygon){
	for(Line const &edgeOut: polygon.edges()){
		for(Line const &edgeIn: edges()){
        		if(Geom::intersects(edgeOut, edgeIn)){
            			return false;
			}
		}
    	}
	
	Line lineBetweenCenters = Line(Geom::center(polygon), Geom::center(*this));

	for(Line const &edgeCheck:polygon.edges()){
		if(Geom::intersects(edgeCheck, lineBetweenCenters)){
			return false;
		}
	}

	return true;
}

//First checks to see if the any of the lines in the polygon intersect any of
//the circles made by the RT vertices. If there's an intersection then it is
//not entirely contained. 
//That leaves if it is entirely enclosed with no intersections and entirely
//outside with no intersections. 
//To find if it is outside then find the distance from the center of the RT
//and the center of the polygon. If it is farther than the radius/2 from the center
//then it must be outside the RT
bool RegularConvexPolygon::containedWithin(ReuleauxTriangle &rt) {
	std::array<Point2D,3> vertices = rt.vertices();
	double rtRad = rt.getRadius();

	for(int i = 0; i < 3; i++){
		Circle currCirc = Circle(vertices[i], rtRad);	
		for(Line const &edge: edges()){
			if(Geom::intersects(edge, currCirc)){
				return false;
			}
		}
	}

	//Create a RegularConvexPolygon with the RT vertices
	//so we can use the center function for polygons
	RegularConvexPolygon rtPoly = 
		RegularConvexPolygon({vertices[0], vertices[1], vertices[2]});

	//Find the centers of the polygon and the RT poly
	Point2D rtCenter = Geom::center(rtPoly);
	Point2D polyCenter = Geom::center(*this);

	//Check if the distance from the centers is greater than the distance
	//from the center of the RT half the radius of the RT circles
	if(Geom::distance(rtCenter, polyCenter) > rtRad/2){
		return false;
	}

	//If there are no intersections and it is not further than half
	//the radius from the center then it must be enclosed by the RT
	return true;
}
