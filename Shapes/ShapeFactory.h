#pragma once

#include "Shape.h"

class ShapeFactory {
public:
    static std::shared_ptr<Shape> createShape(
        ShapeType type,
        int boardWidth, int boardHeight,
        bool fill,
        const std::shared_ptr<Color>& color,
        int x, int y,
        const std::vector<int>& params
    );
};
