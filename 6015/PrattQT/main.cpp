#include <QApplication>
#include <QPushButton>
#include "mainwidget.h"

int main(int argc, char **argv) {
    QApplication app (argc, argv);
    mainWidget widget;
    widget.show();
    return app.exec();
}
