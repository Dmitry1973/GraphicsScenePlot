#include "GraphicsDataItem.h"
#include <QPen>
#include <QPainter>
#include <QLineF>

struct Range{
    double min;
    double max;
};

class GraphicsDataItemPrivate
{
    friend class GraphicsDataItem;

    GraphicsDataItemPrivate();

    QPen m_pen;
    QBrush m_brush;
    QString title;

    Range ordinateRange;
    Range abscissRange;
};

GraphicsDataItemPrivate::GraphicsDataItemPrivate()
{
    m_pen.setCosmetic(true);
}

GraphicsDataItem::GraphicsDataItem(QGraphicsItem *parent):
    QGraphicsObject(parent),
    d_ptr(new GraphicsDataItemPrivate())

{
    d_ptr->title = QString("Some data");
}

GraphicsDataItem::~GraphicsDataItem()
{
    delete d_ptr;
}

void GraphicsDataItem::setPen(const QPen &pen)
{
    Q_D(GraphicsDataItem);
    d->m_pen = pen;
    d->m_pen.setCosmetic(true);
    Q_EMIT penItemChange();
}

QPen GraphicsDataItem::pen()
{
    return d_ptr->m_pen;
}

void GraphicsDataItem::setBrush(const QBrush &brush)
{
    d_ptr->m_brush = brush;
}

QBrush GraphicsDataItem::brush()
{
    return d_ptr->m_brush;
}

void GraphicsDataItem::ordinateRange(double *min, double *max)
{
    Q_D(GraphicsDataItem);
    *min =d->ordinateRange.min;
    *max = d->ordinateRange.max;
}

void GraphicsDataItem::abscissRange(double *min, double *max)
{
    Q_D(GraphicsDataItem);
    *min =d->abscissRange.min;
    *max = d->abscissRange.max;
}

void GraphicsDataItem::setTitle(const QString &title)
{
    d_ptr->title = title;
    Q_EMIT titleChange();
}

QString GraphicsDataItem::title()
{
    return d_ptr->title;
}

void GraphicsDataItem::setOrdinateRange(double min, double max)
{
    Q_D(GraphicsDataItem);
    d->ordinateRange.min = min;
    d->ordinateRange.max = max;
}

void GraphicsDataItem::setAbscissRange(double min, double max)
{
    Q_D(GraphicsDataItem);
    d->abscissRange.min = min;
    d->abscissRange.max = max;
}

class Graphics2DGraphItemPrivate
{
    Q_DECLARE_PUBLIC(Graphics2DGraphItem)
    Graphics2DGraphItem *q_ptr;
    Graphics2DGraphItemPrivate(Graphics2DGraphItem *parent):q_ptr(parent){}
    QVector<QLineF> m_lines;
    template<typename T> void setData( const T & absciss, const T & ordinate, qint32 length)
    {
        q_ptr->prepareGeometryChange();
        --length;
        m_lines.resize(length);

        Range ordinateRange;
        ordinateRange.min = ordinate[0];
            ordinateRange.max = ordinate[0];
        Range abscissRange;
        abscissRange.min = absciss[0];
            abscissRange.max = absciss[0];
        for(int i =0; i < length; ++i)
        {
            if(ordinate[i+1] > ordinateRange.max)
                ordinateRange.max = ordinate[i+1];
            else if(ordinate[i+1] < ordinateRange.min )
                ordinateRange.min = ordinate[i+1];
            if(absciss[i+1] > abscissRange.max)
                abscissRange.max = absciss[i+1];
            else if(absciss[i+1] < abscissRange.min )
                abscissRange.min = absciss[i+1];
            m_lines[i].setLine(absciss[i], ordinate[i], absciss[i+1], ordinate[i+1]);
        }
        m_boundRect.setRect(abscissRange.min, ordinateRange.min, abscissRange.max - abscissRange.min, ordinateRange.max - abscissRange.min);
        q_ptr->setOrdinateRange(ordinateRange.min, ordinateRange.max);
            q_ptr->setAbscissRange(abscissRange.min, abscissRange.max);
        q_ptr->update();
        QMetaObject::invokeMethod(q_ptr, "dataItemChange");
    }

    QRect m_boundRect;
};

Graphics2DGraphItem::Graphics2DGraphItem(QGraphicsItem *parent):
    GraphicsDataItem(parent),
    d_ptr(new Graphics2DGraphItemPrivate(this))
{
    setOrdinateRange(0, 0);
    setAbscissRange(0,0);
}

Graphics2DGraphItem::Graphics2DGraphItem(double *absciss, double *ordinate, int length, QGraphicsItem *parent):
    GraphicsDataItem(parent)
{
    setData(absciss, ordinate, length);
}

Graphics2DGraphItem::~Graphics2DGraphItem()
{
    delete d_ptr;
}

void Graphics2DGraphItem::setData(double *absciss, double *ordinate, int length)
{
    d_ptr->setData<double*>(absciss, ordinate, length);
}

void Graphics2DGraphItem::setData(QList<double> absciss, QList<double> ordinate)
{
    d_ptr->setData<QList<double>>(absciss, ordinate, qMin(absciss.size(), ordinate.size()));
}

void Graphics2DGraphItem::setData(QVector<double> absciss, QVector<double> ordinate)
{
    d_ptr->setData<QVector<double>>(absciss, ordinate, qMin(absciss.size(), ordinate.size()));
}

QRectF Graphics2DGraphItem::boundingRect() const
{
    return d_ptr->m_boundRect;
}

void Graphics2DGraphItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawLines(d_ptr->m_lines);
}


QRectF Graphics2DHistogramItem::boundingRect() const
{
    return QRectF();
}

 void Graphics2DHistogramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
     Q_UNUSED(painter) Q_UNUSED(option) Q_UNUSED(widget)
}



