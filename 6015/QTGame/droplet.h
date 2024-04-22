#ifndef DROPLET_H
#define DROPLET_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QTimer>
#include "bucket.h"
#include "loginpage.h"



class droplet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    droplet(Difficulty difficulty);
    QTimer *getTimer() const;
    void moveDroplet();
    void pause();
    void updateSpeed();
    bool isPaused();
    QMediaPlayer *diamond;
    QAudioOutput *audio;

signals:
    void collide();
    void damageCounter();


private:
    QTimer *dropTimer;
    int dropletSpeed;
};
#endif // DROPLET_H
