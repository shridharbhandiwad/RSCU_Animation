#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QObject>

class BaseComponent : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit BaseComponent(QObject *parent = nullptr);
    virtual ~BaseComponent() = default;
    
    virtual void updateAnimation(double deltaTime);
    virtual void updateState();
    
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    virtual void paintComponent(QPainter *painter) = 0;
    
    QRectF m_boundingRect;
    bool m_isActive;
    double m_animationPhase;
};

#endif // BASECOMPONENT_H
