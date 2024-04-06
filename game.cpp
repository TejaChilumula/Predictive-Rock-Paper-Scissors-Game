#include "game.h"
#include "move.h"

void StatisticsTracker::update(Move p1Move, Move p2Move) {
    if (p1Move == p2Move) {
        wins[2]++;
    } else if ((p1Move == Move::ROCK && p2Move == Move::SCISSORS) ||
               (p1Move == Move::PAPER && p2Move == Move::ROCK) ||
               (p1Move == Move::SCISSORS && p2Move == Move::PAPER)) {
        wins[0]++;
    } else {
        wins[1]++;
    }
}

void StatisticsTracker::save() {
    std::ofstream file("./output/" + fileName);
    file << wins[0] << " " << wins[1] << " " << wins[2] << std::endl;
    file.close();
}

void StatisticsTracker::load() {
    std::ifstream file("./output/" + fileName);
    if (file) {
        file >> wins[0] >> wins[1] >> wins[2];
    }
    file.close();
}

void Game::playRound() {
    Move player1Move = player1->makeMove();
    Move player2Move = player2->makeMove();

    GameObserver *observer1 = dynamic_cast<GameObserver *>(player1);
    if (observer1) {
        observer1->update(player1Move, player2Move);
    }

    GameObserver *observer2 = dynamic_cast<GameObserver *>(player2);
    if (observer2) {
        observer2->update(player2Move, player1Move);
    }

    if (player1Move == player2Move) {
        tieScore++;
    } else if ((player1Move == Move::ROCK && player2Move == Move::SCISSORS) ||
               (player1Move == Move::PAPER && player2Move == Move::ROCK) ||
               (player1Move == Move::SCISSORS && player2Move == Move::PAPER)) {
        humanScore++;
    } else {
        computerScore++;
    }

    currentRound++;
    stats->update(player1Move, player2Move);
}

void Game::printResults() {
    std::cout << "Final Results:\n";
    std::cout << player1->getName() << " wins: " << humanScore << std::endl;
    std::cout << player2->getName() << " wins: " << computerScore << std::endl;
    std::cout << "Ties: " << tieScore << std::endl;
}

void Game::playGame() {
    for (int i = 0; i < 20; i++) {
        std::cout << "Round " << i + 1 << "...\n";
        playRound();
        std::cout << "\n" << std::endl;
    }
    stats->save();
    printResults();
}

int Game::getCurrentRound() const {
    return currentRound;
}

int Game::getHumanScore() const {
    return humanScore;
}

int Game::getComputerScore() const {
    return computerScore;
}

int Game::getTieScore() const {
    return tieScore;
}

Move Game::getHumanMove() const {
    return lastHumanMove;
}

Move Game::getComputerMove() const {
    return lastComputerMove;
}

 bool Game::isGameOver() const {
     // Implement logic to determine if the game is over
     // Example: return currentRound >= totalRounds;
     return currentRound >= totalRounds;
 }
