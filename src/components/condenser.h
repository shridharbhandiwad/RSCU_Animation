#ifndef CONDENSER_H
#define CONDENSER_H

#include "basecomponent.h"

class Condenser : public BaseComponent
{
    Q_OBJECT

public:
    explicit Condenser(int id, QObject *parent = nullptr);
    
    void setActive(bool active);
    bool isActive() const { return m_active; }
    
    void setTemperature(double temp);
    double getTemperature() const { return m_temperature; }
    
    void updateAnimation(double deltaTime) override;

protected:
    void paintComponent(QPainter *painter) override;

private:
    int m_id;
    bool m_active;
    double m_temperature;
};

#endif // CONDENSER_H
