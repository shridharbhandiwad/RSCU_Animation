#ifndef TANK_H
#define TANK_H

#include "basecomponent.h"

class Tank : public BaseComponent
{
    Q_OBJECT

public:
    explicit Tank(QObject *parent = nullptr);
    
    void setLevel(double level); // 0.0 to 100.0
    double getLevel() const { return m_level; }
    
    void setTemperature(double temp);
    double getTemperature() const { return m_temperature; }

protected:
    void paintComponent(QPainter *painter) override;

private:
    double m_level;
    double m_temperature;
};

#endif // TANK_H
