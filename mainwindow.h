#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "player.h" // Make sure this includes HumanPlayer and ComputerPlayer
#include "game.h" // Make sure this includes your Game class

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateUI();

    enum class Outcome {
        HumanWins,
        ComputerWins,
        Tie
    };


private slots:
    void onHumanPlay();
    void playRound(Move humanMove);
    void onStartNewGameClicked();
    //void updateUI();
    void resetUI();
    void resetGame();
    void updateScores(Outcome outcome);
    void checkGameOver();
    void on_spinBox_valueChanged(int rounds);
    void on_comboBox_currentIndexChanged(int index);
    void on_pushButton_clicked();
    void on_buttonRock_clicked();
    void on_buttonPaper_clicked();
    void on_buttonScissors_clicked();
    QString moveToString(Move move);
    void updateButtonsState(bool enabled);
    void handleHumanMove(Move humanMove);
private:
    Ui::MainWindow *ui;
    std::unique_ptr<Game> game; // Assume Game handles logic
    std::unique_ptr<HumanPlayer> humanPlayer;
    std::unique_ptr<ComputerPlayer> computerPlayer; // Player is a polymorphic base class
    std::unique_ptr<Strategy> strategy; // For computer's strategy
    int totalRounds;
    int currentRound;
    // Helper functions
    int humanScore;
    int computerScore;
    int ties;
    //playRound(Move playerMove)

};

#endif // MAINWINDOW_H




/*#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "player.h" // Replace with the actual name of the header file where HumanPlayer is declared
#include "game.h"
//#include "HumanPlayer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startNewGame_clicked(); // Slot for the "Start New Game" button
    void on_spinBox_valueChanged(int rounds);
    void updateRound();
    void on_buttonRock_clicked();
    void on_buttonPaper_clicked();
    void on_buttonScissors_clicked();
    void playRoundAndUpdateUI();
    void updateUI();
   // Qstring moveToString(Move move)
    QString moveToString(Move move);

private:
    Ui::MainWindow *ui; // Pointer to the UI class for your MainWindow
    // Add member variables to keep track of the game state
    Game game;
    HumanPlayer humanPlayer;
    Player *computerPlayer;
    int totalRounds;
    int currentRound;
    int computerScore;
    int humanScore;
    int tieScore;
    // Add any other game state variables you need here
};

#endif // MAINWINDOW_H
*/
