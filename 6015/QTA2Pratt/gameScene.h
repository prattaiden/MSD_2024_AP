#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "bucket.h"
#include <QTimer>
#include "dashboard.h"


class gameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    gameScene();
    int getCounter();
    Dashboard *dashboard;
    void setDash(Dashboard *dash);

private slots:
    void createDroplets();
    void handleCollision();
    void handleDamage();

signals:
    void showWinScreen();
    void showLoseScreen();

private:
    bucket *bucket1;
    QTimer *dropletTimer;
    int counter = 0;
    int damageCounter =5;
};


#endif // GAMESCENE_H
