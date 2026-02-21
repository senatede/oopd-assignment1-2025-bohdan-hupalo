#include "CLI.h"

#include <iostream>

CLI::CLI(const int h) : term_height(h) {
    std::cout << "\033[2J\033[H";
    std::cout << "> ";
    board = nullptr;
}

[[noreturn]] void CLI::run() {
    while (true)
        command_handling(command_input());
}

std::string CLI::command_input() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void CLI::command_handling(const std::string& input) {
    std::cout << "\033[2J\033[H";
    int n = 0;

    if (input == "help") n = help();
    else if (input == "exit") n = exit_();
    else if (input.starts_with("create")) n = create(input);
    else if (input == "clear") n = clear();
    else if (input == "draw") n = draw();
    else if (input.starts_with("add")) n = add(input);
    else if (input == "list") n = list();
    else if (input.starts_with("select")) n = select(input);
    else if (input == "shapes") n = shapes();
    else if (input == "remove") n = remove();
    else if (input.starts_with("paint")) n = paint(input);
    else if (input.starts_with("edit")) n = edit(input);
    else if (input.starts_with("move")) n = move(input);
    else if (input.starts_with("save")) n = save(input);
    else if (input.starts_with("load")) n = load(input);
    else if (input == "colors") n = colors();
    else if (input.starts_with("color")) n = color(input);
    else n = unknown(input);

    for (int i = n; i < term_height - 1; ++i)
        std::cout << std::endl;
    std::cout << "> ";
}

int CLI::help() {
    std::cout << "Available commands:" << std::endl;
    std::cout << " - help" << std::endl;
    std::cout << " - exit" << std::endl;
    std::cout << " - create <width> <height> - Create blackboard." << std::endl;
    std::cout << " - draw - Draw blackboard to the console." << std::endl;
    std::cout << " - color <name> <r> <g> <b> - add color." << std::endl;
    std::cout << " - colors - list all available colors." << std::endl;
    std::cout << " - list - Print all added shapes with their IDs and parameters." << std::endl;
    std::cout << " - shape - Print a list of all available shapes and parameters for add call." << std::endl;
    std::cout << " - add <parameters> - Add shape with specified colour and fill mode to the blackboard." << std::endl;
    std::cout << " - select <id|x y> - Select a shape with an ID Or foreground shape by the selected coordinates." << std::endl;
    std::cout << " - remove - Remove selected from blackboard the shape." << std::endl;
    std::cout << " - edit <parameters> - Allows to modify the parameters of the selected figure." << std::endl;
    std::cout << " - paint <color> - Change the colour the selected of figure." << std::endl;
    std::cout << " - move <coordinates> - Move the selected shape to the coordinates." << std::endl;
    std::cout << " - clear - Remove all shapes from blackboard." << std::endl;
    std::cout << " - save <file-path> - Save the blackboard to the file." << std::endl;
    std::cout << " - load <file-path> - Load the blackboard from the file." << std::endl;
    return 18;
}

int CLI::exit_() {
    std::cout << "Exiting CLI..." << std::endl;
    exit(0);
}

int CLI::create(const std::string& input) {
    std::istringstream iss(input);
    std::string cmd = "";
    int w = 0, h = 0;
    iss >> cmd >> w >> h;
    if (iss && !iss.fail()) {
        delete board;
        board = new Board(w, h);
        std::cout << "Created board " + std::to_string(w) + "x" + std::to_string(h) << std::endl;
    } else {
        std::cout << "Error: use create <width> <height>" << std::endl;
    }
    return 1;
}

int CLI::clear() const {
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl;
        return 1;
    }
    board->clear();
    return 0;
}

int CLI::draw() const {
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl;
        return 1;
    }
    board->print();
    return board->height;
}

int CLI::add(const std::string& input) const {
    std::istringstream iss(input);
    std::string cmd = "", shape = "", fill_word = "", color = "";
    int x = 0, y = 0;
    ShapeType type;

    iss >> cmd >> shape;
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl; return 1;
    }
    if (iss.fail()) {
        std::cout << "Error: use add <shape: triangle|box|circle|line>" << std::endl; return 1;
    }
    if (shape == "triangle") type = ShapeType::Triangle;
    else if (shape == "box") type = ShapeType::Box;
    else if (shape == "circle") type = ShapeType::Circle;
    else if (shape == "line") type = ShapeType::Line;
    else {
        std::cout << "Error: unknown shape " + shape << std::endl; return 1;
    }
    if (type == ShapeType::Triangle || type == ShapeType::Box || type == ShapeType::Circle) {
        iss >> fill_word;
        if (fill_word != "fill" && fill_word != "frame") {
            std::cout << "Error: unknown fill " + fill_word << std::endl; return 1;
        }
    }
    iss >> color >> x >> y;
    int fill = (fill_word == "fill");

    std::vector<int> parameters;
    int value;
    while (iss >> value) parameters.push_back(value);

    switch (board->addShape(type, fill, color, x, y, parameters)) {
        case ERROR_INVALID_COLOR:
            std::cout << "Error: unknown color " + color << std::endl; return 1;
        case ERROR_INVISIBLE_SHAPE:
            std::cout << "Error: shape won't be visible." << std::endl; return 1;
        case ERROR_TOO_LARGE_SHAPE:
            std::cout << "Error: shape will cover all screen." << std::endl; return 1;
        case ERROR_DUPLICATE_SHAPE:
            std::cout << "Error: shape will be a duplicate." << std::endl; return 1;
        default:
            board->print(); return board->height;
    }
}

int CLI::unknown(const std::string& input) {
    std::cout << "Unknown command: " << input << std::endl;
    std::cout << "Use >help for clarification" << std::endl;
    return 2;
}

int CLI::list() const {
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl;
        return 1;
    }
    const auto shapes = board->listShapes();
    if (shapes.empty()) {
        std::cout << "No figures on board" << std::endl;
        return 1;
    }
    std::cout << "All figures on board:" << std::endl;
    for (const auto& shape : shapes) {
        std::cout << shape << std::endl;
    }
    return shapes.size();
}

int CLI::shapes() {
    std::cout << "Available shapes:" << std::endl;
    std::cout << "  triangle <fill|frame> <color: red|green|blue|white> <x> <y> <height>" << std::endl;
    std::cout << "  box <fill|frame> <color: red|green|blue> <x> <y> <width> <height>" << std::endl;
    std::cout << "  circle <fill|frame> <color: red|green|blue> <x> <y> <radius>" << std::endl;
    std::cout << "  line color: red|green|blue> <x> <y> <x2> <y2>" << std::endl;
    return 4;
}

int CLI::select(const std::string &input) const {
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl; return 1;
    }
    std::istringstream iss(input);
    std::string cmd = "";
    iss >> cmd;
    std::vector<int> parameters;
    int value = 0;
    while (iss >> value) parameters.push_back(value);
    switch (parameters.size()) {
        case SELECT_BY_ID_CMD_SIZE:
            if (board->selectShape(parameters[0])) std::cout << "Error: shape with such id does not exist." << std::endl; break;
        case SELECT_BY_COORDINATES_CMD_SIZE:
            if (board->selectShape(parameters[0], parameters[1])) std::cout << "Error: shape with those coordinates does not exist." << std::endl; break;
        default:
            std::cout << "Error: wrong parameters" << std::endl;
    }
    return 1;
}

int CLI::remove() const {
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl; return 1;
    }
    if (board->removeShape() == ERROR_NO_SELECTION) {
        std::cout << "Error: select a shape first." << std::endl; return 1;
    }
    return draw();
}

int CLI::paint(const std::string& input) const {
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl; return -1;
    }
    std::istringstream iss(input);
    std::string cmd = "", color = "";
    iss >> cmd >> color;
    switch (board->paintShape(color)) {
        case ERROR_NO_SELECTION:
            std::cout << "Error: select a shape first." << std::endl; return 1;
        case ERROR_INVALID_COLOR:
            std::cout << "Error: unknown color - " << color << std::endl; return 1;
        case ERROR_DUPLICATE_SHAPE:
            std::cout << "Error: shape will be a duplicate." << std::endl; return 1;
        default:
            return draw();
    }
}

int CLI::edit(const std::string& input) const {
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl; return 1;
    }
    std::istringstream iss(input);
    std::string cmd = "";
    iss >> cmd;
    std::vector<int> parameters;
    int value = 0;
    while (iss >> value) parameters.push_back(value);
    switch (board->editShape(parameters)) {
        case ERROR_NO_SELECTION:
            std::cout << "Error: select a shape first." << std::endl; return 1;
        case ERROR_INVALID_ARGUMENTS:
            std::cout << "Error: invalid argument count." << std::endl; return 1;
        case ERROR_INVISIBLE_SHAPE:
            std::cout << "Error: shape won't be visible." << std::endl; return 1;
        case ERROR_TOO_LARGE_SHAPE:
            std::cout << "Error: shape will cover all screen" << std::endl; return 1;
        case ERROR_DUPLICATE_SHAPE:
            std::cout << "Error: shape will be a duplicate." << std::endl; return 1;
        default:
            return draw();
    }
}

int CLI::move(const std::string& input) const {
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl; return 1;
    }
    std::istringstream iss(input);
    std::string cmd = "";
    iss >> cmd;
    std::vector<int> parameters;
    int value = 0;
    while (iss >> value) parameters.push_back(value);
    switch (board->moveShape(parameters)) {
        case ERROR_NO_SELECTION:
            std::cout << "Error: select a shape first." << std::endl; return 1;
        case ERROR_INVALID_ARGUMENTS:
            std::cout << "Error: invalid argument count." << std::endl; return 1;
        case ERROR_INVISIBLE_SHAPE:
            std::cout << "Error: shape won't be visible." << std::endl; return 1;
        case ERROR_TOO_LARGE_SHAPE:
            std::cout << "Error: shape will cover all screen" << std::endl; return 1;
        case ERROR_DUPLICATE_SHAPE:
            std::cout << "Error: shape will be a duplicate." << std::endl; return 1;
        default:
            return draw();
    }
}

int CLI::save(const std::string& input) const {
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl; return 1;
    }
    std::istringstream iss(input);
    std::string cmd = "", filepath = "";
    iss >> cmd >> filepath;
    if (board->saveToFile(filepath)) std::cout << "Error: saving file failed." << std::endl;
    else std::cout << "Successfully saved to file " << filepath << std::endl;
    return 1;
}

int CLI::load(const std::string& input) {
    std::string cmd = "", filepath = "";
    std::istringstream iss(input);
    iss >> cmd >> filepath;
    auto new_board = Board::loadFromFile(filepath);
    if (!new_board) return 1;
    delete board;
    board = new_board.release();
    std::cout << "Successfully loaded board from " << filepath << std::endl;
    return 1;
}

int CLI::color(const std::string& input) const {
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl; return 1;
    }
    std::istringstream iss(input);
    std::string cmd = "", name = "";
    int r = 0, g = 0, b = 0;
    iss >> cmd >> name >> r >> g >> b;
    if (iss.fail()) std::cout << "Error: invalid parameters, usage: color <name> <r> <g> <b>." << std::endl;
    else if (r<0 || r>256) std::cout << "Error: invalid r value, should be 0 between 255." << std::endl;
    else if (g<0 || g>256) std::cout << "Error: invalid g value, should be 0 between 255." << std::endl;
    else if (b<0 || b>256) std::cout << "Error: invalid b value, should be 0 between 255." << std::endl;
    else if (board->addColor(name, r, g, b)) std::cout << "Error: color with that name already exists." << std::endl;
    else std::cout << "Successfully added color " << name << std::endl;
    return 1;
}

int CLI::colors() const {
    if (board == nullptr) {
        std::cout << "Error: board doesn't exist." << std::endl; return 1;
    }
    const auto colors = board->listColors();
    if (colors.empty()) {
        std::cout << "No colors found." << std::endl; return 1;
    }
    std::cout << "Colors:" << std::endl;
    for (const auto& c : colors) std::cout << c << std::endl;
    return colors.size() + 1;
}
