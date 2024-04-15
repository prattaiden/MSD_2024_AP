#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QApplication>
#include "gameScene.h"

int main(int argc, char **argv){
    QApplication app (argc, argv);

    gameScene *gamescene = new gameScene();

    //create a view
    QGraphicsView *mainView = new QGraphicsView();

    //add the scene to the view
    mainView->setScene(gamescene);
    mainView->setFixedSize(700,700);

    //show the view
    mainView->show();

    return app.exec();

}
