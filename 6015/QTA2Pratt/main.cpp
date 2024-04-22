#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QApplication>
#include "gameScene.h"
#include <loginpage.h>
#include "signup.h"
#include "dashboard.h"
#include <QGraphicsProxyWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>




QJsonObject getUserByUsername(const QString &username) {
    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open users.json";
        return QJsonObject();  // Return an empty object if the file can't be opened
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonArray users = doc.array();
    foreach (const QJsonValue &value, users) {
        QJsonObject obj = value.toObject();
        if (obj["username"].toString() == username) {
            return obj;  // Found the user, return the user object
        }
    }
    return QJsonObject();  // Return an empty object if no user is found
}

int main(int argc, char **argv){
    QApplication app(argc, argv);

    LoginPage loginPage;
    SignUp signUpPage;
    signUpPage.clearUsersJson();


    // Connect signUpRequested signal from LoginPage to slot to show signUpPage
    QObject::connect(&loginPage, &LoginPage::signUpRequested, [&] () {
        signUpPage.show(); // Show the sign-up page
        loginPage.hide(); // Hide the login page
    });

    // Connect switchToLoginPage signal from SignUp to a slot in main application
    QObject::connect(&signUpPage, &SignUp::switchToLoginPage, [&] () {
        signUpPage.hide();  // Hide the sign-up page
        loginPage.show();   // Show the login page
    });

    QObject::connect(&loginPage, &LoginPage::playAsGuestRequested, [&]() {
        QGraphicsView *mainView = new QGraphicsView();
        gameScene *gameScenePtr = new gameScene();
        Dashboard *dashboard = new Dashboard();
        gameScenePtr->setDash(dashboard);

        mainView->setScene(gameScenePtr);
        mainView->setFixedSize(700, 700);
        mainView->show();
        loginPage.hide();


        // Create and embed the dashboard into the graphics scene
        QString username = loginPage.getUsername();
        bool guest = loginPage.isGuest;

        QJsonObject user = getUserByUsername(username);

        if(guest){
            gameScenePtr->dashboard->setName("Guest");
            gameScenePtr->dashboard->setAvatar(QPixmap(":/images/guest.png"));
        }
        else{
            gameScenePtr->dashboard->setName(user["username"].toString());  // Corrected
            QString avatarPath = user["avatar"].toString();  // Get the avatar path as a string
            gameScenePtr->dashboard->setAvatar(QPixmap(avatarPath));  // Load QPixmap from the path

        }

        QGraphicsProxyWidget *proxyWidget = gameScenePtr->addWidget(dashboard);
        proxyWidget->setPos(0, 0); // Position at the top or wherever suitable in your scene
        mainView->setFocus();
    });

    loginPage.show();

    return app.exec();
}

