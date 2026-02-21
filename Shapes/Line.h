#pragma once

#include "Shape.h"

class Line final : public Shape {
public:
    int dx, dy;
    Line(const int boardWidth, const int boardHeight, const bool fill, const std::shared_ptr<Color> &color,
        const int x, const int y, const int x2, const int y2)
            : Shape(boardWidth, boardHeight, ShapeType::Line, fill, color, x, y), dx(x2-x), dy(y2-y) {};
    ~Line() override = default;
    bool equals(const Shape& other) const override;
    std::string info() override;
    std::vector<std::pair<int, int>> getCoordinates() override;
};
