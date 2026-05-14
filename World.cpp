#include "World.h"
#include "Painter.h"
#include <fstream>
#include "algorithm"

// Длительность одного тика симуляции.
// Подробнее см. update()
// Изменять не следует
static constexpr double timePerTick = 0.001;


/// @brief читает коодринаты точки
/// @param stream 
/// @return 
Point readPoint(std::istream& stream)
{
    double x, y;
    stream >> x >> y;
    return Point(x, y);
}

/// @brief читает цвет
/// @param stream 
/// @return 
Color readColor(std::istream& stream)
{
    double r, g, b;
    stream >> r >> g >> b;
    return Color(r, g, b);
}


/**
 * Конструирует объект мира для симуляции
 * @param worldFilePath путь к файлу модели мира
 */
World::World(const std::string& worldFilePath) {
    std::ifstream stream(worldFilePath);
    topLeft = readPoint(stream);
    bottomRight = readPoint(stream);
    physics.setWorldBox(topLeft, bottomRight);

    double radius;
    bool isCollidable;

    // Здесь не хватает обработки ошибок, но на текущем
    // уровне прохождения курса нас это устраивает
    while (stream.peek(), stream.good()) {
        Point center = readPoint(stream);
        Point vectorV = readPoint(stream);
        Velocity velocity(vectorV);
        
        // Читаем три составляющие цвета шара
        Color color = readColor(stream);
        // Читаем радиус шара
        stream >> radius;
        stream >> std::boolalpha >> isCollidable;
        Ball ball(velocity, center,  radius, color, isCollidable);
        balls.push_back(ball);
    }
}

/// @brief Отображает состояние мира
void World::show(Painter& painter) const {
    // Рисуем белый прямоугольник, отображающий границу
    // мира
    painter.draw(topLeft, bottomRight, Color(1, 1, 1));

    // Вызываем отрисовку каждого шара
    for (const Ball& ball : balls) {
        ball.draw(painter);
    }
    for (const Dust& dust : dusts) {
        if(dust.ifAlive()){ 
            dust.draw(painter); 
            
        }
        
    }
}

/// @brief Обновляет состояние мира
void World::update(double time) {
    // учитываем остаток времени, который мы не "доработали" при прошлом update
    time += restTime;
    const auto ticks = static_cast<size_t>(std::floor(time / timePerTick));
    restTime = time - double(ticks) * timePerTick;

    physics.update(balls, dusts, ticks);

    // Обновление времени жизни частиц
    for (auto& dust : dusts) {
        dust.dying(timePerTick); // передача времени для обновления частиц
    }

    // Удаление мертвых частиц (ДО обновления физики)
    dusts.erase(std::remove_if(dusts.begin(), dusts.end(), [](const Dust& dust) { 
        return !dust.ifAlive(); }),
        dusts.end());
}
