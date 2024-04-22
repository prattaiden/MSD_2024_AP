
#include "AvatarPicker.h"
#include <QVBoxLayout>
#include <QListWidgetItem>


AvatarPicker::AvatarPicker(QWidget *parent) : QDialog(parent), listWidget(new QListWidget(this)) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(listWidget);
    // List of avatar image files (assuming they are in the same directory or a resource file)
    QStringList avatarFiles = {":/images/profile1.png", ":/images/profile2.png", ":/images/profile3.png"};
    foreach (const QString &file, avatarFiles) {
        QListWidgetItem *item = new QListWidgetItem(QIcon(file), "");
        item->setData(Qt::UserRole, file);  // Store the file path in the item
        listWidget->addItem(item);
    }
    connect(listWidget, &QListWidget::itemClicked, this, &AvatarPicker::avatarSelected);
    // Set some dialog properties
    setWindowTitle("Choose an Avatar");
    resize(200, 300); // Adjust size as needed
}
void AvatarPicker::avatarSelected(QListWidgetItem *item) {
    if (item) {
        selectedAvatar = item->data(Qt::UserRole).toString();
        accept();  // Closes the dialog with QDialog::Accepted
    }
}
QString AvatarPicker::getSelectedAvatar() const {
    return selectedAvatar;
}
