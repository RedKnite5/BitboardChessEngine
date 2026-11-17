#pragma once

#include <QObject>

#include "search.h"

class EngineWorker : public QObject {
    Q_OBJECT
public slots:
    void calculateMove(std::shared_ptr<Game> state, int depth);
signals:
    void moveReady(int move);
};

