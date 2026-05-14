#define _USE_MATH_DEFINES
#include "Physics.h"


double dot(const Point& lhs, const Point& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Physics::Physics(double timePerTick) : timePerTick{timePerTick} {}

void Physics::setWorldBox(const Point& topLeft, const Point& bottomRight) {
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
}

void Physics::update(std::vector<Ball>& balls,std::vector<Dust>& dusts, const size_t ticks) {
    for (size_t i = 0; i < ticks; ++i) {
        move(balls);
        moveDusts(dusts);
        collideWithBox(balls, dusts);
        collideBalls(balls, dusts);
    }
}


void Physics::collideWithBox(std::vector<Ball>& balls, std::vector<Dust>& dusts) const {
    for (Ball& ball : balls) {
        // шары, не участвующие в коллизиях, 
        // должны пролетать и сквозь стены тоже!
        if (!ball.ifCollidable()) continue;

        const Point p = ball.getCenter();
        const double r = ball.getRadius();
        
        auto isOutOfRange = [](double v, double lo, double hi) {
            return v < lo || v > hi;
        };

        // else if — генератор смайлика рассчитывал именно на такую обработку углов
        if (isOutOfRange(p.x, topLeft.x + r, bottomRight.x - r)) {
            Point vector = ball.getVelocity().vector();
            vector.x = -vector.x;
            ball.setVelocity(vector);
            
            // Эффекты пыли
            for (int i = 0; i < 5; i++) {
                double angle = (i * 2 * M_PI / 5); 
                Velocity v(200.0, angle); 
                dusts.push_back(Dust(v, p, r / 4.0, ball.getColor(), false, 0.4));
            }
        } else if (isOutOfRange(p.y, topLeft.y + r, bottomRight.y - r)) {
            Point vector = ball.getVelocity().vector();
            vector.y = -vector.y;
            ball.setVelocity(vector);

            for (int i = 0; i < 5; i++) {
                double angle = (i * 2 * M_PI / 5); 
                Velocity v(200.0, angle); 
                dusts.push_back(Dust(v, p, r / 4.0, ball.getColor(), false, 0.4));
            }
        }
    }
}

void Physics::collideBalls(std::vector<Ball>& balls, std::vector<Dust>& dusts) const {
    for (auto a = balls.begin(); a != balls.end(); ++a) {
        for (auto b = std::next(a); b != balls.end(); ++b) {
            if (a->ifCollidable() && b->ifCollidable()) {
                const double distanceBetweenCenters2 = distance2(a->getCenter(), b->getCenter());
                const double collisionDistance = a->getRadius() + b->getRadius();
                const double collisionDistance2 = collisionDistance * collisionDistance;

                if (distanceBetweenCenters2 < collisionDistance2) {
                    processCollision(*a, *b, distanceBetweenCenters2);
                    
                    // Пыль при столкновении шаров
                    for (int i = 0; i < 6; i++) {
                        double angle = (i * 2 * M_PI / 6); 
                        Velocity v(250.0, angle); 
                        dusts.push_back(Dust(v, a->getCenter(), a->getRadius() / 4.0, a->getColor(), false, 0.4));
                    }
                }
            }
        }
    }
}

void Physics::move(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {
        Point newPos =
            ball.getCenter() + ball.getVelocity().vector() * timePerTick;
        ball.setCenter(newPos);
    }
}



void Physics::processCollision(Ball& a, Ball& b, double distanceBetweenCenters2) const {
    const Point normal = (b.getCenter() - a.getCenter()) / std::sqrt(distanceBetweenCenters2);
    const Point aV = a.getVelocity().vector();
    const Point bV = b.getVelocity().vector();
    
    const double p = 2 * (dot(aV, normal) - dot(bV, normal)) / (a.getMass() + b.getMass());
 
    // Хоть с точки зрения реальной физики -  b.getMass(), 
    // для сборки смайлика требуется именно a.getMass().
    a.setVelocity(Velocity(aV - normal * p * a.getMass()));
    b.setVelocity(Velocity(bV + normal * p * b.getMass()));
}

void Physics::moveDusts(std::vector<Dust>& dusts) const {
    for (Dust& dust : dusts) {
        if (dust.ifAlive())
        {
            Point newPos = dust.getCenter() + dust.getVelocity().vector() * timePerTick;
            dust.setCenter(newPos);
        }
    }
}


//**cd build  
//**cmake ..  
//**cmake --build .
//** */.\build\physics.exe D:/3_semester/proga/lab5_7/lab5/data/elephant.txt