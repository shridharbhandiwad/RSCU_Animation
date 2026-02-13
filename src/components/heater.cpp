#include "heater.h"
#include <QPen>
#include <QBrush>
#include <QRadialGradient>
#include <QtMath>

Heater::Heater(QObject *parent)
    : BaseComponent(parent)
    , m_power(0.0)
    , m_active(false)
    , m_glowIntensity(0.0)
{
    m_boundingRect = QRectF(-30, -30, 60, 60);
}

void Heater::setPower(double power)
{
    m_power = power;
    update();
}

void Heater::setActive(bool active)
{
    m_active = active;
    m_isActive = active;
    update();
}

void Heater::updateAnimation(double deltaTime)
{
    if (m_active) {
        m_glowIntensity = 0.5 + 0.5 * qSin(m_animationPhase * 3.0);
    } else {
        m_glowIntensity = 0.0;
    }
    
    BaseComponent::updateAnimation(deltaTime);
}

void Heater::paintComponent(QPainter *painter)
{
    // Draw heater body
    painter->setPen(QPen(Qt::black, 3));
    painter->setBrush(QBrush(QColor(150, 150, 150)));
    painter->drawRect(QRectF(-25, -25, 50, 50));
    
    // Draw heating elements
    if (m_active && m_power > 0) {
        // Glow effect when active
        QRadialGradient glowGradient(QPointF(0, 0), 20);
        QColor glowColor = QColor(255, 100, 0);
        glowColor.setAlpha(int(m_glowIntensity * 200));
        glowGradient.setColorAt(0, glowColor);
        glowGradient.setColorAt(1, Qt::transparent);
        
        painter->setBrush(QBrush(glowGradient));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPointF(0, 0), 20, 20);
    }
    
    // Draw heating coils
    QPen coilPen(m_active ? QColor(255, 150, 0) : QColor(100, 100, 100), 3);
    painter->setPen(coilPen);
    painter->setBrush(Qt::NoBrush);
    
    for (int i = -2; i <= 2; ++i) {
        double y = i * 8;
        QPainterPath path;
        path.moveTo(-15, y);
        
        for (double x = -15; x <= 15; x += 3) {
            double wave = qSin(x * 0.5) * 2;
            path.lineTo(x, y + wave);
        }
        
        painter->drawPath(path);
    }
    
    // Draw frame
    painter->setPen(QPen(Qt::black, 3));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(QRectF(-25, -25, 50, 50));
    
    // Draw power indicator
    QColor indicatorColor = m_active ? Qt::red : Qt::darkGray;
    painter->setBrush(QBrush(indicatorColor));
    painter->setPen(QPen(Qt::black, 1));
    painter->drawEllipse(QPointF(20, -20), 5, 5);
}
