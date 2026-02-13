#include "tank.h"
#include <QPen>
#include <QBrush>
#include <QLinearGradient>

Tank::Tank(QObject *parent)
    : BaseComponent(parent)
    , m_level(75.0)
    , m_temperature(25.0)
{
    m_boundingRect = QRectF(-40, -60, 80, 120);
}

void Tank::setLevel(double level)
{
    m_level = qBound(0.0, level, 100.0);
    update();
}

void Tank::setTemperature(double temp)
{
    m_temperature = temp;
    update();
}

void Tank::paintComponent(QPainter *painter)
{
    // Draw tank body
    QRectF tankRect(-35, -55, 70, 110);
    painter->setPen(QPen(Qt::black, 3));
    painter->setBrush(QBrush(QColor(180, 200, 220)));
    painter->drawRect(tankRect);
    
    // Draw liquid level
    double liquidHeight = (m_level / 100.0) * 100.0;
    QRectF liquidRect(-35, 55 - liquidHeight, 70, liquidHeight);
    
    // Color based on temperature
    QColor liquidColor;
    if (m_temperature < 20.0) {
        liquidColor = QColor(100, 150, 255); // Cold - blue
    } else if (m_temperature < 30.0) {
        liquidColor = QColor(100, 200, 200); // Normal - cyan
    } else {
        liquidColor = QColor(255, 150, 100); // Hot - orange
    }
    
    QLinearGradient gradient(liquidRect.topLeft(), liquidRect.bottomLeft());
    gradient.setColorAt(0, liquidColor.lighter(120));
    gradient.setColorAt(1, liquidColor);
    
    painter->setBrush(QBrush(gradient));
    painter->setPen(Qt::NoPen);
    painter->drawRect(liquidRect);
    
    // Draw level markers
    painter->setPen(QPen(Qt::black, 1));
    for (int i = 0; i <= 4; ++i) {
        double y = -55 + (i * 25);
        painter->drawLine(QPointF(-35, y), QPointF(-30, y));
        painter->drawLine(QPointF(30, y), QPointF(35, y));
    }
    
    // Draw tank outline again to cover edges
    painter->setPen(QPen(Qt::black, 3));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(tankRect);
    
    // Draw top cap
    painter->setBrush(QBrush(QColor(150, 170, 190)));
    QRectF topCap(-40, -60, 80, 10);
    painter->drawRect(topCap);
    
    // Draw bottom
    painter->drawRect(QRectF(-40, 50, 80, 10));
    
    // Draw connection points
    painter->setBrush(QBrush(Qt::darkGray));
    painter->setPen(QPen(Qt::black, 2));
    painter->drawEllipse(QPointF(-40, 0), 5, 5);
    painter->drawEllipse(QPointF(40, 0), 5, 5);
}
