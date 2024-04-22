#include "loginpage.h"
#include "signup.h"
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

LoginPage::LoginPage(QWidget* parent) :QWidget(parent){

    SignUp signup;


    titleLabel = new QLabel("Sign in: ", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Username");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    signInButton = new QPushButton("Sign In", this);
    signUpButton = new QPushButton("Sign Up", this);
    guestButton = new QPushButton("Play as Guest", this);

    connect(signInButton, &QPushButton::clicked, this, &LoginPage::onSignInClicked);
    connect(signUpButton, &QPushButton::clicked, this, &LoginPage::onSignUpClicked);
    connect(guestButton, &QPushButton::clicked, this, &LoginPage::onGuestClicked);

    // Create the scroll area for difficulty selection
    difficultyBar = new QScrollArea(this);
    difficultyBar->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
    scrollWidget->setLayout(scrollLayout);
    scrollLayout->addWidget(new QPushButton("Easy"));
    scrollLayout->addWidget(new QPushButton("Medium"));
    scrollLayout->addWidget(new QPushButton("Hard"));
    difficultyBar->setWidget(scrollWidget);


     QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(difficultyBar);
    layout->addWidget(signUpButton);
    layout->addWidget(titleLabel);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(signInButton);
    layout->addWidget(guestButton);
    layout->addStretch();

    setLayout(layout);


}

LoginPage::~LoginPage()
{
    // Clean up allocated resources
    delete titleLabel;
    delete usernameLineEdit;
    delete passwordLineEdit;
    delete signInButton;
    delete signUpButton;
    delete guestButton;
}

void LoginPage::onSignInClicked()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    //emit signInRequested(username, password);

    if (authenticateUser(username, password)) {
        emit playAsGuestRequested();  // Signal to handle successful sign-in
    } else {
        QMessageBox::warning(this, "Login Failed", "Incorrect username or password.");
    }
}

void LoginPage::onSignUpClicked()
{
    emit signUpRequested();
}

void LoginPage::onGuestClicked()
{
    isGuest = true;
    emit playAsGuestRequested();
}

bool LoginPage::authenticateUser(const QString& username, const QString& password) {
    QFile file("users.json");
    // Open the file and read the content
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray users = doc.array();
    file.close();

    // Iterate over the stored users to find a match
    foreach (const QJsonValue &value, users) {
        QJsonObject obj = value.toObject();
        if (obj["username"].toString() == username && obj["password"].toString() == password) {
            return true;  // User authenticated successfully
        }
    }

    return false;  // User not found or password does not match
}

QString LoginPage::getUsername(){
    QString username = usernameLineEdit->text();
    return username;
}
