#include "wallpaperselector.h"

WallpaperSelector::WallpaperSelector(QWidget *parent)
    :AbstractSelector(parent)
{
    setOrientation(Qt::Horizontal);               // 滚动方向
    setScrollInterval(spacer+imageSize.width());    // 字体间隔
    setCircularPrevNext(true,1,1);              // 循环补全
    InitImageMsgs();    // 添加图片资源
    checkCircular();                            // 检验是否需要补全
}

void WallpaperSelector::InitImageMsgs()
{
    addResource(":/res/screen_abstractLine.png");
    addResource(":/res/screen_dateWeather.png");
    addResource(":/res/screen_dreamSpace.png");
    addResource(":/res/screen_floatingParticles.png");
    addResource(":/res/screen_flyInBlueShy.png");
    addResource(":/res/screen_foldedSquare.png");
    addResource(":/res/screen_pastime.png");
    addResource(":/res/screen_starsOrbit.png");
}

void WallpaperSelector::paintSelected(QPainter *painter, QPointF point, int index)
{
    QRectF rect(QPointF(0,0), imageSize);
    rect.moveCenter(point);
    painter->drawImage(rect,QImage(m_resourceList.at((index+m_resourceCount)%m_resourceCount).toString()));

    painter->setPen(QPen(QColor(89,129,178),3));
    painter->drawRoundedRect(rect,6,6);
}

void WallpaperSelector::paintUnselected(QPainter *painter, QPointF point, int index)
{
    QRectF rect(QPointF(0,0), imageSize);
    rect.moveCenter(point);
    painter->drawImage(rect,QImage(m_resourceList.at((index+m_resourceCount)%m_resourceCount).toString()));

    painter->setPen(QPen(QColor(38,39,43),3));
    painter->drawRoundedRect(rect,6,6);
}

void WallpaperSelector::paintBackground(QPainter *painter, QRectF rectf)
{
    painter->fillRect(rectf,background);
}
