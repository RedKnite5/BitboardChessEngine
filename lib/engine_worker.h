#pragma once

#include <QObject>

#include "search.h"

class EngineWorker : public QObject {
    Q_OBJECT
public slots:
    void calculateMove(Game state);  // whatever params you need
signals:
    void moveReady(int move);            // emitted when engine is done
};

