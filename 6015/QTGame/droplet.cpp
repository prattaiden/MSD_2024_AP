#include "droplet.h"
#include <QPixmap>
#include <QUrl>


droplet::droplet(Difficulty difficulty): dropletSpeed(100) {
    setPixmap(QPixmap(":/images/diamond.png").scaled(40,40));
    dropTimer = new QTimer(this);
    connect(dropTimer, &QTimer::timeout, this, &droplet::moveDroplet);

    //music!
    diamond = new QMediaPlayer;
    audio = new QAudioOutput;
    diamond->setAudioOutput(audio);
    diamond->setSource(QUrl(":/sounds/726508__sakurayo__diamond.mp3"));



    // Adjust droplet speed based on difficulty level
    switch (difficulty) {
    case Easy:
        dropletSpeed = 10;
        break;
    case Medium:
        dropletSpeed = 50;
        break;
    case Hard:
        dropletSpeed = 100;
        break;
    default:
        dropletSpeed = 10; // Default to easy difficulty
    }
    dropTimer->start(500);

}
QTimer* droplet::getTimer()const{
    return dropTimer;
}
void droplet::moveDroplet(){
    if(isPaused()){
        return;
    }
    setPos(x(),y() + dropletSpeed);
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    foreach (QGraphicsItem* item, collidingItemsList)
    {
        if (dynamic_cast<bucket*>(item))
        {
            emit collide();
            diamond->play();
            delete this;
            return;
        }
    }
    if(this->y() > 700){
        emit damageCounter();
        delete this;
    }
}

void droplet::updateSpeed(){
    dropletSpeed *= 1.2;
}

void droplet::pause(){
    if(dropTimer->isActive()){
        dropTimer->stop();
    }
}

bool droplet::isPaused(){
    return !dropTimer->isActive();
}
