#include "gameScene.h"
#include <droplet.h>
#include <QRandomGenerator>
#include "bucket.h"
#include "droplet.h"


gameScene :: gameScene() : dropletTimer(new QTimer(this)){

    //setting background image and size
    setBackgroundBrush(QPixmap(":/images/background.jpg"));
    setSceneRect(0,0,700,700);

    connect(dropletTimer, &QTimer::timeout, this, &gameScene::createDroplets);
    dropletTimer->start(2000); // Change the interval as needed


    //setting droplets
    // droplet *droplet = new class droplet();
    // int randomXAxis = QRandomGenerator::global()->bounded(600);
    // droplet->setFlag(QGraphicsItem::ItemIsFocusable);
    // droplet->setFocus();
    // droplet->setPos(randomXAxis, 0);
    // addItem(droplet);





    //setting bucket dude
    bucket *bucket1 = new bucket();
    bucket1->setFlag(QGraphicsItem::ItemIsFocusable);
    bucket1->setFocus();
    bucket1->setPos(200,500);
    addItem(bucket1);


}

void gameScene::createDroplets(){
    droplet* newDroplet = new droplet();
    int randomXAxis = QRandomGenerator::global()->bounded(600);
    newDroplet->setFlag(QGraphicsItem::ItemIsFocusable);
    newDroplet->setFocus();
    newDroplet->setPos(randomXAxis, 0);
    addItem(newDroplet);
}
