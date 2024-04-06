#include "strategy.h"
#include <fstream>
#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

// Implement the RandomStrategy class
Move RandomStrategy::chooseMove() { return Move::fromInt(rand() % 3); };

// Implement the PredictiveStrategy class

// The opponents move will always be the last move
void PredictiveStrategy::updateHistory(Move playerMove, Move opponentMove) {
    // Update the current sequence
    currentSequence += opponentMove.toChar();

    // Ensure the current sequence does not exceed the specified length
    if ((int)currentSequence.length() > sequenceLength) {
        currentSequence.erase(0, currentSequence.length() - sequenceLength);
    }

    // If the sequence is at full length, update the count
    if ((int)currentSequence.length() == sequenceLength) {
        sequences[currentSequence]++;
        save();
    }

    currentSequence += playerMove.toChar();
};

void PredictiveStrategy::load() {
    std::ifstream file("./output/" + fileName);
    std::string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            std::string sequence = line.substr(0, line.find(':'));
            int count = std::stoi(line.substr(line.find(':') + 1));
            sequences[sequence] = count;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file for reading." << std::endl;
    }
};

void PredictiveStrategy::save() {
    std::ofstream file("./output/" + fileName);
    if (file.is_open()) {
        for (const auto &pair : sequences) {
            file << pair.first << ":" << pair.second << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
};

int PredictiveStrategy::getSequenceCount(std::string sequence) {
    if (sequences.find(sequence) != sequences.end()) {
        return sequences.at(sequence);
    }
    return 0; // If the sequence does not exist, return 0
};

/* Implement PredictiveStrategy::chooseMove
    Should check if there are any sequences that match the current
*/
Move PredictiveStrategy::chooseMove() {
    // If the current sequence is not long enough, return a random move
    if ((int)currentSequence.length() < sequenceLength) {
        return Move::fromInt(rand() % 3);
    }

    // Get the last sequenceLength - 1 characters of the current sequence
    std::string lastSequence =
        currentSequence.substr(currentSequence.length() - sequenceLength + 1);

    // Get the count of the last sequence + R, P, and S
    int rCount = getSequenceCount(lastSequence + "R");
    int pCount = getSequenceCount(lastSequence + "P");
    int sCount = getSequenceCount(lastSequence + "S");

    // If the counts are all 0, return a random move
    if (rCount == 0 && pCount == 0 && sCount == 0) {
        return Move::fromInt(rand() % 3);
    }

    // If the counts are not all 0, return the move that would beat the opponents
    // most common move
    if (rCount >= pCount && rCount >= sCount) {
        return Move::PAPER;
    } else if (pCount >= rCount && pCount >= sCount) {
        return Move::SCISSORS;
    } else {
        return Move::ROCK;
    }

    // Return random move
    return Move::fromInt(rand() % 3);
}
