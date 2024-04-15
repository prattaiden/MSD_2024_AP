#include "bucket.h"
#include "QtWidgets/qgraphicsscene.h"

bucket::bucket() {

    setPixmap(QPixmap(":/images/bucket.png").scaled(200,200));

}

void bucket::keyPressEvent(QKeyEvent *event)
{
    int stepSize = 15; // Adjust the step size as needed
    int newX = x();

    if (event->key() == Qt::Key_Right)
    {
        newX += stepSize;
    }
    else if (event->key() == Qt::Key_Left)
    {
        newX -= stepSize;
    }
    else
    {
        QGraphicsPixmapItem::keyPressEvent(event);
        return;
    }

    // Bound the newX within the scene's boundaries
    if (newX >= 0 && newX <= scene()->width() - boundingRect().width())
    {
        setPos(newX, y());
    }
    else
    {
        qDebug() << "Cannot move beyond scene boundaries!";
    }
}

//handle moving left and right
