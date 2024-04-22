// signup.h
#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRegularExpression>


class SignUp : public QWidget
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();
    void clearUsersJson();


signals:
    void userSaved();
    void switchToLoginPage();

private slots:
    void onSuccessfulSignUp();
    void onSignUpClicked();
    void onProfilePictureClicked(); // Slot to handle profile picture selection



private:
    QLabel *titleLabel;
    QLineEdit *firstNameLineEdit;
    QLineEdit *lastNameLineEdit;
    QDateEdit *dateOfBirthEdit;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *signUpButton;

    QHBoxLayout *imageSelectionLayout; // Layout for image selection
    QPushButton *profilePictureButton; // Button for profile picture selection

    QString selectedProfilePicture; // Store the selected profile picture path

    QLabel *selectedAvatarLabel;

    bool saveUser(const QString& username, const QString& password,const QString& first, const QString &last, const QDate &bday, const QString ava);
    bool validateUserInput(const QString& username, const QString& password, const QString& first, const QString& last, const QDate& bday, const QString& ava, QString& errorMessage);

    QLabel *statusLabel;  // To display messages like validation errors

    QString avatarPath;



};


#endif // SIGNUP_H
