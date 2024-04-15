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


private:
     QTimer *dropTimer;


};



#endif // DROPLET_H
