#include "gameScene.h"
#include <droplet.h>
#include <QRandomGenerator>
#include "bucket.h"
#include "droplet.h"


gameScene :: gameScene() : dropletTimer(new QTimer(this)) , counter(0){

    //setting background image and size
    setBackgroundBrush(QPixmap(":/images/background.jpg"));
    setSceneRect(0,0,700,700);

    connect(dropletTimer, &QTimer::timeout, this, &gameScene::createDroplets);
    dropletTimer->start(2000); // Change the interval as needed


    //setting bucket dude
    bucket *bucket1 = new bucket();
    bucket1->setPos(200,500);
    addItem(bucket1);
    bucket1->setFocus();
}

void gameScene::createDroplets(){
    droplet* newDroplet = new droplet();
    int randomXAxis = QRandomGenerator::global()->bounded(600) + 50;
    newDroplet->setFlag(QGraphicsItem::ItemIsFocusable);
    newDroplet->setFocus();
    newDroplet->setPos(randomXAxis, 0);
    addItem(newDroplet);
    connect(newDroplet, SIGNAL(collide()), this, SLOT(handleCollision()));
    connect(newDroplet, SIGNAL(damageCounter()), this, SLOT(handleDamage()));
}

void gameScene::handleCollision(){
    counter++;
    dashboard->setScore(counter);
    qDebug() << "Collision counter:" << counter;

    if(counter >= 150){
        qDebug() << "hit 150 score!";
    }
    if (counter > 5) {
        QList<QGraphicsItem*> itemsList = items();
        foreach (QGraphicsItem* item, itemsList)
        {
            droplet* dropletItem = dynamic_cast<droplet*>(item);
            if (dropletItem)
            {
                dropletItem->updateSpeed();
            }
        }
    }
}

void gameScene::handleDamage(){
    qDebug() << "rain drop missed!";
    damageCounter--;
    dashboard->setHealth(damageCounter);
        if(damageCounter <= 0){
        qDebug() << "you lose"  << damageCounter;
    }
}

int gameScene::getCounter(){
    return counter;
}

void gameScene::setDash(Dashboard *dashboardPtr) {
    dashboard = dashboardPtr;
}
