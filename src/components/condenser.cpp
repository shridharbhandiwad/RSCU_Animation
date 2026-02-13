#include "condenser.h"
#include <QPen>
#include <QBrush>
#include <QtMath>

Condenser::Condenser(int id, QObject *parent)
    : BaseComponent(parent)
    , m_id(id)
    , m_active(false)
    , m_temperature(25.0)
{
    m_boundingRect = QRectF(-30, -40, 60, 80);
}

void Condenser::setActive(bool active)
{
    m_active = active;
    m_isActive = active;
    update();
}

void Condenser::setTemperature(double temp)
{
    m_temperature = temp;
    update();
}

void Condenser::updateAnimation(double deltaTime)
{
    BaseComponent::updateAnimation(deltaTime);
}

void Condenser::paintComponent(QPainter *painter)
{
    // Draw condenser body (finned coil)
    QRectF bodyRect(-25, -35, 50, 70);
    
    QColor bodyColor = m_active ? QColor(200, 150, 100) : QColor(180, 180, 180);
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(QBrush(bodyColor));
    painter->drawRect(bodyRect);
    
    // Draw cooling fins
    QPen finPen(Qt::darkGray, 1);
    painter->setPen(finPen);
    
    for (int i = 0; i < 12; ++i) {
        double y = -32 + (i * 6);
        painter->drawLine(QPointF(-25, y), QPointF(25, y));
    }
    
    // Draw refrigerant tubes
    QPen tubePen(Qt::black, 2);
    painter->setPen(tubePen);
    painter->setBrush(QBrush(QColor(100, 100, 150)));
    
    // Left tube
    painter->drawRect(QRectF(-20, -35, 8, 70));
    
    // Right tube
    painter->drawRect(QRectF(12, -35, 8, 70));
    
    // Draw connection ports
    painter->setBrush(QBrush(Qt::darkGray));
    painter->drawEllipse(QPointF(-16, -40), 4, 4);
    painter->drawEllipse(QPointF(16, -40), 4, 4);
    painter->drawEllipse(QPointF(-16, 40), 4, 4);
    painter->drawEllipse(QPointF(16, 40), 4, 4);
    
    // Draw label
    painter->setPen(QPen(Qt::black));
    QFont font = painter->font();
    font.setPointSize(7);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(bodyRect, Qt::AlignCenter, QString("A/C\nCond.\n%1").arg(m_id));
}
