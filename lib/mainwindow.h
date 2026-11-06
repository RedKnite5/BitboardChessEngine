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
    uint64_t highlighted = 0;

    MainWindow(Game &aGame, int aDepth, QWidget *parent = nullptr);
    ~MainWindow();
    std::array<DraggableLabel*, 64> setupChessboard(QWidget *parent);
    int depth;
    void engineTurn(Move move);
    void getPlayerMove(GuiMove);
    void makePopup(DraggableLabel *dest, std::function<void(int)> callback);
    void playerMove(Move move, int promotion, GuiMove guimove);
    void enpassant(Move move);
    void castle(Move move);
    void clearHighlights();
    void highlight(int square);

signals:
    void RequestEngineMove(Game state, int depth);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
