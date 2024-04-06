#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include <iostream>
#include <string>

struct Move {
    enum Type { NONE, ROCK, PAPER, SCISSORS };

    Type t_;
    Move(Type t) : t_(t) {}
    operator Type() const { return t_; }

    bool operator==(const Move &other) const { return t_ == other.t_; }
    bool operator==(const Type &other) const { return t_ == other; }

    std::string toString() const {
        switch (t_) {
        case ROCK:
            return "Rock";
        case PAPER:
            return "Paper";
        case SCISSORS:
            return "Scissors";
        default:
            return "Unknown";
        }
    }

    char toChar() const {
        switch (t_) {
        case ROCK:
            return 'R';
        case PAPER:
            return 'P';
        case SCISSORS:
            return 'S';
        default:
            return '?';
        }
    }

    static Move fromInt(int value) {
        switch (value) {
        case 0:
            return Move::ROCK;
        case 1:
            return Move::PAPER;
        case 2:
            return Move::SCISSORS;
        default:
            throw std::invalid_argument("Invalid value for Move");
        }
    }

private:
    template <typename T> operator Type() const;
};

#endif
