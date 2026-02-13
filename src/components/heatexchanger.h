#ifndef HEATEXCHANGER_H
#define HEATEXCHANGER_H

#include "basecomponent.h"

class HeatExchanger : public BaseComponent
{
    Q_OBJECT

public:
    explicit HeatExchanger(int id, QObject *parent = nullptr);
    
    void setActive(bool active);
    bool isActive() const { return m_active; }
    
    void setHotSideTemp(double temp);
    void setColdSideTemp(double temp);
    
    double getHotSideTemp() const { return m_hotSideTemp; }
    double getColdSideTemp() const { return m_coldSideTemp; }
    
    void updateAnimation(double deltaTime) override;

protected:
    void paintComponent(QPainter *painter) override;

private:
    int m_id;
    bool m_active;
    double m_hotSideTemp;
    double m_coldSideTemp;
};

#endif // HEATEXCHANGER_H
