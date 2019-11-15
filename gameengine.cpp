#include "gameengine.h"

GameEngine::GameEngine(QWidget *parent)
    : QFrame(parent)
{
    setFocus();
    init();
}

void GameEngine::init()
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    player = new Player;
    player->pos = QPoint(parentWidget()->width() / 2, parentWidget()->height() / 2);

    for (int barrierAmount = 0; barrierAmount < 5; barrierAmount++)
    {
        topOfLoop:
        int rD = qrand() % 75 + 25;

        int rX = qrand() % (parentWidget()->width() - rD / 2) + rD / 2;
        int rY = qrand() % (parentWidget()->height() - rD / 2) + rD / 2;


        QRect barrierRect = QRect(rX, rY, rD, rD);

        if (!collisionList.contains(barrierRect))
        {
            foreach (QRect collisionRect, collisionList)
            {
                if (barrierRect.intersects(collisionRect))
                    goto topOfLoop;
            }
            if (!barrierRect.intersects(parentWidget()->rect()))
                goto topOfLoop;
            collisionList << barrierRect;
        }

    }

    movementTimer = new QTimer(this);
    connect(movementTimer, SIGNAL(timeout()), this, SLOT(processMovement()));
    movementTimer->start(100);
}

void GameEngine::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(Qt::black);

    painter.drawRect(rect());

    painter.setBrush(Qt::black);

    /*for (int x = 0; x < width(); x+=5)
    {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < width(); y+=5)
    {
        painter.drawLine(0, y, width(), y);
    }*/

    foreach (QRect collisionRect, collisionList)
    {
        painter.drawEllipse(collisionRect);
    }

    int playerRadius = 15;

    QRect playerBounds = QRect(QPoint(player->pos.x() - playerRadius, player->pos.y() - playerRadius), QSize(2 * playerRadius, 2 * playerRadius));

    if (playerBounds.top() >= 0 && playerBounds.bottom() <= height() && playerBounds.left() >= 0 && playerBounds.right() <= width())
    {
        foreach (QRect collisionRect, collisionList)
        {
            if (!playerBounds.intersects(collisionRect))
                painter.drawEllipse(player->pos, playerRadius, playerRadius);
            else
            {
                painter.drawEllipse(player->previousPos, playerRadius, playerRadius);
                player->pos = player->previousPos;
            }
        }
    }
    else
    {
        painter.drawEllipse(player->previousPos, playerRadius, playerRadius);
        player->pos = player->previousPos;
    }
}

void GameEngine::keyPressEvent(QKeyEvent *event)
{
    int pressedKey = event->key();

    if (!event->isAutoRepeat())
        keys[pressedKey] = true;
}

void GameEngine::keyReleaseEvent(QKeyEvent *event)
{
    int releasedkey = event->key();

    if (!event->isAutoRepeat())
        keys[releasedkey] = false;
}

void GameEngine::processMovement()
{
    //qDebug() << keys[Qt::Key_W] << keys[Qt::Key_A];
    if (keys[Qt::Key_W] && keys[Qt::Key_A])
    {
        player->velocityX = -sqrt(2)/2;
        player->velocityY = -sqrt(2)/2;
    }
    else if (keys[Qt::Key_A] && keys[Qt::Key_S])
    {
        player->velocityX = -sqrt(2)/2;
        player->velocityY = sqrt(2)/2;
    }
    else if (keys[Qt::Key_S] && keys[Qt::Key_D])
    {
        player->velocityX = sqrt(2)/2;
        player->velocityY = sqrt(2)/2;
    }
    else if (keys[Qt::Key_D] && keys[Qt::Key_W])
    {
        player->velocityX = sqrt(2)/2;
        player->velocityY = -sqrt(2)/2;
    }

    else if (keys[Qt::Key_W] && !keys[Qt::Key_S])
        player->velocityY = -1;
    else if (keys[Qt::Key_A] && !keys[Qt::Key_D])
        player->velocityX = -1;
    else if (keys[Qt::Key_S] && !keys[Qt::Key_W])
        player->velocityY = 1;
    else if (keys[Qt::Key_D] && !keys[Qt::Key_A])
        player->velocityX = 1;

    updatePosition();
}

void GameEngine::updatePosition()
{
    double currentVelocityX = player->velocityX * 2;
    double currentVelocityY = player->velocityY * 2;

    int newX = player->pos.x() + static_cast<int>(currentVelocityX);
    int newY = player->pos.y() + static_cast<int>(currentVelocityY);

    //qDebug() << player->velocityX << " - " << player->velocityY;
    if (player->pos != QPoint(newX, newY))
        player->previousPos = player->pos;

    player->pos.setX(newX);
    player->pos.setY(newY);

    player->velocityX = 0;
    player->velocityY = 0;
    update();
    movementTimer->start(10);
}
