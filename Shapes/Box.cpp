#include "Box.h"

bool Box::equals(const Shape& other) const {
    if (this->type != other.type) return this->id == other.id;
    auto& b = dynamic_cast<const Box&>(other);
    return Shape::equals(other) && width == b.width && height == b.height;
}

std::string Box::info() {
    std::string result;
    result += "id:";
    result += std::to_string(id);
    result += " box ";
    if (fill) result += "fill ";
    else result += "frame ";
    result += color->name;
    result += " x:";
    result += std::to_string(x);
    result += " y:";
    result += std::to_string(y);
    result += " w:";
    result += std::to_string(width);
    result += " h:";
    result += std::to_string(height);
    return result;
}

std::vector<std::pair<int, int>> Box::getCoordinates() {
    std::vector<std::pair<int, int>> coordinates;
    for (int i = y; i < y + height; i++) {
        if (i >= 0 && i < boardHeight) {
            if (i == y || i == y + height - 1 || fill) {
                for (int j = x; j < x + width; j++)
                    if (j >= 0 && j < boardWidth)
                        setPixel(coordinates, i, j);
            }
            else {
                setPixel(coordinates,i, x);
                setPixel(coordinates, i, x + width - 1);
            }
        }
    }
    return coordinates;
}
