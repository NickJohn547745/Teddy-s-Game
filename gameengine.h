#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QTime>

#include <QDebug>
#include <QtMath>
#include <QTimer>

#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>

struct Player {
    double velocityX = 0.0;
    double velocityY = 0.0;

    QPoint pos;

    QPoint previousPos;
};

class GameEngine : public QFrame
{
    Q_OBJECT
public:
    GameEngine(QWidget *parent);
    void init();

protected:
    void paintEvent(QPaintEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

public slots:
    void processMovement();
    void updatePosition();

private:
    QMap<int, bool> keys;
    QTimer *movementTimer;
    Player *player;

    QList<QRect> collisionList;
};

#endif // GAMEENGINE_H
