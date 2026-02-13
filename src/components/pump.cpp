#include "pump.h"
#include <QPen>
#include <QBrush>
#include <QtMath>

Pump::Pump(int pumpId, QObject *parent)
    : BaseComponent(parent)
    , m_pumpId(pumpId)
    , m_running(false)
    , m_flowRate(0.0)
    , m_rotationAngle(0.0)
{
    m_boundingRect = QRectF(-25, -25, 50, 50);
}

void Pump::setRunning(bool running)
{
    m_running = running;
    m_isActive = running;
    update();
}

void Pump::setFlowRate(double flowRate)
{
    m_flowRate = flowRate;
    update();
}

void Pump::updateAnimation(double deltaTime)
{
    if (m_running) {
        m_rotationAngle += deltaTime * 360.0; // Degrees per second
        if (m_rotationAngle > 360.0) {
            m_rotationAngle -= 360.0;
        }
        m_animationPhase += deltaTime * 5.0;
        if (m_animationPhase > 2.0 * M_PI) {
            m_animationPhase -= 2.0 * M_PI;
        }
    }
    BaseComponent::updateAnimation(deltaTime);
}

void Pump::paintComponent(QPainter *painter)
{
    // Draw pump body
    QColor pumpColor = m_running ? QColor(50, 150, 50) : QColor(150, 150, 150);
    painter->setBrush(QBrush(pumpColor));
    painter->setPen(QPen(Qt::black, 2));
    painter->drawEllipse(QPointF(0, 0), 20, 20);
    
    // Draw pump impeller (rotating when running)
    if (m_running) {
        painter->save();
        painter->rotate(m_rotationAngle);
        
        QPen bladePen(Qt::white, 3);
        painter->setPen(bladePen);
        
        for (int i = 0; i < 4; ++i) {
            double angle = i * 90.0;
            double rad = qDegreesToRadians(angle);
            painter->drawLine(QPointF(0, 0), 
                            QPointF(15 * qCos(rad), 15 * qSin(rad)));
        }
        
        painter->restore();
    }
    
    // Draw center dot
    painter->setBrush(QBrush(Qt::black));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(0, 0), 3, 3);
    
    // Draw status indicator
    QColor statusColor = m_running ? Qt::green : Qt::red;
    painter->setBrush(QBrush(statusColor));
    painter->setPen(QPen(Qt::black, 1));
    painter->drawEllipse(QPointF(15, -15), 5, 5);
}
