#ifndef SCROLLSELECTOR_H
#define SCROLLSELECTOR_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QPainter>

class QAbstractSelector :public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double m_curIndex READ getCurIndex WRITE setCurIndex NOTIFY sigCurIndexChanged)
public:
    explicit QAbstractSelector(QWidget *parent = nullptr);
    double getCurIndex() const;
    void setCurIndex(double index);

protected:
    // 绘制选中/未选中对象
    void virtual paintSelected(QPainter *painter, QPointF point, int index);
    void virtual paintUnselected(QPainter *painter, QPointF point, int index);
    QPointF getObjectCenter(int index);
    QPointF getBeginPoint();
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;
    void autoStart();

    // 选择对象列表
    void setResourceList(QVector<QVariant> resourceList);
    void addResource(QVariant resource);
    QVector<QVariant> getResourceList() const;
    // 滚动对象间隔
    void setScrollInterval(int interval);
    int getScrollInterval() const;
    // 滚动方向：水平/垂直
    void setOrientation(Qt::Orientation orientation);
    Qt::Orientation getOrientation() const;
    // 循环滚动及补全
    void setCircularPrevNext(bool circular, int prev, int next);
    std::tuple<bool,int,int> getCircularPrevNext() const;
    // 背景
    void setBackground(QColor color);
    void checkCircular();

signals:
    void sigCurIndexChanged(int index);

protected:
    double m_curIndex = 0.0;

/***********************资源对象***********************/
    QVector<QVariant> m_resourceList;
    int m_resourceCount = 0;
/***********************基础配置***********************/
    int m_scrollInterval = 0xFFFFFFF;       // 两资源对象中心点间隔

    Qt::Orientation m_orientation = Qt::Vertical;  // 滚动方向：水平/垂直
    typedef int (QPoint::*ScrollPoint)() const;
    ScrollPoint m_fCurPoint;        // 视滚动方向改变为x/y

    bool m_isCircular = false;
    int m_prev = 0, m_next = 0;     // 循环效果需补充的前后资源数

    QColor m_background = QColor(38, 39, 44);
private:
    QPropertyAnimation m_animation;             // 动画

/***********************鼠标状态***********************/
    typedef enum tagScrollMouseDragInfo {
        MOUSE_RELEASE = 0,                      // 鼠标离开
        MOUSE_PRESS = 1,                        // 按下
        MOUSE_PRESS_MOVE = 2,                   // 按下移动
        MOUSE_RELEASE_MOVE = 3                  // 鼠标离开并滑动
    }Scroll_Mouse_Drag_INFO_E;
    Scroll_Mouse_Drag_INFO_E m_dragFlag = MOUSE_RELEASE;

};

class NumberSelector : public QAbstractSelector
{
public:
    explicit NumberSelector(int startNumber = 0,
                                int endNumber = 24,
                                QWidget *parent = nullptr);
protected:
    void paintSelected(QPainter *painter, QPointF point, int index) override;
    void paintUnselected(QPainter *painter, QPointF point, int index) override;

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

class WallpaperSelector : public QAbstractSelector
{
public:
    explicit WallpaperSelector(QWidget *parent = nullptr);
    void InitImageMsgs();

protected:
    void paintSelected(QPainter *painter, QPointF point, int index) override;
    void paintUnselected(QPainter *painter, QPointF point, int index) override;

private:
    QSize imageSize = QSize(320,320);
    int spacer = 20;
    QColor background = Qt::black;   // 选择框背景颜

};

class ScrollSelector : public QWidget
{
    Q_OBJECT
public:
    explicit ScrollSelector(QWidget *parent = nullptr);

};

#endif // SCROLLSELECTOR_H
