#include "solenoidvalve.h"
#include <QPen>
#include <QBrush>
#include <QtMath>

SolenoidValve::SolenoidValve(int id, QObject *parent)
    : BaseComponent(parent)
    , m_id(id)
    , m_open(false)
    , m_energized(false)
    , m_pulsePhase(0.0)
{
    m_boundingRect = QRectF(-15, -25, 30, 50);
}

void SolenoidValve::setOpen(bool open)
{
    m_open = open;
    update();
}

void SolenoidValve::setEnergized(bool energized)
{
    m_energized = energized;
    m_isActive = energized;
    update();
}

void SolenoidValve::updateAnimation(double deltaTime)
{
    if (m_energized) {
        m_pulsePhase += deltaTime * 4.0;
        if (m_pulsePhase > 2.0 * M_PI) {
            m_pulsePhase -= 2.0 * M_PI;
        }
    }
    
    BaseComponent::updateAnimation(deltaTime);
}

void SolenoidValve::paintComponent(QPainter *painter)
{
    // Draw valve body
    QRectF bodyRect(-12, -5, 24, 20);
    QColor bodyColor = m_open ? QColor(100, 200, 100) : QColor(200, 100, 100);
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(QBrush(bodyColor));
    painter->drawRect(bodyRect);
    
    // Draw solenoid coil
    QRectF coilRect(-10, -20, 20, 15);
    QColor coilColor = m_energized ? QColor(255, 200, 0) : QColor(150, 150, 150);
    
    if (m_energized) {
        // Add pulse effect when energized
        double intensity = 0.7 + 0.3 * qSin(m_pulsePhase);
        coilColor = QColor(int(255 * intensity), int(200 * intensity), 0);
    }
    
    painter->setBrush(QBrush(coilColor));
    painter->drawRect(coilRect);
    
    // Draw coil windings
    QPen windingPen(Qt::black, 1);
    painter->setPen(windingPen);
    for (int i = 0; i < 5; ++i) {
        double y = -18 + (i * 3);
        painter->drawLine(QPointF(-10, y), QPointF(10, y));
    }
    
    // Draw plunger
    QRectF plungerRect(-3, m_open ? -15 : -10, 6, 15);
    painter->setBrush(QBrush(Qt::darkGray));
    painter->setPen(QPen(Qt::black, 1));
    painter->drawRect(plungerRect);
    
    // Draw inlet/outlet ports
    painter->setBrush(QBrush(Qt::black));
    painter->drawEllipse(QPointF(0, -5), 3, 3);
    painter->drawEllipse(QPointF(0, 15), 3, 3);
    
    // Draw side ports
    painter->drawEllipse(QPointF(-12, 5), 3, 3);
    painter->drawEllipse(QPointF(12, 5), 3, 3);
    
    // Draw label
    painter->setPen(QPen(Qt::white));
    QFont font = painter->font();
    font.setPointSize(6);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(bodyRect, Qt::AlignCenter, QString("SV%1").arg(m_id));
}
