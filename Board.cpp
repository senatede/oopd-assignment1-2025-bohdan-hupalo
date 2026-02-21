#include "Board.h"

#include "Shapes/Triangle.h"
#include "Shapes/Box.h"
#include "Shapes/Circle.h"
#include "Shapes/Line.h"
#include "Shapes/ShapeFactory.h"
#include <iostream>

int Shape::nextId = 0;

Board::Board(const int w, const int h) : width(w), height(h), shapes(0), grid(std::vector(this->height, std::vector<std::shared_ptr<Color>>(this->width, nullptr))) {
    colors["red"] = std::make_shared<Color>("red", 255, 0, 0);
    colors["green"] = std::make_shared<Color>("green", 0, 255, 0);
    colors["blue"] = std::make_shared<Color>("blue", 0, 0, 255);
    colors["white"] = std::make_shared<Color>("white", 255, 255, 255);
}

void Board::drawAllShapes() {
    grid = std::vector(this->height, std::vector<std::shared_ptr<Color>>(this->width, nullptr));
    for (const auto& shape : this->shapes) {
        auto coordinates = shape->getCoordinates();
        if (!coordinates.empty()) {
            for (auto [row, col] : coordinates) {
                grid[row][col] = shape->color;
            }
        }
    }
}

void Board::print() {
    std::string result;
    drawAllShapes();
    std::cout << "\033[2J\033[H";
    for (auto& row : grid) {
        for (auto c : row) {
            if (c == nullptr) {
                std::cout << "\033[0m";
                std::cout << ' ';
            }
            else {
                std::cout << "\033[38;2;" << c->r << ";" << c->g << ";" << c->b << "m";;
                std::cout << static_cast<char>(std::toupper(c->name[0]));
            }
        }
        std::cout << "\n";
    }
    std::cout << "\033[0m";
}

void Board::clear() {
    shapes.clear();
    grid = std::vector(this->height, std::vector<std::shared_ptr<Color>>(this->width, nullptr));
}

std::vector<std::string> Board::listShapes() const {
    std::vector<std::string> result;
    result.reserve(this->shapes.size());
    for (const auto& shape : this->shapes) {
        std::string info = "";
        if (selectedShapeIndex >= 0 && selectedShapeIndex < shapes.size() && shape->id == shapes[selectedShapeIndex]->id)
            info += "* ";
        info += shape->info();
        result.emplace_back(info);
    }
    return result;
}

int Board::selectShape(int id) {
    auto it = [&]->int {
        for (size_t i = 0; i < shapes.size(); i++) if (shapes[i]->id == id) return i;
        return -1;
    };
    const int index = it();
    if (index == -1) return 1;
    selectedShapeIndex = index;
    std::cout << "You have selected shape with id:" << shapes[selectedShapeIndex]->info() << std::endl;
    return 0;
}

int Board::isShapeValid(const std::shared_ptr<Shape>& shape) const {
    const size_t n_coord = shape->getCoordinates().size();
    if (n_coord == 0) return ERROR_INVISIBLE_SHAPE;
    if (n_coord == width * height) return ERROR_TOO_LARGE_SHAPE;
    for (const auto& sh : this->shapes) {
        if (shape->equals(*sh)) return ERROR_DUPLICATE_SHAPE;
    }
    return 0;
}

int Board::addShape(ShapeType type, const bool fill, const std::string& color, const int x, const int y, const std::vector<int>& params) {
    if (!colors.contains(color)) return ERROR_INVALID_COLOR;

    try {
        auto shape = ShapeFactory::createShape(type, this->width, this->height, fill, colors[color], x, y, params);

        const int valid = isShapeValid(shape);
        if (valid == 0) {
            shapes.emplace_back(shape); return 0;
        }
        return valid;
    } catch (const std::invalid_argument&) {
        return ERROR_INVALID_ARGUMENTS;
    }
}

int Board::selectShape(int x, int y) {
    auto it = [&] {
        for (int i = static_cast<int>(shapes.size()) - 1; i >= 0; i--)
            if (std::ranges::any_of(shapes[i]->getCoordinates(), [&](const auto& c){ return c.first == y && c.second == x; })) {
                return i;
            }
        return -1;
    };
    const int index = it();
    if (index == -1) return ERROR_NO_SELECTION;
    selectedShapeIndex = index;
    std::cout << "You have selected shape with " << shapes[selectedShapeIndex]->info() << std::endl;
    return 0;
}


int Board::removeShape() {
    if (selectedShapeIndex == -1) return ERROR_NO_SELECTION;
    shapes.erase(shapes.begin() + selectedShapeIndex);
    selectedShapeIndex = -1;
    return 0;
}

int Board::paintShape(const std::string& color) {
    if (selectedShapeIndex == -1) return ERROR_NO_SELECTION;
    if (!colors.contains(color)) return ERROR_INVALID_COLOR;
    const auto old_color = shapes[selectedShapeIndex]->color;
    shapes[selectedShapeIndex]->color = colors[color];
    if (const int code = isShapeValid(shapes[selectedShapeIndex]); code != 0) {
        shapes[selectedShapeIndex]->color = old_color;
        return code;
    }
    return 0;
}

int Board::editShape(const std::vector<int>& parameters) const {
    if (selectedShapeIndex == -1) return ERROR_NO_SELECTION;

    switch (auto shape = shapes[selectedShapeIndex]; shape->type) {
        case ShapeType::Triangle: {
            if (parameters.size() != 1) return ERROR_INVALID_ARGUMENTS;

            const auto tr = std::dynamic_pointer_cast<Triangle>(shape);
            const int old_height = tr->height;
            const int new_height = parameters[0];
            tr->height = new_height;

            const int valid = isShapeValid(tr);
            if (valid == 0) return 0;
            tr->height = old_height;
            return valid;
        }
        case ShapeType::Box:{
            if (parameters.size() != 2) return ERROR_INVALID_ARGUMENTS;

            const auto box = std::dynamic_pointer_cast<Box>(shape);
            const int old_width = box->width;
            const int new_width = parameters[0];
            const int old_height = box->height;
            const int new_height = parameters[1];
            box->width = new_width;
            box->height = new_height;

            const int valid = isShapeValid(box);
            if (valid == 0) return 0;
            box->width = old_width;
            box->height = old_height;
            return valid;
        }
        case ShapeType::Circle: {
            if (parameters.size() != 1) return ERROR_INVALID_ARGUMENTS;

            const auto cir = std::dynamic_pointer_cast<Circle>(shape);
            const int old_radius = cir->radius;
            const int new_radius = parameters[0];
            cir->radius = new_radius;

            const int valid = isShapeValid(cir);
            if (valid == 0) return 0;
            cir->radius = old_radius;
            return valid;
        }
        case ShapeType::Line: {
            if (parameters.size() != 2) return ERROR_INVALID_ARGUMENTS;

            const auto lin = std::dynamic_pointer_cast<Line>(shape);
            const int old_dx = lin->dx;
            const int new_dx = parameters[0]-lin->x;
            const int old_dy = lin->y;
            const int new_dy = parameters[1]-lin->y;
            lin->dx = new_dx;
            lin->dy = new_dy;

            const int valid = isShapeValid(lin);
            if (valid == 0) return 0;
            lin->dx = old_dx;
            lin->dy = old_dy;
            return valid;
        }
    }
}

int Board::moveShape(const std::vector<int>& parameters) {
    int x, y;
    if (selectedShapeIndex == -1) return ERROR_NO_SELECTION;
    auto shape = shapes[selectedShapeIndex];
    switch (parameters.size()) {
        case 0:
            x = shape->x; y = shape->y; break;
        case 2:
            x = parameters[0]; y = parameters[1]; break;
        default:
            return ERROR_INVALID_ARGUMENTS;
    }
    const int old_x = shape->x;
    const int old_y = shape->y;
    shape->x = x;
    shape->y = y;
    int valid = isShapeValid(shape);
    if (x == old_x && y == old_y || valid == 0) {
        shapes.emplace_back(shape);
        shapes.erase(shapes.begin() + selectedShapeIndex);
        selectedShapeIndex = static_cast<int>(shapes.size()) - 1;
        return 0;
    }
    shape->x = old_x;
    shape->y = old_y;
    return valid;
}

int Board::saveToFile(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file.is_open()) return 1;

    file << "{\n";
    file << R"(  "width": )" << width << ",\n";
    file << R"(  "height": )" << height << ",\n";
    file << R"(  "next_id": )" << Shape::getNextId() << ",\n";
    file << R"(  "selected_id": )" << selectedShapeIndex << ",\n";

    file << R"(  "colors": [)" << "\n";
    int i = 0;
    for (const auto &val: colors | std::views::values) {
        i++;
        if (auto &color = val; color->name != "red" && color->name != "green" && color->name != "blue" && color->name != "white") {
            file << "    {\n";
            file << R"(      "name": ")" << color->name << "\",\n";
            file << R"(      "r": )" << color->r << ",\n";
            file << R"(      "g": )" << color->g << ",\n";
            file << R"(      "b": )" << color->b << "\n";
            file << "    }";
            if (i < colors.size()-4) file << ",";
            file << "\n";
        }
    }

    file << "  ],\n";

    file << R"(  "shapes": [)" << "\n";
    for (size_t i = 0; i < shapes.size(); i++) {
        auto &shape = shapes[i];
        file << "    {\n";
        file << R"(      "id": )" << shape->id << ",\n";
        file << R"(      "type": )" << static_cast<int>(shape->type) << ",\n";
        file << R"(      "fill": )" << shape->fill << ",\n";
        file << R"(      "color": ")" << shape->color->name << "\",\n";
        file << R"(      "x": )" << shape->x << ",\n";
        file << R"(      "y": )" << shape->y << ",\n";

        switch (shape->type) {
            case ShapeType::Triangle: {
                const auto tr = std::dynamic_pointer_cast<Triangle>(shape);
                file << "      \"height\": " << tr->height << "\n";
                break;
            }
            case ShapeType::Box: {
                const auto box = std::dynamic_pointer_cast<Box>(shape);
                file << "      \"width\": " << box->width << ",\n";
                file << "      \"height\": " << box->height << "\n";
                break;
            }
            case ShapeType::Circle: {
                const auto cir = std::dynamic_pointer_cast<Circle>(shape);
                file << "      \"radius\": " << cir->radius << "\n";
                break;
            }
            case ShapeType::Line: {
                const auto lin = std::dynamic_pointer_cast<Line>(shape);
                file << "      \"dx\": " << lin->dx << ",\n";
                file << "      \"dy\": " << lin->dy << "\n";
                break;
            }
        }
        file << "    }";
        if (i + 1 < shapes.size()) file << ",";
        file << "\n";
    }

    file << "  ]\n";
    file << "}\n";

    file.close();
    return 0;
}

std::unique_ptr<Board> Board::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "Error: file " << filepath << " doesnt exist." << std::endl;
        return nullptr;
    }

    nlohmann::json j;
    try {
        file >> j;
    } catch (const nlohmann::json::exception&) {
        file.close();
        std::cout << "Error: file " << filepath << " is not a json filetype" << std::endl;
        return nullptr;
    }
    file.close();

    if (!j.contains("width") || !j.contains("height") || !j.contains("next_id") || !j.contains("selected_id")) {
        std::cout << "Error: file " << filepath << " does not include necessary board parameters"<< std::endl;
        return nullptr;
    }

    int width = j["width"];
    int height = j["height"];
    auto board = std::make_unique<Board>(width, height);
    Shape::setNextId(j["next_id"]);
    board->selectedShapeIndex = j["selected_id"];

    if (!j.contains("colors") || !j["colors"].is_array()) {
        std::cout << "Error: file " << filepath << " does not contain color parameters." << std::endl;
        return nullptr;
    }
    for (const auto& c : j["colors"]) {
        if (!c.contains("name") || !c.contains("r") || !c.contains("g") || !c.contains("b")) {
            std::cout << "Error: some color doesn't contain necessary parameters " << filepath << std::endl;
            return nullptr;
        }
        std::string name = c["name"];
        int r = c["r"];
        int g = c["g"];
        int b = c["b"];
        if (r < 0 || r > 255) {
            std::cout << "Error: color " << name << "has invalid r value." << std::endl;
            return nullptr;
        }
        if (g < 0 || g > 255) {
            std::cout << "Error: color " << name << "has invalid g value." << std::endl;
            return nullptr;
        }
        if (b < 0 || b > 255) {
            std::cout << "Error: color " << name << "has invalid b value." << std::endl;
            return nullptr;
        }

        if (board->colors.contains(name)) {
            std::cout << "Error: color " << name << " already exists." << std::endl;
            return nullptr;
        }
        board->colors[name] = std::make_shared<Color>(name, r, g, b);
    }

    if (!j.contains("shapes") || !j["shapes"].is_array())
        return board;
    for (const auto& s : j["shapes"]) {
        if (!s.contains("id") || !s.contains("type") || !s.contains("fill") || !s.contains("color") || !s.contains("x") || !s.contains("y")) {
            std::cout << "Error: some shape doesn't contain necessary parameters " << filepath << std::endl;
            return nullptr;
        }

        int id = s["id"];
        auto type = static_cast<ShapeType>(s["type"].get<int>());
        bool fill = s["fill"].get<int>() != 0;
        std::string color = s["color"];
        if (!board->colors.contains(color)) {
            std::cout << "Error: shape with id " << id << " uses unknown color" << std::endl;
            return nullptr;
        }
        int x = s["x"];
        int y = s["y"];

        switch (type) {
            case ShapeType::Triangle: {
                if (!s.contains("height")){
                    std::cout << "Error: triangle with id:" << id << "doesn't contain height of triangle" << std::endl;
                    return nullptr;
                }
                int h = s["height"];
                auto tr = std::make_shared<Triangle>(board->width, board->height, fill, board->colors[color], x, y, h);
                tr->id = id;
                if (board->isShapeValid(tr) != 0) {
                    std::cout << "Error: shape with id:" << id << "will be a duplicate" << std::endl;
                    return nullptr;
                }
                board->shapes.emplace_back(tr);
                break;
            }
            case ShapeType::Box: {
                if (!s.contains("width") || !s.contains("height")){
                    std::cout << "Error: box with id:" << id << "doesn't contain width and height" << std::endl;
                    return nullptr;
                }
                int w = s["width"];
                int h = s["height"];
                auto box = std::make_shared<Box>(board->width, board->height, fill, board->colors[color], x, y, w, h);
                box->id = id;
                if (board->isShapeValid(box) != 0) {
                    std::cout << "Error: shape with id:" << id << "will be a duplicate" << std::endl;
                    return nullptr;
                }
                board->shapes.emplace_back(box);
                break;
            }
            case ShapeType::Circle: {
                if (!s.contains("radius")) {
                    std::cout << "Error: circle with id:" << id << "doesn't contain radius" << std::endl;
                    return nullptr;
                }
                int r = s["radius"];
                auto cir = std::make_shared<Circle>(board->width, board->height, fill, board->colors[color], x, y, r);
                cir->id = id;
                if (board->isShapeValid(cir) != 0) {
                    std::cout << "Error: shape with id:" << id << "will be a duplicate" << std::endl;
                    return nullptr;
                }
                board->shapes.emplace_back(cir);
                break;
            }
            case ShapeType::Line: {
                if (!s.contains("dx") || !s.contains("dy")) {
                    std::cout << "Error: line with id:" << id << "doesn't contain dx and dy" << std::endl;
                    return nullptr;
                }
                int dx = s["dx"];
                int dy = s["dy"];
                auto lin = std::make_shared<Line>(board->width, board->height, fill, board->colors[color], x, y, x + dx, y + dy);
                lin->id = id;
                if (board->isShapeValid(lin) != 0) {
                    std::cout << "Error: shape with id:" << id << "will be a duplicate" << std::endl;
                    return nullptr;
                }
                board->shapes.emplace_back(lin);
                break;
            }
        }
    }
    return board;
}

int Board::addColor(const std::string& name, const int r, const int g, const int b) {
    if (colors.contains(name)) return 1;
    colors.emplace(name, std::make_shared<Color>(name, r, g, b));
    return 0;
}

std::vector<std::string> Board::listColors() const {
    std::vector<std::string> result;
    if (colors.empty()) return result;
    for (const auto& c : colors)
        result.emplace_back(c.second->info());
    return result;
}
