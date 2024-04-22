#ifndef DROPLET_H
#define DROPLET_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "bucket.h"

class droplet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    droplet();
    QTimer *getTimer() const;
    void moveDroplet();

    void updateSpeed();


signals:
    void collide();
    void damageCounter();



private:
     QTimer *dropTimer;
     int dropletSpeed = 20;



};



#endif // DROPLET_H
