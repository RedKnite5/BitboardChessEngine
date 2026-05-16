#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

#include "draggable.h"
#include "board.h"
#include "search.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Game &game;
    std::array<DraggableLabel*, 64> squares;
    QPixmap *lastTakenPieceImage = nullptr;
    QThread *thread = nullptr;
    int player_color = 1;  // 1 for white; 0 for black
    std::shared_ptr<DragDropState> DDState;

    MainWindow(Game &aGame, QWidget *parent = nullptr);
    ~MainWindow();
    std::array<DraggableLabel*, 64> setupChessboard(QWidget *parent);
    void engineTurn(int move);
    void getPlayerMove(GuiMove);
    void makePopup(DraggableLabel *dest, std::function<void(int)> callback);
    void playerMove(int move, int promotion, GuiMove guimove);
    void enpassant(int move);
    void castle(int move);

signals:
    void RequestEngineMove(Game state);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
