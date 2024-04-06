#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "strategy.h" // Make sure this includes RandomStrategy and PredictiveStrategy
//#include "iostream.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentRound(0), totalRounds(0) {
    ui->setupUi(this);
    ui->labelHumanChoice->clear();
    ui->labelHumanChoice_2->clear();
    ui->labelComputerChoice->clear();
    ui->labelWinner->clear();
    ui->labelComputerWins->setText("Computer wins: 0");
    ui->labelHumanWins->setText("Human wins: 0");
    ui->labelTies->setText("Ties: 0");


    enum class Outcome {
        HumanWins,
        ComputerWins,
        Tie
    };

    // Assuming HumanPlayer and ComputerPlayer have default constructors
    humanPlayer = std::make_unique<HumanPlayer>();
    std::unique_ptr<Strategy> strategy = std::make_unique<RandomStrategy>();
    std::unique_ptr<Player> computerPlayer = std::make_unique<ComputerPlayer>(std::move(strategy));


    // Connect signals and slots

    connect(ui->buttonRock, &QPushButton::clicked, this, &MainWindow::on_buttonRock_clicked);
    connect(ui->buttonPaper, &QPushButton::clicked, this, &MainWindow::on_buttonPaper_clicked);
    connect(ui->buttonScissors, &QPushButton::clicked, this, &MainWindow::on_buttonScissors_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBox_currentIndexChanged);
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::on_spinBox_valueChanged);

    // Set the initial number of rounds
    on_spinBox_valueChanged(ui->spinBox->value());
    //resetUI();
}

MainWindow::~MainWindow() {
    delete ui;
}





void MainWindow::onHumanPlay() {
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    if (!buttonSender) return;

    // Translate button press to Move enum
    Move humanMove = Move::NONE;
    if (buttonSender == ui->buttonRock) {
        humanMove = Move::ROCK;
    } else if (buttonSender == ui->buttonPaper) {
        humanMove = Move::PAPER;
    } else if (buttonSender == ui->buttonScissors) {
        humanMove = Move::SCISSORS;
    }

    // Update human move in UI
    ui->labelHumanChoice->setText(moveToString(humanMove));

    // Get computer move based on strategy
    Move computerMove = game->getComputerMove();

    // Update computer move in UI
    ui->labelComputerChoice->setText(moveToString(computerMove));

    // Play the round and update UI
    playRound(humanMove);
}

void MainWindow::playRound(Move humanMove) {
    // Simulate the human and computer making their moves
    Move computerMove = computerPlayer->makeMove();

    qDebug() << "computer move clicked.";

    // Assuming the existence of a GameObserver interface and that Player classes implement this
    dynamic_cast<GameObserver*>(humanPlayer.get())->update(humanMove, computerMove);
    dynamic_cast<GameObserver*>(computerPlayer.get())->update(computerMove, humanMove);

    // Update the UI with the moves
    ui->labelHumanChoice->setText(moveToString(humanMove));
    ui->labelComputerChoice->setText(moveToString(computerMove));

    // Determine the outcome
    Outcome outcome;
    if (humanMove == computerMove) {
        outcome = Outcome::Tie;
        ui->labelWinner->setText("It's a tie!");
    } else if ((humanMove == Move::ROCK && computerMove == Move::SCISSORS) ||
               (humanMove == Move::PAPER && computerMove == Move::ROCK) ||
               (humanMove == Move::SCISSORS && computerMove == Move::PAPER)) {
        outcome = Outcome::HumanWins;
        ui->labelWinner->setText(QString("%1 wins the round!").arg(humanPlayer->getName().c_str()));
    } else {
        outcome = Outcome::ComputerWins;
        ui->labelWinner->setText(QString("%1 wins the round!").arg(computerPlayer->getName().c_str()));
    }

    // Update scores and check for game over
    updateScores(outcome);
    checkGameOver();
}

/*
void MainWindow::playRound(Move humanMove) {


    Move computerMove = computerPlayer->makeMove();

    ui->labelHumanChoice->setText(moveToString(humanMove));
    ui->labelComputerChoice->setText(moveToString(computerMove));

    qDebug() << "HumanMove.";
    qDebug() << "CompMove."<<computerMove;


    // Determine the outcome of the round
    // This is a simple rule: Rock beats Scissors, Scissors beats Paper, Paper beats Rock
    Outcome outcome;
    if (humanMove == computerMove) {
        outcome = Outcome::Tie; // Enum value representing a tie
    } else if ((humanMove == Move::ROCK && computerMove == Move::SCISSORS) ||
               (humanMove == Move::SCISSORS && computerMove == Move::PAPER) ||
               (humanMove == Move::PAPER && computerMove == Move::ROCK)) {
        outcome = Outcome::HumanWins; // Enum value representing a human win
    } else {
        outcome = Outcome::ComputerWins; // Enum value representing a computer win
    }

    // Update the scores based on the outcome
    if (outcome == Outcome::HumanWins) {
        humanScore++; // Assuming humanScore is an integer member variable
    } else if (outcome == Outcome::ComputerWins) {
        computerScore++; // Assuming computerScore is an integer member variable
    } else {
        ties++; // Assuming ties is an integer member variable
    }

    // Update the UI based on the outcome
    updateScores();
    checkGameOver();
}
*/
void MainWindow::updateButtonsState(bool enabled) {
    ui->buttonRock->setEnabled(enabled);
    ui->buttonPaper->setEnabled(enabled);
    ui->buttonScissors->setEnabled(enabled);
}

void MainWindow::onStartNewGameClicked() {
    // Reset the game state and UI
    //updateUI();
    qDebug() << "button clicked.";
    resetGame();
    resetUI();
}

void MainWindow::resetUI() {
    ui->labelHumanChoice->clear();
    ui->labelComputerChoice->clear();
    ui->labelWinner->clear();
    ui->labelComputerWins->setText("Computer wins: 0");
    ui->labelHumanWins->setText("Human wins: 0");
    ui->labelTies->setText("Ties: 0");
    ui->round_val->setText("1"); // Start from round 1
}

void MainWindow::resetGame() {
    // Reset game state, such as scores and rounds
    humanScore = 0;
    computerScore = 0;
    ties = 0;
    currentRound = 0;

    // Set up strategy for the computer based on the combobox selection
    std::unique_ptr<Strategy> strategy;
    if (ui->comboBox->currentText() == "Smarter") {
        strategy = std::make_unique<PredictiveStrategy>();
    } else {
        strategy = std::make_unique<RandomStrategy>();
    }
    computerPlayer->setStrategy(std::move(strategy));

    // Reset the total number of rounds based on the spinbox value
    totalRounds = 0;

    // Reset the spinbox value to its initial state
    ui->spinBox->setValue(0);
}



// void MainWindow::processPlayerMove(Move move) {
//     // Logic to process player's move
// }






void MainWindow::on_pushButton_clicked() {
    // Start a new game
    // Reset scores and other game state here
   // updateScores();
    currentRound = 0;
    totalRounds = ui->spinBox->value();
    ui->labelComputerWins->setText("Computer wins: 0");
    ui->labelHumanWins->setText("Human wins: 0");
    ui->labelTies->setText("Ties: 0");
    ui->labelHumanChoice->setText("");
    ui->labelComputerChoice->setText("");
    ui->labelWinner->setText("");

    totalRounds = 0;

    // Reset the spinbox value to its initial state
    ui->spinBox->setValue(0);

    qDebug() << "Paper button clicked.";
}

void MainWindow::on_comboBox_currentIndexChanged(int index) {

        // Ensure the game is not in an inconsistent state when changing strategy
        if (totalRounds > 0 && currentRound < totalRounds) {
            QMessageBox::warning(this, "Warning", "Cannot change strategy during an active game. Please finish the game or restart.");
            return; // Exit the method to avoid changing the strategy mid-game
        }

        // Change computer strategy based on the user selection
        try {
            if (index == 0) { // Assuming 0 is for PredictiveStrategy
                computerPlayer->setStrategy(std::make_unique<PredictiveStrategy>());
            } else { // For RandomStrategy or any other default strategy
                qDebug() << "Random choice";
                computerPlayer->setStrategy(std::make_unique<RandomStrategy>());
            }
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to change strategy: %1").arg(e.what()));
            return;
        }

        // Reset the game to apply the new strategy cleanly
        resetGame();
    // }



    // // Change computer strategy based on the user selection
    // // Update the computerPlayer's strategy
    // if (index == 0) {
    //     computerPlayer->setStrategy(std::make_unique<PredictiveStrategy>());
    // } else {
    //     computerPlayer->setStrategy(std::make_unique<RandomStrategy>());
    // }

    // // Potentially reset the game here as well
    // resetGame();
}

void MainWindow::on_spinBox_valueChanged(int rounds) {
    totalRounds = rounds;
    updateButtonsState(rounds > 0);
}

// void MainWindow::updateScores() {
//     ui->labelHumanWins->setText(QString("Human wins: %1").arg(humanScore));
//     ui->labelComputerWins->setText(QString("Computer wins: %1").arg(computerScore));
//     ui->labelTies->setText(QString("Ties: %1").arg(ties));
// }


void MainWindow::updateScores(Outcome outcome) {
    switch (outcome) {
    case Outcome::HumanWins:
        humanScore++;
        ui->labelHumanWins->setText(QString("Human wins: %1").arg(humanScore));
        break;
    case Outcome::ComputerWins:
        computerScore++;
        ui->labelComputerWins->setText(QString("Computer wins: %1").arg(computerScore));
        break;
    case Outcome::Tie:
        ties++;
        ui->labelTies->setText(QString("Ties: %1").arg(ties));
        break;
    }
}


void MainWindow::checkGameOver() {
    if (currentRound >= totalRounds) {
        QString winnerText;
        if (humanScore > computerScore) {
            winnerText = "Human wins the game!";
        } else if (computerScore > humanScore) {
            winnerText = "Computer wins the game!";
        } else {
            winnerText = "The game is a tie!";
        }
        ui->labelWinner->setText(winnerText);
        // Additional logic to handle end-of-game scenario
    }
}
void MainWindow::updateUI() {
    // Update the UI components like scores and moves
    ui->labelComputerWins->setText(QString("Computer wins: %1").arg(computerScore));
    ui->labelHumanWins->setText(QString("Human wins: %1").arg(humanScore));
    ui->labelTies->setText(QString("Ties: %1").arg(ties));
    ui->round_val->setText(QString::number(currentRound));
}

// void MainWindow::playRound(Move playerMove) {
//     // Logic to play a round based on the human's move
//     // This should invoke game logic and then update the UI
//     updateUI();
// }



void MainWindow::on_buttonRock_clicked() {
    qDebug() << "Paper button clicked.";
    handleHumanMove(Move::ROCK);
}

void MainWindow::on_buttonPaper_clicked() {
    handleHumanMove(Move::PAPER);
}

void MainWindow::on_buttonScissors_clicked() {
    handleHumanMove(Move::SCISSORS);
}

void MainWindow::handleHumanMove(Move humanMove) {
    // Update the UI to show the human player's move
    ui->labelHumanChoice->setText(moveToString(humanMove));

    // Get the computer's move based on the selected strategy
    Move computerMove = computerPlayer->makeMove();

    // Update the UI to show the computer's move (optional)
    ui->labelComputerChoice->setText(moveToString(computerMove));

    // Play the round and update the game state
    playRound(humanMove);
}


QString MainWindow::moveToString(Move move) {
    switch (static_cast<Move::Type>(move)) {
    case Move::ROCK:
        return "Rock";
    case Move::PAPER:
        return "Paper";
    case Move::SCISSORS:
        return "Scissors";
    default:
        return "Unknown";
    }
}

//QString MainWindow::moveToString






/*#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "player.h"
#include "game.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentRound(0)
    , computerScore(0)
    , humanScore(0)
    , tieScore(0) {
    ui->setupUi(this);

    // Create the strategy for the computer player
    auto strategy = std::make_unique<RandomStrategy>();

    // Create the computer player with the strategy
    computerPlayer = std::make_unique<ComputerPlayer>(std::move(strategy));

    // Create the human player
    humanPlayer = std::make_unique<HumanPlayer>();

    // Initialize the game with both players
    game = std::make_unique<Game>(humanPlayer.get(), computerPlayer.get());

    // Setup UI signals and slots connections
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_startNewGame_clicked);
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox_valueChanged(int)));
    connect(ui->buttonRock, &QPushButton::clicked, this, &MainWindow::on_buttonRock_clicked);
    connect(ui->buttonPaper, &QPushButton::clicked, this, &MainWindow::on_buttonPaper_clicked);
    connect(ui->buttonScissors, &QPushButton::clicked, this, &MainWindow::on_buttonScissors_clicked);

    // Initialize the game state
    on_startNewGame_clicked();
}

MainWindow::~MainWindow() {
    delete ui;
}

// Define all other member functions here

void MainWindow::on_startNewGame_clicked() {
    // Reset the game state
    currentRound = 0;
    computerScore = 0;
    humanScore = 0;
    tieScore = 0;

    totalRounds = ui->spinBox->value();
    game->reset(totalRounds); // Assuming the Game class has a reset method

    updateUI();
}

void MainWindow::on_spinBox_valueChanged(int rounds) {
    totalRounds = rounds;
}

void MainWindow::updateRound() {
    // Implementation details...
}

void MainWindow::on_buttonRock_clicked() {
    humanPlayer->setMove(Move::ROCK);
    playRoundAndUpdateUI();
}

void MainWindow::on_buttonPaper_clicked() {
    humanPlayer->setMove(Move::PAPER);
    playRoundAndUpdateUI();
}

void MainWindow::on_buttonScissors_clicked() {
    humanPlayer->setMove(Move::SCISSORS);
    playRoundAndUpdateUI();
}

void MainWindow::playRoundAndUpdateUI() {
    if (game) {
        game->playRound();
        updateUI();
    }
}

void MainWindow::updateUI() {
    // Assuming your Game class has methods like getCurrentRound, getHumanScore, getComputerScore, getTieScore, getHumanMove, getComputerMove, etc.
    // Update the round label
    ui->round_val->setText(QString::number(game.getCurrentRound()));

    // Update the scores
    ui->labelComputerWins->setText(QString("Computer wins: %1").arg(game.getComputerScore()));
    ui->labelHumanWins->setText(QString("Human wins: %1").arg(game.getHumanScore()));
    ui->labelTies->setText(QString("Ties: %1").arg(game.getTieScore()));

    // Update the move choices
    // You would need to convert the enum or whatever type you're using for moves to a string
    ui->labelHumanChoice->setText(QString("Human chooses: %1").arg(moveToString(game.getHumanMove())));
    ui->labelComputerChoice->setText(QString("Computer chooses: %1").arg(moveToString(game.getComputerMove())));

    // Check if the game has a winner and update the winner label
    if (game.isGameOver()) {
        QString winnerText;
        if (game.getHumanScore() > game.getComputerScore()) {
            winnerText = "Human wins!";
        } else if (game.getComputerScore() > game.getHumanScore()) {
            winnerText = "Computer wins!";
        } else {
            // You can implement additional logic to handle tie-breaking if necessary
            winnerText = "It's a tie!";
        }
        ui->labelWinner->setText(QString("The winner: %1").arg(winnerText));
    }
}

QString MainWindow::moveToString(Move move) {
    // This function assumes that Move is an enum class within your Game or Move class
    switch (static_cast<Move::Type>(move)) {
    case Move::Type::ROCK:
        return "Rock";
    case Move::Type::PAPER:
        return "Paper";
    case Move::Type::SCISSORS:
        return "Scissors";
    default:
        return "Unknown";
    }
}

// Add member variable declarations in your header file
// int currentRound;
// int computerScore;
// int humanScore;
// int tieScore;
*/
