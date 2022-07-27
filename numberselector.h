#ifndef NUMBERSELECTOR_H
#define NUMBERSELECTOR_H

#include "abstractselector.h"

class NumberSelector : public AbstractSelector
{
public:
    explicit NumberSelector(int startNumber = 0,
                                int endNumber = 24,
                                QWidget *parent = nullptr);
protected:
    void paintSelected(QPainter *painter, QPointF point, int index) override;
    void paintUnselected(QPainter *painter, QPointF point, int index) override;
    void paintBackground(QPainter *painter, QRectF rectf) override;

protected:
    int m_start = 0, m_end = 24;
/******************非选中/选中字体状态******************/
    // 字体框大小
    QSizeF size = QSizeF(48,26),
    selected_size = QSizeF(54,36);
    // 字体样式
    QFont font = QFont("OPPOSans-R, OPPOSans"),
    selected_font = QFont("OPPOSans-R, OPPOSans");
    int font_pixelsize = 30, font_selected_pixelsize = 44;
    // 字体颜色
    QColor color = QColor(89, 91, 102),
    selected_color = QColor(255, 255, 255);
    // 字体间距
    int spacer = 35;

    QColor background = QColor(38, 39, 44);   // 选择框背景颜色
};
#endif // NUMBERSELECTOR_H
