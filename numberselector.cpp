#include "numberselector.h"

NumberSelector::NumberSelector(int startNumber, int endNumber, QWidget *parent)
    : AbstractSelector(parent),
      m_start(startNumber),
      m_end(endNumber)
{
    setOrientation(Qt::Vertical);               // 滚动方向
    setScrollInterval(spacer+size.height());    // 字体间隔
    setCircularPrevNext(true,1,2);              // 循环补全
    for(int i=m_start; i<m_end; i++)            // 创建数值
        this->addResource(i);
    checkCircular();                            // 检验是否需要补全
    // 字体像素大小设置，该部分无法在创建时初始化
    font.setPixelSize(font_pixelsize);
    selected_font.setPixelSize(font_selected_pixelsize);
}

void NumberSelector::paintSelected(QPainter *painter, QPointF point, int index)
{
    QRectF rect(QPoint(0,0),selected_size);
    painter->setPen(selected_color);
    painter->setFont(selected_font);
    rect.moveCenter(point);
    int number = m_resourceList.at((index+m_resourceCount)%m_resourceCount).toInt();
    painter->drawText(rect,Qt::AlignCenter,QString().sprintf("%02d",number));
}

void NumberSelector::paintUnselected(QPainter *painter, QPointF point, int index)
{
    QRectF rect(QPoint(0,0),size);
    painter->setPen(color);
    painter->setFont(font);
    rect.moveCenter(point);
    int number = m_resourceList.at((index+m_resourceCount)%m_resourceCount).toInt();
    painter->drawText(rect,Qt::AlignCenter,QString().sprintf("%02d",number));
}

void NumberSelector::paintBackground(QPainter *painter, QRectF rectf)
{
    painter->fillRect(rectf,background);
}
