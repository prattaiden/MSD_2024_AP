#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "bucket.h"
#include <QTimer>


class gameScene : public QGraphicsScene
{

    Q_OBJECT

public:


    gameScene();

private slots:
    void createDroplets();


private:

    bucket *bucket1;
    QTimer *dropletTimer;

};


#endif // GAMESCENE_H
