#ifndef WALLPAPERSELECTOE_H
#define WALLPAPERSELECTOE_H

#include "abstractselector.h"

class WallpaperSelector : public AbstractSelector
{
public:
    explicit WallpaperSelector(QWidget *parent = nullptr);
    void InitImageMsgs();

protected:
    void paintSelected(QPainter *painter, QPointF point, int index) override;
    void paintUnselected(QPainter *painter, QPointF point, int index) override;
    void paintBackground(QPainter *painter, QRectF rectf) override;

private:
    QSize imageSize = QSize(320,320);
    int spacer = 20;
    QColor background = Qt::black;   // 选择框背景颜

};


#endif // WALLPAPERSELECTOE_H
