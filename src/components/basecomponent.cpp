#include "basecomponent.h"

BaseComponent::BaseComponent(QObject *parent)
    : QObject(parent)
    , m_boundingRect(0, 0, 50, 50)
    , m_isActive(false)
    , m_animationPhase(0.0)
{
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

void BaseComponent::updateAnimation(double deltaTime)
{
    if (m_isActive) {
        m_animationPhase += deltaTime * 2.0; // Animation speed
        if (m_animationPhase > 2.0 * M_PI) {
            m_animationPhase -= 2.0 * M_PI;
        }
        update();
    }
}

void BaseComponent::updateState()
{
    update();
}

QRectF BaseComponent::boundingRect() const
{
    return m_boundingRect;
}

void BaseComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    painter->setRenderHint(QPainter::Antialiasing);
    paintComponent(painter);
}
