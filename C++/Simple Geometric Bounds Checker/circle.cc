/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include <iostream>

#include "circle.h"
#include "polygon.h"
#include "geom.h"
#include "reuleauxtriangle.h"

Circle::Circle(const Point2D &center, double radius) {
    setCenter(center);
    setRadius(radius);
}

Point2D Circle::center() {
    return center_;
}

void Circle::setCenter(const Point2D &center) {
    center_.x = center.x;
    center_.y = center.y;
}

double Circle::radius() {
    return radius_;
}

void Circle::setRadius(double radius) {
    radius_ = radius;
}

/*
 * If distance between centers is less than difference between the radius of containing 
 * circle and the radius of this circle, this circle is contained
 */
bool Circle::containedWithin(Circle &circle) {
    return Geom::distance(center(), circle.center()) <= (circle.radius() - radius());
}

/*
 * If circle intersects with any polygon edge, circle is not contained       
 * If line between centers intersects with any polygon edge, circle is not contained
 * Only other possibility is that the circle is contained
 */
bool Circle::containedWithin(RegularConvexPolygon &polygon) 
{
    for (Line const &edge: polygon.edges()) {
        if (Geom::intersects(edge, *this)) 
            return false;
    }

    Line line(Geom::center(polygon), center_);
    for (Line const &edge: polygon.edges()) {
        if (Geom::intersects(edge, line))
            return false;
    }

    return true;
}

//If the distance from the center of any of the circles created by the vertices plus the radius
//of the contained circle is greater than the radius of the circles created by the RT then the 
//circle is partially outside or completely outside the RT
bool Circle::containedWithin(ReuleauxTriangle &rt) {
	//Get the radius of the circles from the RT
	double rtRadius = rt.getRadius();

	std::array<Point2D,3> vertices = rt.vertices();

	for(int i = 0; i < 3; i++){
		if(Geom::distance(vertices[i], center()) + radius() > rtRadius){
			return false;
		}
	}
	return true;

	    
}
