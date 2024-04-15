#include "droplet.h"
#include <QPixmap>

droplet::droplet() {

    setPixmap(QPixmap(":/images/water.gif").scaled(30,30));

    dropTimer = new QTimer(this);

    connect(dropTimer, &QTimer::timeout, this, &droplet::moveDroplet);
    dropTimer->start(1000);


}

QTimer* droplet::getTimer()const{
    return dropTimer;
}

void droplet::moveDroplet(){
    setPos(x(),y() + 30);

    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    foreach (QGraphicsItem* item, collidingItemsList)
    {
        if (dynamic_cast<bucket*>(item))
        {
            // Handle collision with bucket
            qDebug() << "Bucket collision detected!";
            delete this;
            return;
        }
    }

    if(this->y() > 700){
        delete this;
    }
}
