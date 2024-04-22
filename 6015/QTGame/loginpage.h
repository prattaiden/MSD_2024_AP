#ifndef LOGINPAGE_H
#define LOGINPAGE_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>


enum Difficulty {Easy, Medium, Hard};

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();
    bool authenticateUser(const QString& username, const QString& password);
    QString getUsername();
    Difficulty getSelectedDifficulty();

    bool isGuest;
    QString username;

private slots:
    void onSignInClicked();
    void onSignUpClicked();
    void onGuestClicked();


signals:
    void signInRequested(const QString &username, const QString &password);
    void signUpRequested();
    void playAsGuestRequested();
\


private:
    QLabel *titleLabel;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *signInButton;
    QPushButton *signUpButton;
    QPushButton *guestButton;
    QLabel *difficultyLabel;
    QComboBox *difficulty;
};

#endif // LOGINPAGE_H
