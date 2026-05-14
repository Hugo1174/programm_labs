#pragma once
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"
#include "Color.h"

class Ball {
public:
    Ball(const Velocity velocity, const Point center,  double radius, const Color color, bool isCollidable);

    void setVelocity(const Velocity& velocity); //устанавливает скорость шара
    Velocity getVelocity() const;  //возвращает текущую скорость шара

    void draw(Painter& painter) const; //рисует шар

    void setCenter(const Point& center); //устанавливает новые координаты центра шара
    Point getCenter() const;  //возвращает координаты центра шара

    double getRadius() const; //возвращает радиус шара
    double getMass() const; //вычисляет и возвращает массу шара, используя формулу объёма сферы и предполагая постоянную плотность
    Color getColor() const;  //возвращает цвет шара
    bool ifCollidable() const; //Возвращает true, если шар участвует в коллизиях, и false в противном случае
    
private:
    Velocity velocity_general;
    Point center_general; 
    double radius_general;
    Color color_general;
    bool isCollidable_general; 
};

