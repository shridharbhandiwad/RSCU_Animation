#ifndef HEATER_H
#define HEATER_H

#include "basecomponent.h"

class Heater : public BaseComponent
{
    Q_OBJECT

public:
    explicit Heater(QObject *parent = nullptr);
    
    void setPower(double power); // kW
    double getPower() const { return m_power; }
    
    void setActive(bool active);
    bool isActive() const { return m_active; }
    
    void updateAnimation(double deltaTime) override;

protected:
    void paintComponent(QPainter *painter) override;

private:
    double m_power;
    bool m_active;
    double m_glowIntensity;
};

#endif // HEATER_H
