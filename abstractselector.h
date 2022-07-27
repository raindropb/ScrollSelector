#ifndef ABSTRACTSELECTOR_H
#define ABSTRACTSELECTOR_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QPainter>

class AbstractSelector :public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double m_curIndex READ getCurIndex WRITE setCurIndex NOTIFY sigCurIndexChanged)
public:
    explicit AbstractSelector(QWidget *parent = nullptr);
    double getCurIndex() const;
    void setCurIndex(double index);

    // 选择对象列表
    void setResourceList(QVector<QVariant> resourceList);
    void addResource(QVariant resource);
    const QVector<QVariant> getResourceList() const;
    // 滚动对象间隔
    void setScrollInterval(int interval);
    int getScrollInterval() const;
    // 滚动方向：水平/垂直
    void setOrientation(Qt::Orientation orientation);
    Qt::Orientation getOrientation() const;
    // 循环滚动及补全
    void setCircularPrevNext(bool circular, int prev, int next);
    std::tuple<bool,int,int> getCircularPrevNext() const;
    void checkCircular();

protected:
    // 绘制选中/未选中对象/背景
    void virtual paintSelected(QPainter *painter, QPointF point, int index);
    void virtual paintUnselected(QPainter *painter, QPointF point, int index);
    void virtual paintBackground(QPainter *painter, QRectF rectf);
    QPointF getBeginPoint();
    QPointF getObjectCenter(int index);
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;
    void autoStart();

signals:
    void sigCurIndexChanged(int index);

protected:
    double m_curIndex = 0.0;
    int m_curRoundIndex = 0;

/***********************资源对象***********************/
    QVector<QVariant> m_resourceList;
    int m_resourceCount = 0;
/***********************基础配置***********************/
    int m_scrollInterval = 0xFFFFFFF;       // 两资源对象中心点间隔

    Qt::Orientation m_orientation = Qt::Vertical;  // 滚动方向：水平/垂直
    typedef int (QPoint::*ScrollPoint)() const;
    ScrollPoint m_fCurPoint = &QPoint::y;        // 视滚动方向改变为x/y

    bool m_isCircular = false;
    int m_prev = 0, m_next = 0;     // 循环效果需补充的前后资源数

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

#endif // ABSTRACTSELECTOR_H
