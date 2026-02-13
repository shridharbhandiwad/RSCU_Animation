#ifndef BLOWER_H
#define BLOWER_H

#include "basecomponent.h"

class Blower : public BaseComponent
{
    Q_OBJECT

public:
    explicit Blower(int id, QObject *parent = nullptr);
    
    void setRunning(bool running);
    bool isRunning() const { return m_running; }
    
    void setSpeed(double speed); // 0.0 to 100.0
    double getSpeed() const { return m_speed; }
    
    void updateAnimation(double deltaTime) override;

protected:
    void paintComponent(QPainter *painter) override;

private:
    int m_id;
    bool m_running;
    double m_speed;
    double m_rotationAngle;
};

#endif // BLOWER_H
