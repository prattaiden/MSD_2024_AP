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


bool isBirthday(const QJsonObject &user){
    QDate bday = QDate::fromString(user["bday"].toString(), "ddd MMM dd yyyy");
    qDebug() << "Birthday: " << user["bday"].toString();
    QDate today = QDate::currentDate();
    qDebug() << "Date: " << today;

    return (bday.month() == today.month() && bday.day() == today.day());
}



int main(int argc, char **argv){

    QApplication app(argc, argv);

    LoginPage loginPage;
    SignUp signUpPage;

    //---------------------------------Clears user signup--for testing purposes------------------------------------------------
    // signUpPage.clearUsersJson();
    //-------------------------------------------------------------------------------------------------------------------------


    QObject::connect(&loginPage, &LoginPage::signUpRequested, [&] () {
        signUpPage.show(); // Show the sign-up page
        loginPage.hide(); // Hide the login page
    });

    QObject::connect(&signUpPage, &SignUp::switchToLoginPage, [&] () {
        signUpPage.hide();  // Hide the sign-up page
        loginPage.show();   // Show the login page
    });


    QObject::connect(&loginPage, &LoginPage::playAsGuestRequested, [&]() {
        QGraphicsView *mainView = new QGraphicsView();
        Difficulty selectedDiff = loginPage.getSelectedDifficulty();
        gameScene *gameScenePtr = new gameScene(selectedDiff);
        mainView->setScene(gameScenePtr);
        mainView->setFixedSize(1000, 700);
        mainView->show();
        loginPage.hide();

        // Create and embed the dashboard into the graphics scene
        QString username = loginPage.getUsername();
        gameScenePtr->setUserName(username);
        QJsonObject user = getUserByUsername(username);

        bool isGuest = loginPage.isGuest; // You would need to add a public method or friend class to access this.
        Dashboard *dashboard = new Dashboard(nullptr);
        gameScenePtr->setDash(dashboard);



        if (isGuest) {
            // Set guest-specific settings
            gameScenePtr->dashboard->setName("Guest");
            gameScenePtr->dashboard->setAvatar(QPixmap(":/images/guest.jpeg"));
        } else {
            if( isBirthday(user) ){
                gameScenePtr->dashboard->setName(user["username"].toString() + " - Happy Birthday!");
            }
            else{
                gameScenePtr->dashboard->setName(user["username"].toString());
            }
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



// newUser["first"] = first;
// newUser["last"] = last;
// newUser["bday"] = bday.toString();
// newUser["username"] = username;
// newUser["password"] = password;
// newUser["avatar"] = ava;



