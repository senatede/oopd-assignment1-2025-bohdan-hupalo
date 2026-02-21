#include "Shape.h"

void Shape::setPixel(std::vector<std::pair<int, int>>& coordinates, const int row, const int col) const {
    if (col >= 0 && col < boardWidth && row >= 0 && row < boardHeight) coordinates.emplace_back(row, col);
}

bool Shape::equals(const Shape& other) const {
    return id != other.id &&
           type == other.type &&
           fill == other.fill &&
           color == other.color &&
           x == other.x &&
           y == other.y;
}