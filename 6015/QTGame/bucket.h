#ifndef BUCKET_H
#define BUCKET_H
#include <qobject.h>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QGraphicsItem>
class bucket : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    bucket();
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
signals:
    void moved();
};
#endif // BUCKET_H
