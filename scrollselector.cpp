#include "scrollselector.h"
#include <QDebug>

ScrollSelector::ScrollSelector(QWidget *parent) : QWidget(parent)
{

}

QAbstractSelector::QAbstractSelector(QWidget *parent)
    : QWidget(parent)
{
    m_animation.setTargetObject(this);
    m_animation.setPropertyName("m_curIndex");
    m_animation.setEasingCurve(QEasingCurve::OutQuart);
}

double QAbstractSelector::getCurIndex() const
{
    return  m_curIndex;
}

void QAbstractSelector::setCurIndex(double index)
{
    if(m_isCircular)
    {
        if(index > m_resourceCount)
            index -= m_resourceCount;
        if(index < 0)
            index += m_resourceCount;
    }
    if(qRound(index) != qRound(m_curIndex))
    {
        m_curIndex = index;
        emit sigCurIndexChanged(qRound(m_curIndex));
    }
    //属性动画设置值会调用该接口，此处计算位置并刷新
    m_curIndex=index;
    update();
}

void QAbstractSelector::paintSelected(QPainter *painter, QPointF point, int index)
{
    Q_UNUSED(painter)
    Q_UNUSED(point)
    Q_UNUSED(index)
}

void QAbstractSelector::paintUnselected(QPainter *painter, QPointF point, int index)
{
    Q_UNUSED(painter)
    Q_UNUSED(point)
    Q_UNUSED(index)
}

QPointF QAbstractSelector::getObjectCenter(int index)
{
    if(m_orientation == Qt::Horizontal)
        return QPointF(m_scrollInterval*index,0);
    else
        return QPointF(0,m_scrollInterval*index);
}

QPointF QAbstractSelector::getBeginPoint()
{
    if(m_orientation == Qt::Horizontal)
        return QPointF(width()/2-m_scrollInterval*getCurIndex(),height()/2);
    else
        return QPointF(width()/2,height()/2-m_scrollInterval*getCurIndex());
}

void QAbstractSelector::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if(m_resourceCount == 0)
        return;
    QPainter painter(this);
    painter.fillRect(rect(),m_background);
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

bool QAbstractSelector::event(QEvent *event)
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

void QAbstractSelector::autoStart()
{
    int index = qRound(getCurIndex());
    if ( index > m_resourceCount-1) index = m_resourceCount-1;
    if ( index < 0 ) index = 0;
    int dutime = 1000 * qAbs(getCurIndex() - (double) index);
    m_animation.stop();
    m_animation.setDuration(dutime);
    m_animation.setStartValue(getCurIndex());
    m_animation.setEndValue((double)index);
    m_animation.start();
}


void QAbstractSelector::setResourceList(QVector<QVariant> resourceList)
{
    m_resourceList = resourceList;
    m_resourceCount = m_resourceList.size();
}

void QAbstractSelector::addResource(QVariant resource)
{
    m_resourceList.append(resource);
    m_resourceCount = m_resourceList.size();
}

Qt::Orientation QAbstractSelector::getOrientation() const
{
    return m_orientation;
}

void QAbstractSelector::setScrollInterval(int interval)
{
    m_scrollInterval = interval;
}

QVector<QVariant> QAbstractSelector::getResourceList() const
{
    return m_resourceList;
}

void QAbstractSelector::setOrientation(Qt::Orientation orientation)
{
    m_orientation = orientation;
    if(orientation == Qt::Horizontal)
        m_fCurPoint = &QPoint::x;
    else
        m_fCurPoint = &QPoint::y;
}

int QAbstractSelector::getScrollInterval() const
{
    return m_scrollInterval;
}

void QAbstractSelector::setCircularPrevNext(bool circular, int prev, int next)
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

std::tuple<bool, int, int> QAbstractSelector::getCircularPrevNext() const
{
    return std::make_tuple(m_isCircular,m_prev,m_next);
}

void QAbstractSelector::setBackground(QColor color)
{
    m_background = color;
}

void QAbstractSelector::checkCircular()
{
    // 数字较少时取消补全
    if(m_resourceCount < (m_prev+m_next+1))
        m_prev = m_next = 0;
}

NumberSelector::NumberSelector(int startNumber, int endNumber, QWidget *parent)
    : QAbstractSelector(parent),
      m_start(startNumber),
      m_end(endNumber)
{
    setOrientation(Qt::Horizontal);               // 滚动方向
    setBackground(background);                  // 背景颜色
    setScrollInterval(spacer+size.width());    // 字体间隔
    setCircularPrevNext(true,3,4);              // 循环补全
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

WallpaperSelector::WallpaperSelector(QWidget *parent)
    :QAbstractSelector(parent)
{
    setOrientation(Qt::Horizontal);               // 滚动方向
    setBackground(background);                  // 背景颜色
    setScrollInterval(spacer+imageSize.width());    // 字体间隔
    setCircularPrevNext(true,1,2);              // 循环补全
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
