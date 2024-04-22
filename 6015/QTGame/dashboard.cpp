// dashboard.cpp
#include "dashboard.h"



Dashboard::Dashboard(QWidget *parent) : QWidget(parent) {
    // Initialize the labels
    avatarLabel = new QLabel(this);
    nameLabel = new QLabel(this);
    dateLabel = new QLabel(this);
    scoreLabel = new QLabel(this);
    healthLabel = new QLabel(this);



    // Set default or placeholder content
    avatarLabel->setPixmap(QPixmap(":/default_avatar.png").scaled(50, 50, Qt::KeepAspectRatio));
    nameLabel->setText("Name: Placeholder");
    dateLabel->setText("Date: " + QDate::currentDate().toString("dd MMM yyyy"));
    scoreLabel->setText("Score: 0");
    healthLabel->setText("Health: 5");

    // Layout setup
    QVBoxLayout *topLayout = new QVBoxLayout();
    topLayout->addWidget(avatarLabel);
    topLayout->addWidget(nameLabel);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(scoreLabel);
    mainLayout->addWidget(healthLabel);
    mainLayout->addWidget(dateLabel);



    setLayout(mainLayout);
}

void Dashboard::setAvatar(const QPixmap &avatar) {
    avatarLabel->setPixmap(avatar.scaled(50, 50, Qt::KeepAspectRatio));
}

void Dashboard::setName(const QString &name) {
    nameLabel->setText(name);
}

void Dashboard::setScore(int score) {
    scoreLabel->setText("Score: " + QString::number(score));
}

void Dashboard::setHealth(int damage){
    healthLabel->setText("Health: " + QString::number(damage));
}
