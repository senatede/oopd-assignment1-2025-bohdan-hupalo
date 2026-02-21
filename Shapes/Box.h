#pragma once

#include "Shape.h"

class Box final : public Shape {
public:
    int width, height;

    Box(const int boardWidth, const int boardHeight, const bool fill, const std::shared_ptr<Color> &color,
        const int x, const int y, const int w, const int h)
            : Shape(boardWidth, boardHeight, ShapeType::Box, fill, color, x, y), width(w), height(h) {}
    ~Box() override = default;
    bool equals(const Shape& other) const override;
    std::string info() override;
    std::vector<std::pair<int, int>> getCoordinates() override;
};

