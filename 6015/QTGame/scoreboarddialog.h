#ifndef SCOREBOARDDIALOG_H
#define SCOREBOARDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

class ScoreBoardDialog : public QDialog {
    Q_OBJECT

public:
    explicit ScoreBoardDialog(QWidget *parent = nullptr);
    void setAllTimeGreatest(int score);
    void setYourScore(int score);
    void addScoreRecord(const QString &date,const QString &username, int score);
    int allTimeGreatestScore;

private:
    QLabel *allTimeGreatestLabel;
    QLabel *yourScoreLabel;
    QTableWidget *scoreTable;
};

#endif // SCOREBOARDDIALOG_H
