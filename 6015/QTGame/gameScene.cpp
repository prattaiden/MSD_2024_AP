#include "gameScene.h"
#include <droplet.h>
#include <QRandomGenerator>
#include "bucket.h"
#include "droplet.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDate>
#include <QDebug>
#include "loginpage.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QVector>
struct ScoreRecord {
    QString username;
    QString date;
    int score;
    ScoreRecord(const QString& username, const QString& date, int score) : username(username), date(date), score(score) {}
    QJsonObject toJsonObject() const {
        QJsonObject obj;
        obj["username"] = username;
        obj["date"] = date;
        obj["score"] = score;
        return obj;
    }
};
void gameScene::setUserName(QString &username) {
    userName=username;
}
gameScene :: gameScene(Difficulty diff) : dropletTimer(new QTimer(this)) , counter(0), allTime(0){
    //setting background image and size
    difficultyMember = diff;
    setBackgroundBrush(QPixmap(":/images/cave.jpg"));
    setSceneRect(0,0,1000,700);
    connect(dropletTimer, &QTimer::timeout, this, &gameScene::createDroplets);
    //dropletTimer->start(2000); // Change the interval as needed
    switch (diff) {
    case Easy:
        dropletTimer->start(2000); // 2 seconds for easy difficulty
        break;
    case Medium:
        dropletTimer->start(1000); // 1 second for medium difficulty
        break;
    case Hard:
        dropletTimer->start(500); // 0.5 seconds for hard difficulty
        break;
    default:
        dropletTimer->start(2000); // Default to easy difficulty
    }
    //setting bucket dude
    bucket *bucket1 = new bucket();
    bucket1->setPos(200,500);
    addItem(bucket1);
    bucket1->setFocus();
    // In your gameScene constructor or an appropriate initialization method
    QPushButton *endGameButton = new QPushButton("End Game");
    QGraphicsProxyWidget *proxyWidget = this->addWidget(endGameButton);
    proxyWidget->setPos(905, 5); // Set position as required
    connect(endGameButton, &QPushButton::clicked, this, &gameScene::endGame);
}
void gameScene::createDroplets(){
    droplet* newDroplet = new droplet(difficultyMember);
    int randomXAxis = QRandomGenerator::global()->bounded(950);
    newDroplet->setFlag(QGraphicsItem::ItemIsFocusable);
    newDroplet->setFocus();
    newDroplet->setPos(randomXAxis, 0);
    addItem(newDroplet);
    connect(newDroplet, SIGNAL(collide()), this, SLOT(handleCollision()));
    connect(newDroplet, SIGNAL(damageCounter()), this, SLOT(handleDamage()));
}
void gameScene::handleCollision(){
    counter+= 5;
    dashboard->setScore(counter);
    qDebug() << "Collision counter:" << counter;
    if(counter >= 300){ //set the max score for win condition
        qDebug() << "hit 150 score!";
        endGame();
    }
    if (counter > 10) {//this updates the speed of the droplets once the score goes over 10
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
    --damageCounter;
    dashboard->setHealth(damageCounter);
    qDebug() << "droplet missed!";
    qDebug() << "missed score: " << damageCounter;
    if(damageCounter == 0){
        endGame();
    }
}
int gameScene::getCounter(){
    return counter;
}
void gameScene::setDash(Dashboard *dashboardPtr) {
    dashboard = dashboardPtr;
}
void gameScene::endGame() {
    dropletTimer->stop();
        //-----------------------------------------CLEARS RECORD BOARD--------------------------------------------------------------
    // clearBoard();
    //-------------------------------------------------------------------------------------------------------------------------
    int finalScore = getCounter();
    qDebug() << "End Game triggered for username:" << userName << " with score:" << finalScore;
    saveScoreHistory(finalScore);
    counter = finalScore;
    damageCounter = 0;
    scoreBoard();
    QList<QGraphicsItem*> itemsList = items();
    for (QGraphicsItem* item : itemsList) {
        droplet* dropletItem = dynamic_cast<droplet*>(item);
        if (dropletItem) {
            dropletItem->pause(); // Pause the droplet
        }
    }
}
void gameScene::saveScoreHistory(int score) {
    // Load the current all-time high score first
    QFile fileHighScore("all_time_high.json");
    if (fileHighScore.open(QIODevice::ReadOnly)) {
        QJsonDocument docHighScore = QJsonDocument::fromJson(fileHighScore.readAll());
        fileHighScore.close();
        allTime = docHighScore.object().value("allTimeHigh").toInt();
        qDebug() << "Current all-time high score loaded:" << allTime;
    }
    // Now proceed with saving the new score
    QFile fileScores("scores.json");
    QJsonDocument docScores;
    if (fileScores.open(QIODevice::ReadOnly)) {
        docScores = QJsonDocument::fromJson(fileScores.readAll());
        fileScores.close();
    }
    QJsonArray scoresArray = docScores.array();
    ScoreRecord newScore(userName, QDate::currentDate().toString("yyyy-MM-dd"), score);
    scoresArray.append(newScore.toJsonObject());
    qDebug() << "Saving new score for username:" << userName << " with score:" << score;
    // Update the all-time high score if necessary
    if (score > allTime) {
        allTime = score;
        qDebug() << "New all-time high score is now:" << allTime;
        updateAllTimeHighScore();  // Update the all-time high score in its file
    }
    // Save the scores array back to the file
    docScores.setArray(scoresArray);
    if (fileScores.open(QIODevice::WriteOnly)) {
        fileScores.write(docScores.toJson());
        fileScores.close();
    }
}
void gameScene::updateAllTimeHighScore() {
    qDebug() << "updateAllTimeHighScore" << allTime;
    QFile file("all_time_high.json");
    QJsonDocument doc;
    QJsonObject obj;
    obj["allTimeHigh"] = allTime;  // Store the highest score ever achieved
    doc.setObject(obj);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "All-time high score updated:" << allTime;
    }
}
void gameScene::scoreBoard() {
    ScoreBoardDialog* scoreDialog = new ScoreBoardDialog();
    QFile file("scores.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray scoresArray = doc.array();
        qDebug() << "Loading scores for username:" << userName;
        for (const QJsonValue& value : scoresArray) {
            QJsonObject scoreObj = value.toObject();
            if (scoreObj["username"].toString() == userName) {
                int score = scoreObj["score"].toInt();
                QString date = scoreObj["date"].toString();
                scoreDialog->addScoreRecord(date, userName, score);
            }
        }
        file.close();
    }
    // Load and display the all-time high score
    loadAllTimeHighScore(scoreDialog);
    scoreDialog->setYourScore(getCounter());
    scoreDialog->show();
}
void gameScene::loadAllTimeHighScore(ScoreBoardDialog* scoreDialog) {
    QFile file("all_time_high.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        QJsonObject obj = doc.object();
        int highestScore = obj["allTimeHigh"].toInt();
        scoreDialog->setAllTimeGreatest(highestScore);
        qDebug() << "Loaded all-time high score:" << highestScore;
    }
}
void gameScene::clearBoard() {
    QFile file("scores.json"); // Adjust path as needed
    if (file.open(QIODevice::WriteOnly)) {
        file.close();  // Just open in write mode and close to clear the contents
        qDebug() << "scores.json has been cleared.";
    } else {
        qDebug() << "Failed to clear scores.json";
    }
}
