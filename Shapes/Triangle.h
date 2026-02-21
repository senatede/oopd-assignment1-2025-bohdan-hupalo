#pragma once

#include "Shape.h"

class Triangle final : public Shape {
public:
    int height;

    Triangle(const int boardWidth, const int boardHeight, const bool fill, const std::shared_ptr<Color> &color,
        const int x, const int y, const int h)
            : Shape(boardWidth, boardHeight, ShapeType::Triangle, fill, color, x, y), height(h) {}
    ~Triangle() override = default;
    bool equals(const Shape& other) const override;
    std::string info() override;
    std::vector<std::pair<int, int>> getCoordinates() override;
};
