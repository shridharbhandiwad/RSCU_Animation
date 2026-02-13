#ifndef LCUSCENE_H
#define LCUSCENE_H

#include <QGraphicsScene>
#include <QVector>

class DataModel;
class BaseComponent;
class Pump;
class Valve;
class Tank;
class Heater;
class HeatExchanger;
class Condenser;
class Blower;
class Pipe;
class SolenoidValve;

class LCUScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit LCUScene(DataModel *dataModel, QObject *parent = nullptr);
    
    void updateAnimations(double deltaTime);

private:
    void setupScene();
    void createCoolantSystem();
    void createRefrigerantSystem();
    void createChannelSystem();
    void createPipingConnections();
    void createLabels();
    
    DataModel *m_dataModel;
    
    // Coolant system components
    Tank *m_tank;
    Heater *m_heater;
    QVector<Pump*> m_coolantPumps;
    QVector<Pipe*> m_coolantPipes;
    
    // Channel system
    QVector<Valve*> m_channelValves;
    QVector<Pipe*> m_channelPipes;
    
    // Refrigerant system
    QVector<HeatExchanger*> m_heatExchangers;
    QVector<SolenoidValve*> m_solenoidValves;
    QVector<Condenser*> m_condensers;
    QVector<Blower*> m_blowers;
    QVector<Pipe*> m_refrigerantPipes;
    
    // All components for easy iteration
    QVector<BaseComponent*> m_allComponents;
};

#endif // LCUSCENE_H
