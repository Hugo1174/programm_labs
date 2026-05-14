#include "Ball.hpp"
#include <cmath>

Ball::Ball(const Velocity velocity, const Point center,  double radius, const Color color, bool isCollidable): 
     velocity_general(velocity), center_general(center), radius_general(radius), color_general(color), isCollidable_general(isCollidable) {}

/**
 * Задает скорость объекта
 * @param velocity новое значение скорости
 */
void Ball::setVelocity(const Velocity& velocity) {
    this->velocity_general = velocity;
    
}

/**
 * @return скорость объекта
 */
Velocity Ball::getVelocity() const {
    // TODO: место для доработки
    return velocity_general;
}

/**
 * @brief Выполняет отрисовку объекта
 * @details объект Ball абстрагирован от конкретного
 * способа отображения пикселей на экране. Он "знаком"
 * лишь с интерфейсом, который предоставляет Painter
 * Рисование выполняется путем вызова painter.draw(...)
 * @param painter контекст отрисовки
 */
void Ball::draw(Painter& painter) const {
    painter.draw(center_general, radius_general, color_general);
}

/**
 * Задает координаты центра объекта
 * @param center новый центр объекта
 */
void Ball::setCenter(const Point& center) {
    this->center_general = center;
}

/**
 * @return центр объекта
 */
Point Ball::getCenter() const {
    return center_general;
}

/**
 * @brief Возвращает радиус объекта
 * @details обратите внимание, что метод setRadius()
 * не требуется
 */
double Ball::getRadius() const {
    return radius_general;
}

/**
 * @brief Возвращает массу объекта
 * @details В нашем приложении считаем, что все шары
 * состоят из одинакового материала с фиксированной
 * плотностью. В этом случае масса в условных единицах
 * эквивалентна объему: PI * radius^3 * 4. / 3.
 */
double Ball::getMass() const {
    return  M_PI * pow(radius_general, 3) * 4. / 3.;
}

Color Ball::getColor() const {
    return color_general;
}

bool Ball::ifCollidable() const{
    return isCollidable_general;
}


