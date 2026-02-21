#ifndef CLI_H
#define CLI_H

#include <sstream>
#include <string>
#include "Board.h"

class CLI {
    int term_height = 0;
    Board* board = nullptr;

    static std::string command_input();
    void command_handling(const std::string& input);
    static int help() ;
    static int exit_() ;
    int create(const std::string& input);
    int clear() const;
    int draw() const;
    int list() const;
    static int shapes() ;
    int add(const std::string& input) const;
    int select(const std::string& input) const;
    int remove() const;
    int paint(const std::string& input) const;
    int edit(const std::string& input) const;
    int move(const std::string& input) const;
    int save(const std::string& input) const;
    int load(const std::string& input);
    static int unknown(const std::string& input);

    int color(const std::string& input) const;
    int colors() const;

public:
    explicit CLI(int h);
    [[noreturn]] void run();
};

#endif //CLI_H
