#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "move.h"
#include "strategy.h"
#include <memory> // For std::unique_ptr
#include <string>

class GameObserver {
public:
    virtual void update(Move playerMove, Move opponentMove) = 0;
    virtual ~GameObserver() {}
};

class Player {
protected:
    std::string name;

public:
    Player(std::string name) : name(name) {}
    virtual ~Player() {}
    virtual std::string getName() const = 0; // Pure virtual function
    virtual Move makeMove() = 0;             // Pure virtual function
    virtual void setMove(Move move) = 0;     // Pure virtual function
};

// HumanPlayer class
class HumanPlayer : public Player {
private:
    Move nextMove;

public:
    HumanPlayer(std::string name = "Human") : Player(name), nextMove(Move::NONE) {}

    std::string getName() const override { return name; }
    // Move makeMove() override { return nextMove; }
    // void setMove(Move move) override { nextMove = move; }

    Move makeMove() override;
    void setMove(Move move) override;
};

// ComputerPlayer class
class ComputerPlayer : public Player, public GameObserver {
private:
    std::unique_ptr<Strategy> strategy;

public:
    // Constructors that accept raw pointers and convert to unique_ptr
    ComputerPlayer(std::unique_ptr<Strategy> strategy)
        : Player("Computer"), strategy(std::move(strategy)) {}
    ComputerPlayer(std::string name, std::unique_ptr<Strategy> strategy)
        : Player(name), strategy(std::move(strategy)) {}

    void setStrategy(std::unique_ptr<Strategy> newStrategy) {
        strategy = std::move(newStrategy);
    }

    std::string getName() const override { return name; }
    Move makeMove() override { return strategy->chooseMove(); }
    void setMove(Move) override { /* ComputerPlayer may not need to implement setMove */ }

    void update(Move playerMove, Move opponentMove) override {
        if (strategy) {
            strategy->updateHistory(playerMove, opponentMove);
        }
    }
};

#endif // PLAYER_H_INCLUDED





/*#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "move.h"
#include "strategy.h"
#include <memory> // For std::unique_ptr
#include <string>

class GameObserver {
public:
    virtual void update(Move playerMove, Move opponentMove) = 0;
    virtual ~GameObserver() {}
};

class Player {
protected:
    std::string name;

public:
    Player(std::string name) : name(name) {}
    virtual ~Player() {}
    virtual std::string getName() = 0; // Pure virtual function
    virtual Move makeMove() = 0;       // Pure virtual function
    virtual void setMove(Move move) = 0;
};

// HumanPlayer class
class HumanPlayer : public Player {
public:
    HumanPlayer() : Player("Human"), nextMove(Move::NONE) {}
    HumanPlayer(std::string name) : Player(name), nextMove(Move::NONE) {}
    std::string getName() override { return name; };
    Move makeMove() override;
    void setMove(Move move);
private:
    Move nextMove;
};

// ComputerPlayer class
class ComputerPlayer : public Player, public GameObserver {
private:
    std::unique_ptr<Strategy> strategy;

public:
    ComputerPlayer(Strategy *strategy)
        : Player(strategy->stratName), strategy(std::move(strategy)) {}
    ComputerPlayer(std::string name, Strategy *strategy)
        : Player(name), strategy(std::move(strategy)) {}

    std::string getName() override { return name; };
    Move makeMove() override { return strategy->chooseMove(); }

    void update(Move playerMove, Move opponentMove) override {
        if (strategy)
            strategy->updateHistory(playerMove, opponentMove);
    }
};

#endif
*/
