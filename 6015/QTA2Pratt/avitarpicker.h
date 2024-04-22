#ifndef AVATARPICKER_H
#define AVATARPICKER_H
#include <QDialog>
#include <QListWidget>
class AvitarPicker : public QDialog {
    Q_OBJECT
public:
    explicit AvitarPicker(QWidget *parent = nullptr);
    QString getSelectedAvatar() const;
private:
    QListWidget *listWidget;
    QString selectedAvatar;
private slots:
    void avitarSelected(QListWidgetItem *item);
};
#endif // AVATARPICKER_H
