#ifndef VALVE_H
#define VALVE_H

#include "basecomponent.h"

class Valve : public BaseComponent
{
    Q_OBJECT

public:
    enum ValveType {
        BallValve,
        GateValve,
        CheckValve
    };

    explicit Valve(int valveId, ValveType type = BallValve, QObject *parent = nullptr);
    
    void setOpen(bool open);
    bool isOpen() const { return m_open; }
    
    void setPosition(double position); // 0.0 = closed, 1.0 = fully open
    double getPosition() const { return m_position; }
    
    void updateAnimation(double deltaTime) override;

protected:
    void paintComponent(QPainter *painter) override;

private:
    int m_valveId;
    ValveType m_type;
    bool m_open;
    double m_position;
    double m_targetPosition;
};

#endif // VALVE_H
