#include "bucket.h"
#include "QtWidgets/qgraphicsscene.h"
bucket::bucket() {
    setPixmap(QPixmap(":/images/miner.png").scaled(200,200));
    setFlag(QGraphicsItem::ItemIsFocusable); // Set focusable flag
    setFocus(); // Set focus on the bucket item
}
void bucket::keyPressEvent(QKeyEvent *event)
{
    // qDebug() << "Key pressed!" << event->key();
    int stepSize = 40; // Adjust the step size as needed
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
void bucket::focusInEvent(QFocusEvent *event)
{
    QGraphicsPixmapItem::focusInEvent(event);
}
