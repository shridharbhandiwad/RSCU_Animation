#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QVector>

class DataModel : public QObject
{
    Q_OBJECT

public:
    explicit DataModel(QObject *parent = nullptr);
    
    // System state
    bool isSystemRunning() const { return m_systemRunning; }
    void setSystemRunning(bool running);
    
    // Coolant system parameters
    double getSupplyTemp() const { return m_supplyTemp; }
    void setSupplyTemp(double temp);
    
    double getReturnTemp() const { return m_returnTemp; }
    void setReturnTemp(double temp);
    
    double getSystemPressure() const { return m_systemPressure; }
    void setSystemPressure(double pressure);
    
    double getReturnPressure() const { return m_returnPressure; }
    void setReturnPressure(double pressure);
    
    double getFlowRate() const { return m_flowRate; }
    void setFlowRate(double rate);
    
    double getTankLevel() const { return m_tankLevel; }
    void setTankLevel(double level);
    
    double getHeaterPower() const { return m_heaterPower; }
    void setHeaterPower(double power);
    
    // Channel states (4 channels)
    bool getChannelState(int channel) const;
    void setChannelState(int channel, bool open);
    
    double getChannelFlowRate(int channel) const;
    void setChannelFlowRate(int channel, double rate);
    
    // Pump states (2 coolant pumps)
    bool getPumpState(int pump) const;
    void setPumpState(int pump, bool running);
    
    // Refrigerant system
    bool getSolenoidValveState(int valve) const;
    void setSolenoidValveState(int valve, bool open);
    
    bool getCompressorState(int compressor) const;
    void setCompressorState(int compressor, bool running);
    
    bool getBlowerState(int blower) const;
    void setBlowerState(int blower, bool running);
    
    double getCondenserTemp(int condenser) const;
    void setCondenserTemp(int condenser, double temp);
    
    double getPHETemp(int phe) const;
    void setPHETemp(int phe, double temp);
    
    // Cooling capacity
    int getCoolingCapacity() const { return m_coolingCapacity; }
    void setCoolingCapacity(int capacity);
    
    // Trips and alarms
    void resetAllTrips();
    
    // Simulation update
    void updateSimulation(double deltaTime);
    
signals:
    void dataChanged();
    void systemStateChanged(bool running);

private:
    void simulateCoolantSystem(double deltaTime);
    void simulateRefrigerantSystem(double deltaTime);
    void simulateChannels(double deltaTime);
    
    // System state
    bool m_systemRunning;
    
    // Coolant system
    double m_supplyTemp;
    double m_returnTemp;
    double m_systemPressure;
    double m_returnPressure;
    double m_flowRate;
    double m_tankLevel;
    double m_heaterPower;
    
    // Channels
    QVector<bool> m_channelStates;
    QVector<double> m_channelFlowRates;
    
    // Pumps
    QVector<bool> m_pumpStates;
    
    // Refrigerant system
    QVector<bool> m_solenoidValves;
    QVector<bool> m_compressorStates;
    QVector<bool> m_blowerStates;
    QVector<double> m_condenserTemps;
    QVector<double> m_pheTemps;
    
    // System parameters
    int m_coolingCapacity;
    
    // Simulation time
    double m_simulationTime;
};

#endif // DATAMODEL_H
