// dashboard.h
#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>

class Dashboard : public QWidget {
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    void setAvatar(const QPixmap &avatar);
    void setName(const QString &name);
    void setScore(int score);
    void setHealth(int damage);

private:
    QLabel *avatarLabel;
    QLabel *nameLabel;
    QLabel *dateLabel;
    QLabel *scoreLabel;
    QLabel *healthLabel;
};

#endif // DASHBOARD_H
