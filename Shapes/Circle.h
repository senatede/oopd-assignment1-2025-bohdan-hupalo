#pragma once

#include "Shape.h"

class Circle final : public Shape {
    public:
    int radius;
    Circle(const int boardWidth, const int boardHeight, const bool fill, const std::shared_ptr<Color> &color,
        const int x, const int y, const int r)
            : Shape(boardWidth, boardHeight, ShapeType::Circle, fill, color, x, y), radius(r) {};
    ~Circle() override = default;
    bool equals(const Shape& other) const override;
    std::string info() override;
    std::vector<std::pair<int, int>> getCoordinates() override;
};
