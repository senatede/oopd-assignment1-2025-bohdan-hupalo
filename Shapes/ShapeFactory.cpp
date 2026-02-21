#include "ShapeFactory.h"
#include "Triangle.h"
#include "Circle.h"
#include "Box.h"
#include "Line.h"

std::shared_ptr<Shape> ShapeFactory::createShape(ShapeType type, int boardWidth, int boardHeight, bool fill, const std::shared_ptr<Color>& color, int x, int y, const std::vector<int>& params) {
    switch (type) {
        case ShapeType::Triangle:
            if (params.size() != 1) throw std::invalid_argument("Triangle needs height");
            return std::make_shared<Triangle>(boardWidth, boardHeight, fill, color, x, y, params[0]);

        case ShapeType::Box:
            if (params.size() != 2) throw std::invalid_argument("Box needs width, height");
            return std::make_shared<Box>(boardWidth, boardHeight, fill, color, x, y, params[0], params[1]);

        case ShapeType::Circle:
            if (params.size() != 1) throw std::invalid_argument("Circle needs radius");
            return std::make_shared<Circle>(boardWidth, boardHeight, fill, color, x, y, params[0]);

        case ShapeType::Line:
            if (params.size() != 2) throw std::invalid_argument("Line needs x2, y2");
            return std::make_shared<Line>(boardWidth, boardHeight, fill, color, x, y, params[0], params[1]);
    }
};