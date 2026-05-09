#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    std::array<std::array<DraggableLabel*, 8>, 8> squares;
    QPixmap *lastTakenPieceImage = nullptr;
    QThread *thread = nullptr;
    MainWindow(Game &aGame, QWidget *parent = nullptr);
    ~MainWindow();
    void engineTurn(int move);
    void getPlayerMove(GuiMove);
    void makePopup(DraggableLabel *dest, std::function<void(int)> callback);
    void playerMove(int move, int promotion, GuiMove guimove);
    void enpassant(int move);

signals:
    void RequestEngineMove(Game state);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
