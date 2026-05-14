#pragma once
#include <iostream>
#include <string>

int id_graphic = 1;

// класс, представляющий графический примитив
class GraphicPrimitive
{
private:
    const int id;           // id графического примитива
    std::string figureName; // название примитива

public:
    // конструктор с параметрами
    // name название примитива
    GraphicPrimitive(const std::string name) : figureName(name), id(id_graphic++) {}


    // конструктор с ID (для избежания ошибок при использовании make_shared)
    GraphicPrimitive(int id, const std::string name) : id(id), figureName(name) {}

    //  гетер id
    const int getId() const
    {
        return id;
    }

    // гетер названия примитива
    const std::string getFigureName() const
    {
        return figureName;
    }

    // деструктор
    ~GraphicPrimitive()
    {
        std::cout << "Удален графический примитив \"" << figureName << "\" (ID: " << id << ")" << std ::endl;
    }
};

// класс графического примитива "круг"
class Circle : public GraphicPrimitive
{
private:
    double radius; // радиус круга
public:
    //  конструктор с параметрами
    //  radius радиус
    Circle(const double &radius) : GraphicPrimitive("Circle"), radius(radius)
    {
        std::cout << "Создан графический примитив \"Circle\" с радиусом " << radius << "; ID: " << getId() << std ::endl;
    }

};

// класс графического примитива "прямоугольник"
class Rectangle : public GraphicPrimitive
{
private:
    double length; // длина прямоугольника
    double width;  // ширина прямоугольника
public:
    // конструктор с параметрами
    // length длина
    // width ширина
    Rectangle(const double &length, const double &width) : GraphicPrimitive("Rectangle"), length(length), width(width)
    {
        std::cout << "Создан графический примитив \"Rectangle\" с длиной " << length << " и шириной " << width << "; ID: " << getId() << std ::endl;
    }

};

// класс прямоугольника с равными сторонами (квадрат)
class Square : public Rectangle
{
private:
    double length; // сторона квадрата
public:
    // конструктор с параметрами
    // length сторона квадрата
    Square(const double &length) : Rectangle(length, length) {}
};