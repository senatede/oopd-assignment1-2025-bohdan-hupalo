#pragma once

#include <utility>
#include <vector>
#include "Color.h"

enum class ShapeType { Triangle, Box, Circle, Line };

class Shape {
protected:
    int boardWidth, boardHeight;
    static int nextId;

    void setPixel(std::vector<std::pair<int, int>>& coordinates, const int row, const int col) const;
public:
    ShapeType type;
    int x, y;
    int id;
    bool fill;
    std::shared_ptr<Color> color;

    Shape(const int boardWidth, const int boardHeight, const ShapeType type, const bool fill, const std::shared_ptr<Color> &color, const int x, const int y)
        : boardWidth(boardWidth), boardHeight(boardHeight), type(type), x(x), y(y), id(nextId++), fill(fill), color(color) {}
    virtual ~Shape() = default;

    virtual bool equals(const Shape& other) const;
    virtual std::string info() = 0;
    virtual std::vector<std::pair<int, int>> getCoordinates() = 0;
    static int getNextId() {return nextId;}
    static void setNextId(const int id) {nextId = id;}
};