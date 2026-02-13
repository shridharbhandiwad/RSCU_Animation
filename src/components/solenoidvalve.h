#ifndef SOLENOIDVALVE_H
#define SOLENOIDVALVE_H

#include "basecomponent.h"

class SolenoidValve : public BaseComponent
{
    Q_OBJECT

public:
    explicit SolenoidValve(int id, QObject *parent = nullptr);
    
    void setOpen(bool open);
    bool isOpen() const { return m_open; }
    
    void setEnergized(bool energized);
    bool isEnergized() const { return m_energized; }
    
    void updateAnimation(double deltaTime) override;

protected:
    void paintComponent(QPainter *painter) override;

private:
    int m_id;
    bool m_open;
    bool m_energized;
    double m_pulsePhase;
};

#endif // SOLENOIDVALVE_H
