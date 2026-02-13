#include "heatexchanger.h"
#include <QPen>
#include <QBrush>
#include <QtMath>

HeatExchanger::HeatExchanger(int id, QObject *parent)
    : BaseComponent(parent)
    , m_id(id)
    , m_active(false)
    , m_hotSideTemp(25.0)
    , m_coldSideTemp(15.0)
{
    m_boundingRect = QRectF(-25, -35, 50, 70);
}

void HeatExchanger::setActive(bool active)
{
    m_active = active;
    m_isActive = active;
    update();
}

void HeatExchanger::setHotSideTemp(double temp)
{
    m_hotSideTemp = temp;
    update();
}

void HeatExchanger::setColdSideTemp(double temp)
{
    m_coldSideTemp = temp;
    update();
}

void HeatExchanger::updateAnimation(double deltaTime)
{
    BaseComponent::updateAnimation(deltaTime);
}

void HeatExchanger::paintComponent(QPainter *painter)
{
    // Draw PHE body (Plate Heat Exchanger)
    QRectF bodyRect(-20, -30, 40, 60);
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(QBrush(QColor(180, 180, 200)));
    painter->drawRect(bodyRect);
    
    // Draw plates
    QPen platePen(Qt::darkGray, 1);
    painter->setPen(platePen);
    
    for (int i = 0; i < 8; ++i) {
        double y = -25 + (i * 7);
        painter->drawLine(QPointF(-18, y), QPointF(18, y));
    }
    
    // Draw hot side (top)
    QColor hotColor = m_active ? QColor(255, 150, 100) : QColor(200, 200, 200);
    painter->setBrush(QBrush(hotColor));
    painter->setPen(QPen(Qt::black, 2));
    QRectF hotPort(-10, -35, 20, 8);
    painter->drawRect(hotPort);
    
    // Draw cold side (bottom)
    QColor coldColor = m_active ? QColor(100, 150, 255) : QColor(200, 200, 200);
    painter->setBrush(QBrush(coldColor));
    QRectF coldPort(-10, 27, 20, 8);
    painter->drawRect(coldPort);
    
    // Draw side connections
    painter->setBrush(QBrush(Qt::darkGray));
    painter->drawEllipse(QPointF(-25, 0), 5, 5);
    painter->drawEllipse(QPointF(25, 0), 5, 5);
    
    // Draw label
    painter->setPen(QPen(Qt::black));
    QFont font = painter->font();
    font.setPointSize(8);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(bodyRect, Qt::AlignCenter, QString("PHE %1").arg(m_id));
}
