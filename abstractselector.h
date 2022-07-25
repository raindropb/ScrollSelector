#ifndef ABSTRACTSELECTOR_H
#define ABSTRACTSELECTOR_H

#include <QWidget>

class AbstractSelector : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractSelector(QWidget *parent = nullptr);

signals:

};

#endif // ABSTRACTSELECTOR_H
