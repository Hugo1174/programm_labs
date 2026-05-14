#pragma once
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"
#include "Color.h"

class Ball {
public:
    Ball(const Velocity velocity, const Point center,  double radius, const Color color, bool isCollidable);

    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;

    void draw(Painter& painter) const;

    void setCenter(const Point& center);
    Point getCenter() const;

    double getRadius() const;
    
    double getMass() const;
    Color getColor() const;
    bool ifCollidable() const;
    
private:
    Velocity velocity_general;
    Point center_general;
    double radius_general;
    Color color_general;
    bool isCollidable_general;
    
};

