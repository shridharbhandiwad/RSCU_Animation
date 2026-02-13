#include "pipe.h"
#include <QPen>
#include <QBrush>
#include <QPainterPath>
#include <QtMath>

Pipe::Pipe(QObject *parent)
    : BaseComponent(parent)
    , m_flowing(false)
    , m_flowForward(true)
    , m_width(6.0)
    , m_fluidColor(QColor(100, 150, 200))
    , m_flowOffset(0.0)
{
    m_boundingRect = QRectF(-100, -100, 200, 200);
}

void Pipe::setPath(const QVector<QPointF> &points)
{
    m_points = points;
    
    if (m_points.size() >= 2) {
        // Calculate bounding rect from points
        double minX = m_points[0].x();
        double maxX = m_points[0].x();
        double minY = m_points[0].y();
        double maxY = m_points[0].y();
        
        for (const QPointF &point : m_points) {
            minX = qMin(minX, point.x());
            maxX = qMax(maxX, point.x());
            minY = qMin(minY, point.y());
            maxY = qMax(maxY, point.y());
        }
        
        double margin = m_width + 5;
        m_boundingRect = QRectF(minX - margin, minY - margin, 
                                maxX - minX + 2 * margin, 
                                maxY - minY + 2 * margin);
    }
    
    update();
}

void Pipe::setFlowing(bool flowing)
{
    m_flowing = flowing;
    m_isActive = flowing;
    update();
}

void Pipe::setFlowDirection(bool forward)
{
    m_flowForward = forward;
    update();
}

void Pipe::setWidth(double width)
{
    m_width = width;
    update();
}

void Pipe::setFluidColor(const QColor &color)
{
    m_fluidColor = color;
    update();
}

void Pipe::updateAnimation(double deltaTime)
{
    if (m_flowing) {
        double speed = 50.0; // pixels per second
        if (m_flowForward) {
            m_flowOffset += speed * deltaTime;
        } else {
            m_flowOffset -= speed * deltaTime;
        }
        
        // Keep offset in reasonable range
        double dashLength = 20.0;
        while (m_flowOffset > dashLength) {
            m_flowOffset -= dashLength;
        }
        while (m_flowOffset < 0) {
            m_flowOffset += dashLength;
        }
    }
    
    BaseComponent::updateAnimation(deltaTime);
}

void Pipe::paintComponent(QPainter *painter)
{
    if (m_points.size() < 2) {
        return;
    }
    
    // Draw pipe outline
    QPen outlinePen(Qt::black, m_width + 2);
    outlinePen.setCapStyle(Qt::RoundCap);
    outlinePen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(outlinePen);
    
    QPainterPath path;
    path.moveTo(m_points[0]);
    for (int i = 1; i < m_points.size(); ++i) {
        path.lineTo(m_points[i]);
    }
    painter->drawPath(path);
    
    // Draw pipe inner
    QColor pipeColor = m_flowing ? m_fluidColor.lighter(150) : QColor(200, 200, 200);
    QPen innerPen(pipeColor, m_width);
    innerPen.setCapStyle(Qt::RoundCap);
    innerPen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(innerPen);
    painter->drawPath(path);
    
    // Draw flow animation
    if (m_flowing) {
        QPen flowPen(m_fluidColor.darker(120), m_width * 0.4);
        flowPen.setCapStyle(Qt::RoundCap);
        flowPen.setJoinStyle(Qt::RoundJoin);
        flowPen.setDashPattern({10, 10});
        flowPen.setDashOffset(m_flowOffset);
        
        painter->setPen(flowPen);
        painter->drawPath(path);
        
        // Draw flow direction arrows
        for (int i = 0; i < m_points.size() - 1; ++i) {
            QPointF p1 = m_points[i];
            QPointF p2 = m_points[i + 1];
            
            QPointF mid = (p1 + p2) / 2.0;
            QPointF dir = p2 - p1;
            double length = qSqrt(dir.x() * dir.x() + dir.y() * dir.y());
            
            if (length > 30) {
                dir /= length;
                
                if (!m_flowForward) {
                    dir = -dir;
                }
                
                QPointF perp(-dir.y(), dir.x());
                
                QPointF arrowTip = mid + dir * 8;
                QPointF arrowLeft = arrowTip - dir * 8 + perp * 4;
                QPointF arrowRight = arrowTip - dir * 8 - perp * 4;
                
                QPainterPath arrow;
                arrow.moveTo(arrowTip);
                arrow.lineTo(arrowLeft);
                arrow.moveTo(arrowTip);
                arrow.lineTo(arrowRight);
                
                QPen arrowPen(m_fluidColor.darker(150), 2);
                painter->setPen(arrowPen);
                painter->drawPath(arrow);
            }
        }
    }
}
