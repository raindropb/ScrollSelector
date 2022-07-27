#include "abstractselector.h"

AbstractSelector::AbstractSelector(QWidget *parent)
    : QWidget(parent)
{
    // 动画
    m_animation.setTargetObject(this);
    m_animation.setPropertyName("m_curIndex");
    m_animation.setEasingCurve(QEasingCurve::OutQuart);
}

double AbstractSelector::getCurIndex() const
{
    return  m_curIndex;
}

void AbstractSelector::setCurIndex(double index)
{
    // 判断是否启用循环
    m_curIndex = index;
    if(m_isCircular)
    {
        if(qRound(m_curIndex)>m_resourceCount-1)
            m_curIndex -= m_resourceCount;
        else if(qRound(m_curIndex) < 0)
            m_curIndex += m_resourceCount;
    }
    if(qRound(m_curIndex) != m_curRoundIndex && qRound(m_curIndex) >=0 && qRound(m_curIndex) <= m_resourceCount-1)
    {
        m_curRoundIndex = qRound(m_curIndex);
        emit sigCurIndexChanged(m_curRoundIndex);
    }
    //属性动画设置值会调用该接口，此处计算位置并刷新
    update();
}

void AbstractSelector::setResourceList(QVector<QVariant> resourceList)
{
    m_resourceList = resourceList;
    m_resourceCount = m_resourceList.size();
}

void AbstractSelector::addResource(QVariant resource)
{
    m_resourceList.append(resource);
    m_resourceCount = m_resourceList.size();
}

const QVector<QVariant> AbstractSelector::getResourceList() const
{
    return m_resourceList;
}

void AbstractSelector::setScrollInterval(int interval)
{
    m_scrollInterval = interval;
}

int AbstractSelector::getScrollInterval() const
{
    return m_scrollInterval;
}

void AbstractSelector::setOrientation(Qt::Orientation orientation)
{
    m_orientation = orientation;
    if(orientation == Qt::Vertical)
        m_fCurPoint = &QPoint::y;
    else
        m_fCurPoint = &QPoint::x;
}

Qt::Orientation AbstractSelector::getOrientation() const
{
    return m_orientation;
}

void AbstractSelector::setCircularPrevNext(bool circular, int prev, int next)
{
    m_isCircular = circular;
    if(m_isCircular)
    {
        m_prev = prev;
        m_next = next;
    }
    else
        m_prev = m_next = 0;
}

std::tuple<bool, int, int> AbstractSelector::getCircularPrevNext() const
{
    return std::make_tuple(m_isCircular,m_prev,m_next);
}

void AbstractSelector::checkCircular()
{
    // 选择对象较少时，取消前后补全
    if(m_resourceCount < (m_prev+m_next+1))
        m_prev = m_next = 0;
}

void AbstractSelector::paintSelected(QPainter *painter, QPointF point, int index)
{
    Q_UNUSED(painter)
    Q_UNUSED(point)
    Q_UNUSED(index)
}

void AbstractSelector::paintUnselected(QPainter *painter, QPointF point, int index)
{
    Q_UNUSED(painter)
    Q_UNUSED(point)
    Q_UNUSED(index)
}

void AbstractSelector::paintBackground(QPainter *painter, QRectF rectf)
{
    Q_UNUSED(painter)
    Q_UNUSED(rectf)
}

QPointF AbstractSelector::getBeginPoint()
{
    if(m_orientation == Qt::Vertical)
        return QPointF(width()/2,height()/2-m_scrollInterval*getCurIndex());
    else
        return QPointF(width()/2-m_scrollInterval*getCurIndex(),height()/2);

}

QPointF AbstractSelector::getObjectCenter(int index)
{
    if(m_orientation == Qt::Vertical)
        return QPointF(0,m_scrollInterval*index);
    else
        return QPointF(m_scrollInterval*index,0);
}

void AbstractSelector::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if(m_resourceCount == 0)
        return;
    QPainter painter(this);
    paintBackground(&painter,this->rect());
    painter.save();
    //平移到中心点绘制，便于计算
    painter.translate(getBeginPoint());
    for (int index=-m_prev; index < m_resourceCount+m_next; index++)
    {
        QPointF center = getObjectCenter(index);
        if((int)m_curIndex == index)
        {
            paintSelected(&painter,center,index);
        }
        else
        {
            paintUnselected(&painter,center,index);
        }
    }
    painter.restore();
}

bool AbstractSelector::event(QEvent *event)
{
    // 无有效选择区域，不予处理
    if(m_resourceCount==0)
        return QWidget::event(event);
    QMouseEvent *mouse =  dynamic_cast<QMouseEvent *>(event);
    static int dragPoint = -1;         //拖动时的坐标
    if(mouse && m_animation.state() == QAbstractAnimation::Stopped)
    {
        int curMousePoint = (mouse->pos().*m_fCurPoint)();
        if ( mouse->type() ==QEvent::MouseButtonPress){   //首次按下
          dragPoint  = curMousePoint;
          m_dragFlag = MOUSE_PRESS;
        } else if(mouse->type() == QEvent::MouseButtonRelease &&
           m_dragFlag == MOUSE_PRESS){
           m_dragFlag = MOUSE_RELEASE;
        } else if(mouse->type() == QEvent::MouseMove &&
                  m_dragFlag == MOUSE_PRESS) {                  //初次滑动,判断移动阀值,避免误操作
            if(qAbs(dragPoint - curMousePoint) > 2){
               dragPoint = curMousePoint;
               m_dragFlag = MOUSE_PRESS_MOVE;
            }
        } else if(mouse->type() == QEvent::MouseMove &&
                   m_dragFlag== MOUSE_PRESS_MOVE ){
            int offset = ( curMousePoint - dragPoint);
                        double offsetIndex  = double(offset)/double(m_scrollInterval);
                        double index = getCurIndex() - offsetIndex;
                        setCurIndex(index);
                        dragPoint = curMousePoint;
        } else if(mouse->type() == QEvent::MouseButtonRelease &&
                   m_dragFlag == MOUSE_PRESS_MOVE){
                autoStart();
                m_dragFlag = MOUSE_RELEASE;
        }
    }
    return QWidget::event(event);
}

void AbstractSelector::autoStart()
{
    int index = qRound(getCurIndex());
    int dutime;
    if(!m_isCircular)
    {
        if ( index > m_resourceCount-1) index = m_resourceCount-1;
        if ( index < 0 ) index = 0;
        dutime = qMin((int)(1000 * qAbs(getCurIndex() - (double)index)), 1000);
    }
    else
        dutime = 1000 * qAbs(getCurIndex() - (double)index);
    m_animation.stop();
    m_animation.setDuration(dutime);
    m_animation.setStartValue(getCurIndex());
    m_animation.setEndValue((double)index);
    m_animation.start();
}
