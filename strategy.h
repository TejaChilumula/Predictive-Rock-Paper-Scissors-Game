#ifndef STRATEGY_H
#define STRATEGY_H

#include <QMainWindow>
#include <QObject>
#include <QSharedDataPointer>
#include <QWidget>


#include "move.h"
#include <map>

// Strategy interface
class Strategy {
public:
    std::string stratName;
    virtual Move chooseMove() = 0;
    virtual void updateHistory(Move playerMove, Move opponentMove) {}
    virtual ~Strategy() {}
};

// Concrete Strategy: RandomStrategy
class RandomStrategy : public Strategy {
public:
    RandomStrategy() { stratName = "Random"; }
    Move chooseMove() override;
};

// Concrete Strategy: PredictiveStrategy
class PredictiveStrategy : public Strategy {
private:
    std::map<std::string, int> sequences;
    std::string currentSequence = "";
    int sequenceLength;
    std::string fileName;

public:
    PredictiveStrategy(int sequenceLength, std::string fileName)
        : sequenceLength(sequenceLength), fileName(fileName) {
        stratName = "Predictive";
        load();
    }

    PredictiveStrategy() {
        sequenceLength = 5;
        fileName = "smart-output.txt";
        stratName = "Predictive";
        load();
    }

    Move chooseMove() override;
    void load();
    void save();
    void updateHistory(Move playerMove, Move opponentMove) override;
    int getSequenceCount(std::string sequence);
};

#endif
