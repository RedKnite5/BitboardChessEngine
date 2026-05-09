#pragma once

#include <QObject>

#include "search.h"

class EngineWorker : public QObject {
    Q_OBJECT
public slots:
    void calculateMove(Game state);
signals:
    void moveReady(int move);
};

