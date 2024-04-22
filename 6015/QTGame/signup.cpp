// signup.cpp
#include "signup.h"
#include "avatarpicker.h"
#include <QFileDialog>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <QMessageBox>


SignUp::SignUp(QWidget *parent) : QWidget(parent)
{
    statusLabel = new QLabel;
    titleLabel = new QLabel("Sign Up Page", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    firstNameLineEdit = new QLineEdit(this);
    firstNameLineEdit->setPlaceholderText("First Name");

    lastNameLineEdit = new QLineEdit(this);
    lastNameLineEdit->setPlaceholderText("Last Name");

    dateOfBirthEdit = new QDateEdit(this);
    QLabel *bday = new QLabel("Birthday:");
    dateOfBirthEdit->setDisplayFormat("yyyy-MM-dd");
    dateOfBirthEdit->setDate(QDate::currentDate()); // Set default date to current date

    // genderLineEdit = new QLineEdit(this);
    // genderLineEdit->setPlaceholderText("Gender (optional)");



    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Username");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    signUpButton = new QPushButton("Sign Up", this);

    connect(signUpButton, &QPushButton::clicked, this, &SignUp::onSignUpClicked);
    connect(this, &SignUp::userSaved, this, &SignUp::onSuccessfulSignUp);

    // Profile picture selection
    //imageSelectionLayout = new QHBoxLayout(this);
    profilePictureButton = new QPushButton("Select Profile Picture", this);
    //imageSelectionLayout->addWidget(profilePictureButton);
    connect(profilePictureButton, &QPushButton::clicked, this, &SignUp::onProfilePictureClicked);



    //ADD TO THE LAYOUT

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(firstNameLineEdit);
    layout->addWidget(lastNameLineEdit);
    layout->addWidget(bday);
    layout->addWidget(dateOfBirthEdit);
    //layout->addWidget(genderLineEdit);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(profilePictureButton); // Add image selection layout to main layout
    selectedAvatarLabel = new QLabel(this);
    layout->addWidget(selectedAvatarLabel);

    layout->addWidget(signUpButton);

    layout->addStretch();

    setLayout(layout);
}

SignUp::~SignUp()
{
    // Clean up allocated resources
    delete titleLabel;
    delete firstNameLineEdit;
    delete lastNameLineEdit;
    delete dateOfBirthEdit;
    delete usernameLineEdit;
    delete passwordLineEdit;
    delete signUpButton;
}

bool SignUp::validateUserInput(const QString& username, const QString& password, const QString& first, const QString& last, const QDate& bday, const QString& ava, QString& errorMessage) {
    QStringList errors;

    if (username.isEmpty()) errors.append("Username is required.");
    if (password.isEmpty()) errors.append("Password is required.");
    if (first.isEmpty()) errors.append("First name is required.");
    if (last.isEmpty()) errors.append("Last name is required.");
    if (bday.toString().isEmpty()) errors.append("Birthday is required.");
    if (ava.isEmpty()) errors.append("Avatar is required.");

    if (!errors.isEmpty()) {
        errorMessage = errors.join("\n");
        return false;
    }

    // Using QRegularExpression for pattern matching
    QRegularExpression upperCasePattern("[A-Z]");
    QRegularExpression lowerCasePattern("[a-z]");
    QRegularExpression digitPattern("[0-9]");

    if (password.length() < 8 || !password.contains(upperCasePattern) || !password.contains(lowerCasePattern) || !password.contains(digitPattern)) {
        errorMessage = "Password must be at least 8 characters and include an uppercase letter, a lowercase letter, and a number.";
        return false;
    }

    return true; // All validations passed
}


void SignUp::onSignUpClicked()
{
    QString firstName = firstNameLineEdit->text();
    QString lastName = lastNameLineEdit->text();
    QDate dateOfBirth = dateOfBirthEdit->date();
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    QString errorMessage;

    // Validate user input
    if (!validateUserInput(username, password, firstName, lastName, dateOfBirth, avatarPath, errorMessage)) {
        QMessageBox::warning(this, "Input Validation", errorMessage);
        return; // Exit the function if validation fails
    }
    // Emit signal with sign-up information
    if(saveUser(username, password, firstName, lastName, dateOfBirth, avatarPath)){
        emit userSaved();
    }

    else{
        qDebug() << "Error saving user.";
    }
}

void SignUp::onProfilePictureClicked()
{
    AvatarPicker *picker = new AvatarPicker(this);
    if (picker->exec() == QDialog::Accepted) {
        avatarPath = picker->getSelectedAvatar();
        QPixmap avatarPixmap(avatarPath);
        selectedAvatarLabel->setPixmap(avatarPixmap.scaled(100, 100));
    }
}

//Call this function when a new user completes the registration form. It stores the user's username and password directly in a JSON file.
bool SignUp::saveUser(const QString& username, const QString& password, const QString& first, const QString &last, const QDate &bday, const QString ava) {
    QFile file("users.json");
    QJsonDocument doc;
    QJsonArray users;
    // Open the file and read the current users if the file exists
    if (file.open(QIODevice::ReadOnly)) {
        doc = QJsonDocument::fromJson(file.readAll());
        users = doc.array();
        file.close();
    }
    // Check if the username already exists
    foreach (const QJsonValue &value, users) {
        QJsonObject obj = value.toObject();
        if (obj["username"].toString() == username) {
            QMessageBox::warning(this, "Input Validation", "Username already exists. Choose a different username.");
            return false;  // Return false if the username already exists

        }
    }
    // Create a new JSON object for the new user
    QJsonObject newUser;
    newUser["first"] = first;
    newUser["last"] = last;
    newUser["bday"] = bday.toString();
    newUser["username"] = username;
    newUser["password"] = password;
    newUser["avatar"] = ava;

    users.append(newUser);
    // Replace the current JSON array with the new one containing the new user
    doc.setArray(users);
    // Write the updated JSON document back to the file
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        return true;  // Return true if user is successfully added
    }
    return false;  // Return false if there was an error writing to the file
}

void SignUp::onSuccessfulSignUp()
{
    // Clear the input fields or perform any other cleanup
    firstNameLineEdit->clear();
    lastNameLineEdit->clear();
    dateOfBirthEdit->setDate(QDate::currentDate());
    usernameLineEdit->clear();
    passwordLineEdit->clear();
    selectedAvatarLabel->clear();  // Clear the selected avatar label

    emit switchToLoginPage();
}

void SignUp::clearUsersJson() {
    QFile file("users.json"); // Adjust path as needed
    if (file.open(QIODevice::WriteOnly)) {
        file.close();  // Just open in write mode and close to clear the contents
        qDebug() << "users.json has been cleared.";
    } else {
        qDebug() << "Failed to clear users.json";
    }
}
