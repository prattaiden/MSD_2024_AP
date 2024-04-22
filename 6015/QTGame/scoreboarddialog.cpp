#include "scoreboarddialog.h"
#include <QHeaderView>


ScoreBoardDialog::ScoreBoardDialog(QWidget *parent) : QDialog(parent) {
    allTimeGreatestLabel = new QLabel("All Time Greatest: ", this);
    yourScoreLabel = new QLabel("Your Score: 0", this);
    scoreTable = new QTableWidget(this);
    scoreTable->setColumnCount(3);
    scoreTable->setHorizontalHeaderLabels({"Date","Username", "Score"});
    scoreTable->horizontalHeader()->setStretchLastSection(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(allTimeGreatestLabel);
    layout->addWidget(yourScoreLabel);
    layout->addWidget(scoreTable);
    setLayout(layout);

    setWindowTitle("Score Board");
    resize(400, 300); // Adjust size as necessary
}

//all time greatest is currently not global.. just for the specific account
void ScoreBoardDialog::setAllTimeGreatest(int score) {
    allTimeGreatestLabel->setText(QString("All Time Greatest: %1").arg(score));
}


void ScoreBoardDialog::setYourScore(int score) {
    yourScoreLabel->setText(QString("Your Score: %1").arg(score));
}


void ScoreBoardDialog::addScoreRecord(const QString &date, const QString &username, int score) {
    int row = scoreTable->rowCount();
    scoreTable->insertRow(row);
    scoreTable->setItem(row, 0, new QTableWidgetItem(date));
    scoreTable->setItem(row, 1, new QTableWidgetItem(username));
    scoreTable->setItem(row, 2, new QTableWidgetItem(QString::number(score)));

    qDebug() << "Added score for " << username << " on " << date << ": " << score;
}


