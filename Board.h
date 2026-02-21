#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <unordered_map>
#include <Shapes/Color.h>
#include "Shapes/Shape.h"


enum select {
    SELECT_BY_ID_CMD_SIZE,
    SELECT_BY_COORDINATES_CMD_SIZE
};

enum errors {
    ERROR_NO_SELECTION,
    ERROR_INVALID_COLOR,
    ERROR_DUPLICATE_SHAPE,
    ERROR_INVALID_ARGUMENTS,
    ERROR_INVISIBLE_SHAPE,
    ERROR_TOO_LARGE_SHAPE,
};


class Board {
public:
    int width, height;

    Board(int w, int h);
    Board() = default;

    ~Board() = default;

    void print();
    void clear();
    std::vector<std::string> listShapes() const;
    int selectShape(int id);
    int selectShape(int x, int y);
    int removeShape();
    int paintShape(const std::string& color);
    int editShape(const std::vector<int>& parameters) const;
    int moveShape(const std::vector<int>& parameters);
    int addShape(ShapeType type, bool fill, const std::string& color, int x, int y, const std::vector<int>& params);
    int addColor(const std::string& name, int r, int g, int b);
    std::vector<std::string> listColors() const;

    int saveToFile(const std::string& filepath) const;
    static std::unique_ptr<Board> loadFromFile(const std::string& filepath);
private:
    std::unordered_map<std::string, std::shared_ptr<Color>> colors;
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<std::vector<std::shared_ptr<Color>> > grid;
    int selectedShapeIndex = -1;
    void drawAllShapes();
    int isShapeValid(const std::shared_ptr<Shape>& shape) const;
};

