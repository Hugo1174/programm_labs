#include "Dust.hpp"
#include <cmath>

Dust::Dust(const Velocity velocity, const Point center, double radius, const Color color, bool isCollidable, double lifetime)
    : velocity_general(velocity), center_general(center), radius_general(radius), color_general(color), isCollidable_general(isCollidable), lifetime_general(lifetime) {}
/**
 * Задает скорость объекта
 * @param velocity новое значение скорости
 */
void Dust::setVelocity(const Velocity& velocity) {
    this->velocity_general = velocity;
    
}

/**
 * @return скорость объекта
 */
Velocity Dust::getVelocity() const {
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
void Dust::draw(Painter& painter) const {
    painter.draw(center_general, radius_general, color_general);
}

/**
 * Задает координаты центра объекта
 * @param center новый центр объекта
 */
void Dust::setCenter(const Point& center) {
    this->center_general = center;
}

/**
 * @return центр объекта
 */
Point Dust::getCenter() const {
    return center_general;
}

/**
 * @brief Возвращает радиус объекта
 * @details обратите внимание, что метод setRadius()
 * не требуется
 */
double Dust::getRadius() const {
    return radius_general;
}

/**
 * @brief Возвращает массу объекта
 * @details В нашем приложении считаем, что все шары
 * состоят из одинакового материала с фиксированной
 * плотностью. В этом случае масса в условных единицах
 * эквивалентна объему: PI * radius^3 * 4. / 3.
 */
double Dust::getMass() const {
    return  M_PI * pow(radius_general, 3) * 4. / 3.;
}

bool Dust::ifAlive() const {
    return isAlive_general;
}


void Dust::dying(double time){
    lifetime_general -= time;
    if (lifetime_general <= 0){ isAlive_general = false; }
}
