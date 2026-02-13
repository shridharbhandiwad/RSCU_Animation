#ifndef PUMP_H
#define PUMP_H

#include "basecomponent.h"

class Pump : public BaseComponent
{
    Q_OBJECT

public:
    explicit Pump(int pumpId, QObject *parent = nullptr);
    
    void setRunning(bool running);
    bool isRunning() const { return m_running; }
    
    void setFlowRate(double flowRate);
    double getFlowRate() const { return m_flowRate; }
    
    void updateAnimation(double deltaTime) override;

protected:
    void paintComponent(QPainter *painter) override;

private:
    int m_pumpId;
    bool m_running;
    double m_flowRate;
    double m_rotationAngle;
};

#endif // PUMP_H
