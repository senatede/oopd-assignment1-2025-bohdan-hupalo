#ifndef COLOR_H
#define COLOR_H

class Color {
    public:
    std::string name;
    int r, g, b;

    Color(std::string  name, const int r, const int g, const int b) : name(std::move(name)), r(r), g(g), b(b) {}
    ~Color() = default;

    std::string info() const {
        return "  - " + name + ": " + "(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ")";
    }

    bool operator==(const Color &other) const {
        return name == other.name && r == other.r && g == other.g && b == other.b;
    }
};

#endif //COLOR_H
