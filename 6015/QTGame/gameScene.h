#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "bucket.h"
#include <QTimer>
#include "dashboard.h"
#include "loginpage.h"
#include "scoreboarddialog.h"

// enum Difficulty {Easy, Medium, Hard};

class gameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    gameScene(Difficulty difficulty);
    int getCounter();
    Dashboard *dashboard;
    void setDash(Dashboard *dash);
    void saveScoreHistory(int score);
    void scoreBoard();
    void endGame();
    int allTime;
    QJsonArray loadUserScoreHistory();
    void clearBoard();
    QString userName;
    void setUserName(QString &username);
    Difficulty getDiff();
    Difficulty difficultyMember;
    void updateAllTimeHighScore();
    void loadAllTimeHighScore(ScoreBoardDialog* scoreDialog);




private slots:
    void createDroplets();
    void handleCollision();
    void handleDamage();

signals:
    void gameClose();
    void allTimeGreatestChanged(int newScore);

private:
    bucket *bucket1;
    QTimer *dropletTimer;
    int counter = 0;
    int damageCounter = 5;
};
#endif // GAMESCENE_H
