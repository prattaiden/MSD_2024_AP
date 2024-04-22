#ifndef AVATARPICKER_H
#define AVATARPICKER_H
#include <QDialog>
#include <QListWidget>
class AvatarPicker : public QDialog {
    Q_OBJECT
public:
    explicit AvatarPicker(QWidget *parent = nullptr);
    QString getSelectedAvatar() const;
private:
    QListWidget *listWidget;
    QString selectedAvatar;
private slots:
    void avatarSelected(QListWidgetItem *item);
};
#endif // AVATARPICKER_H










