#include "valve.h"
#include <QPen>
#include <QBrush>
#include <QtMath>

Valve::Valve(int valveId, ValveType type, QObject *parent)
    : BaseComponent(parent)
    , m_valveId(valveId)
    , m_type(type)
    , m_open(false)
    , m_position(0.0)
    , m_targetPosition(0.0)
{
    m_boundingRect = QRectF(-20, -20, 40, 40);
}

void Valve::setOpen(bool open)
{
    m_open = open;
    m_targetPosition = open ? 1.0 : 0.0;
    m_isActive = true;
    update();
}

void Valve::setPosition(double position)
{
    m_position = qBound(0.0, position, 1.0);
    update();
}

void Valve::updateAnimation(double deltaTime)
{
    // Smooth valve movement
    if (qAbs(m_position - m_targetPosition) > 0.01) {
        double step = deltaTime * 2.0; // Speed of valve movement
        if (m_position < m_targetPosition) {
            m_position = qMin(m_position + step, m_targetPosition);
        } else {
            m_position = qMax(m_position - step, m_targetPosition);
        }
        update();
    } else {
        m_position = m_targetPosition;
        m_isActive = false;
    }
    
    BaseComponent::updateAnimation(deltaTime);
}

void Valve::paintComponent(QPainter *painter)
{
    // Draw valve body (diamond shape)
    QPolygonF valveBody;
    valveBody << QPointF(0, -15) << QPointF(15, 0) << QPointF(0, 15) << QPointF(-15, 0);
    
    QColor bodyColor = m_open ? QColor(100, 200, 100) : QColor(200, 100, 100);
    painter->setBrush(QBrush(bodyColor));
    painter->setPen(QPen(Qt::black, 2));
    painter->drawPolygon(valveBody);
    
    // Draw valve gate/disc based on position
    painter->save();
    
    if (m_type == BallValve) {
        // Ball valve - rotate indicator
        double angle = m_position * 90.0; // 0 to 90 degrees
        painter->rotate(angle);
        painter->setPen(QPen(Qt::black, 3));
        painter->drawLine(QPointF(-10, 0), QPointF(10, 0));
    } else if (m_type == GateValve) {
        // Gate valve - vertical movement
        double yPos = -10.0 + (m_position * 20.0);
        painter->setPen(QPen(Qt::black, 4));
        painter->drawLine(QPointF(-8, yPos), QPointF(8, yPos));
    } else {
        // Check valve - arrow
        QPen arrowPen(Qt::black, 2);
        painter->setPen(arrowPen);
        QPolygonF arrow;
        arrow << QPointF(-5, 5) << QPointF(0, -5) << QPointF(5, 5);
        painter->drawPolyline(arrow);
    }
    
    painter->restore();
    
    // Draw connection points
    painter->setBrush(QBrush(Qt::darkGray));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(-15, 0), 3, 3);
    painter->drawEllipse(QPointF(15, 0), 3, 3);
    
    // Draw status indicator
    QColor statusColor = m_open ? Qt::green : Qt::red;
    painter->setBrush(QBrush(statusColor));
    painter->setPen(QPen(Qt::black, 1));
    painter->drawEllipse(QPointF(0, -18), 4, 4);
}
