#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QRadioButton>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSpinBox>

class mainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit mainWidget(QWidget *parent = nullptr);
    void setGridLayout(QGridLayout *layout);
    void fillSummary();
    void clearWidgets();

    QLabel name_label;
    QLineEdit name_edit;
    QLabel age_label;
    QSpinBox age;
    QDialogButtonBox q_dialog_button_box_;
    QRadioButton male_button_;
    QRadioButton female_button_;
    QRadioButton other_button_;
    QTextEdit q_text_edit_;

private:
    QPushButton *ref;
    QTextEdit *summary;
    QPushButton *fin;




signals:
};

#endif // MAINWIDGET_H
