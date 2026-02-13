#include "blower.h"
#include <QPen>
#include <QBrush>
#include <QtMath>

Blower::Blower(int id, QObject *parent)
    : BaseComponent(parent)
    , m_id(id)
    , m_running(false)
    , m_speed(0.0)
    , m_rotationAngle(0.0)
{
    m_boundingRect = QRectF(-25, -25, 50, 50);
}

void Blower::setRunning(bool running)
{
    m_running = running;
    m_isActive = running;
    update();
}

void Blower::setSpeed(double speed)
{
    m_speed = qBound(0.0, speed, 100.0);
    update();
}

void Blower::updateAnimation(double deltaTime)
{
    if (m_running) {
        double rotationSpeed = 180.0 + (m_speed / 100.0) * 720.0; // 180-900 degrees/sec
        m_rotationAngle += deltaTime * rotationSpeed;
        if (m_rotationAngle > 360.0) {
            m_rotationAngle -= 360.0;
        }
    }
    
    BaseComponent::updateAnimation(deltaTime);
}

void Blower::paintComponent(QPainter *painter)
{
    // Draw blower housing
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(QBrush(QColor(150, 150, 170)));
    painter->drawEllipse(QPointF(0, 0), 22, 22);
    
    // Draw fan blades (rotating when running)
    painter->save();
    painter->rotate(m_rotationAngle);
    
    QColor bladeColor = m_running ? QColor(100, 150, 200) : QColor(120, 120, 120);
    QPen bladePen(bladeColor, 3);
    painter->setPen(bladePen);
    
    int numBlades = 6;
    for (int i = 0; i < numBlades; ++i) {
        double angle = (i * 360.0 / numBlades);
        double rad = qDegreesToRadians(angle);
        
        QPainterPath bladePath;
        bladePath.moveTo(QPointF(0, 0));
        
        double x1 = 18 * qCos(rad);
        double y1 = 18 * qSin(rad);
        double x2 = 15 * qCos(rad + 0.3);
        double y2 = 15 * qSin(rad + 0.3);
        
        bladePath.lineTo(QPointF(x1, y1));
        bladePath.quadTo(QPointF(x2, y2), QPointF(0, 0));
        
        painter->fillPath(bladePath, QBrush(bladeColor));
        painter->drawPath(bladePath);
    }
    
    painter->restore();
    
    // Draw center hub
    painter->setBrush(QBrush(Qt::darkGray));
    painter->setPen(QPen(Qt::black, 1));
    painter->drawEllipse(QPointF(0, 0), 5, 5);
    
    // Draw housing ring
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::black, 3));
    painter->drawEllipse(QPointF(0, 0), 22, 22);
    
    // Draw status indicator
    QColor statusColor = m_running ? Qt::green : Qt::red;
    painter->setBrush(QBrush(statusColor));
    painter->setPen(QPen(Qt::black, 1));
    painter->drawEllipse(QPointF(18, -18), 5, 5);
}
