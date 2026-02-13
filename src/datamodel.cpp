#include "datamodel.h"
#include <QtMath>

DataModel::DataModel(QObject *parent)
    : QObject(parent)
    , m_systemRunning(false)
    , m_supplyTemp(25.0)
    , m_returnTemp(30.0)
    , m_systemPressure(2.5)
    , m_returnPressure(2.0)
    , m_flowRate(0.0)
    , m_tankLevel(75.0)
    , m_heaterPower(0.0)
    , m_coolingCapacity(30)
    , m_simulationTime(0.0)
{
    // Initialize 4 channels
    m_channelStates.resize(4);
    m_channelFlowRates.resize(4);
    for (int i = 0; i < 4; ++i) {
        m_channelStates[i] = false;
        m_channelFlowRates[i] = 0.0;
    }
    
    // Initialize 2 coolant pumps
    m_pumpStates.resize(2);
    for (int i = 0; i < 2; ++i) {
        m_pumpStates[i] = false;
    }
    
    // Initialize 3 solenoid valves
    m_solenoidValves.resize(3);
    for (int i = 0; i < 3; ++i) {
        m_solenoidValves[i] = false;
    }
    
    // Initialize 3 compressors
    m_compressorStates.resize(3);
    for (int i = 0; i < 3; ++i) {
        m_compressorStates[i] = false;
    }
    
    // Initialize 3 blowers
    m_blowerStates.resize(3);
    for (int i = 0; i < 3; ++i) {
        m_blowerStates[i] = false;
    }
    
    // Initialize 3 condensers
    m_condenserTemps.resize(3);
    for (int i = 0; i < 3; ++i) {
        m_condenserTemps[i] = 35.0;
    }
    
    // Initialize 3 PHEs
    m_pheTemps.resize(3);
    for (int i = 0; i < 3; ++i) {
        m_pheTemps[i] = 28.0;
    }
}

void DataModel::setSystemRunning(bool running)
{
    if (m_systemRunning != running) {
        m_systemRunning = running;
        
        if (running) {
            // Start system - turn on pumps and open some channels
            setPumpState(0, true);
            setChannelState(0, true);
            setChannelState(1, true);
            setCompressorState(0, true);
            setBlowerState(0, true);
            setSolenoidValveState(0, true);
        } else {
            // Stop system
            for (int i = 0; i < m_pumpStates.size(); ++i) {
                setPumpState(i, false);
            }
            for (int i = 0; i < m_channelStates.size(); ++i) {
                setChannelState(i, false);
            }
            for (int i = 0; i < m_compressorStates.size(); ++i) {
                setCompressorState(i, false);
            }
            for (int i = 0; i < m_blowerStates.size(); ++i) {
                setBlowerState(i, false);
            }
        }
        
        emit systemStateChanged(running);
        emit dataChanged();
    }
}

void DataModel::setSupplyTemp(double temp)
{
    m_supplyTemp = temp;
    emit dataChanged();
}

void DataModel::setReturnTemp(double temp)
{
    m_returnTemp = temp;
    emit dataChanged();
}

void DataModel::setSystemPressure(double pressure)
{
    m_systemPressure = pressure;
    emit dataChanged();
}

void DataModel::setReturnPressure(double pressure)
{
    m_returnPressure = pressure;
    emit dataChanged();
}

void DataModel::setFlowRate(double rate)
{
    m_flowRate = rate;
    emit dataChanged();
}

void DataModel::setTankLevel(double level)
{
    m_tankLevel = level;
    emit dataChanged();
}

void DataModel::setHeaterPower(double power)
{
    m_heaterPower = power;
    emit dataChanged();
}

bool DataModel::getChannelState(int channel) const
{
    if (channel >= 0 && channel < m_channelStates.size()) {
        return m_channelStates[channel];
    }
    return false;
}

void DataModel::setChannelState(int channel, bool open)
{
    if (channel >= 0 && channel < m_channelStates.size()) {
        m_channelStates[channel] = open;
        emit dataChanged();
    }
}

double DataModel::getChannelFlowRate(int channel) const
{
    if (channel >= 0 && channel < m_channelFlowRates.size()) {
        return m_channelFlowRates[channel];
    }
    return 0.0;
}

void DataModel::setChannelFlowRate(int channel, double rate)
{
    if (channel >= 0 && channel < m_channelFlowRates.size()) {
        m_channelFlowRates[channel] = rate;
        emit dataChanged();
    }
}

bool DataModel::getPumpState(int pump) const
{
    if (pump >= 0 && pump < m_pumpStates.size()) {
        return m_pumpStates[pump];
    }
    return false;
}

void DataModel::setPumpState(int pump, bool running)
{
    if (pump >= 0 && pump < m_pumpStates.size()) {
        m_pumpStates[pump] = running;
        emit dataChanged();
    }
}

bool DataModel::getSolenoidValveState(int valve) const
{
    if (valve >= 0 && valve < m_solenoidValves.size()) {
        return m_solenoidValves[valve];
    }
    return false;
}

void DataModel::setSolenoidValveState(int valve, bool open)
{
    if (valve >= 0 && valve < m_solenoidValves.size()) {
        m_solenoidValves[valve] = open;
        emit dataChanged();
    }
}

bool DataModel::getCompressorState(int compressor) const
{
    if (compressor >= 0 && compressor < m_compressorStates.size()) {
        return m_compressorStates[compressor];
    }
    return false;
}

void DataModel::setCompressorState(int compressor, bool running)
{
    if (compressor >= 0 && compressor < m_compressorStates.size()) {
        m_compressorStates[compressor] = running;
        emit dataChanged();
    }
}

bool DataModel::getBlowerState(int blower) const
{
    if (blower >= 0 && blower < m_blowerStates.size()) {
        return m_blowerStates[blower];
    }
    return false;
}

void DataModel::setBlowerState(int blower, bool running)
{
    if (blower >= 0 && blower < m_blowerStates.size()) {
        m_blowerStates[blower] = running;
        emit dataChanged();
    }
}

double DataModel::getCondenserTemp(int condenser) const
{
    if (condenser >= 0 && condenser < m_condenserTemps.size()) {
        return m_condenserTemps[condenser];
    }
    return 0.0;
}

void DataModel::setCondenserTemp(int condenser, double temp)
{
    if (condenser >= 0 && condenser < m_condenserTemps.size()) {
        m_condenserTemps[condenser] = temp;
        emit dataChanged();
    }
}

double DataModel::getPHETemp(int phe) const
{
    if (phe >= 0 && phe < m_pheTemps.size()) {
        return m_pheTemps[phe];
    }
    return 0.0;
}

void DataModel::setPHETemp(int phe, double temp)
{
    if (phe >= 0 && phe < m_pheTemps.size()) {
        m_pheTemps[phe] = temp;
        emit dataChanged();
    }
}

void DataModel::setCoolingCapacity(int capacity)
{
    m_coolingCapacity = capacity;
    emit dataChanged();
}

void DataModel::resetAllTrips()
{
    // Reset any alarm or trip conditions
    emit dataChanged();
}

void DataModel::updateSimulation(double deltaTime)
{
    if (!m_systemRunning) {
        return;
    }
    
    m_simulationTime += deltaTime;
    
    simulateCoolantSystem(deltaTime);
    simulateRefrigerantSystem(deltaTime);
    simulateChannels(deltaTime);
    
    emit dataChanged();
}

void DataModel::simulateCoolantSystem(double deltaTime)
{
    // Simulate coolant flow based on pump states
    double totalFlow = 0.0;
    for (int i = 0; i < m_pumpStates.size(); ++i) {
        if (m_pumpStates[i]) {
            totalFlow += 50.0; // 50 lpm per pump
        }
    }
    m_flowRate = totalFlow;
    
    // Simulate temperature changes with some oscillation
    if (totalFlow > 0) {
        m_supplyTemp = 20.0 + 3.0 * qSin(m_simulationTime * 0.5);
        m_returnTemp = m_supplyTemp + 5.0 + 2.0 * qSin(m_simulationTime * 0.3);
    } else {
        m_supplyTemp = 25.0;
        m_returnTemp = 25.0;
    }
    
    // Simulate pressure
    m_systemPressure = totalFlow > 0 ? 2.5 + 0.3 * qSin(m_simulationTime * 0.7) : 0.0;
    m_returnPressure = m_systemPressure * 0.8;
    
    // Heater power
    m_heaterPower = m_systemRunning ? m_coolingCapacity * 0.5 : 0.0;
}

void DataModel::simulateRefrigerantSystem(double deltaTime)
{
    // Simulate condenser temperatures based on compressor states
    for (int i = 0; i < m_compressorStates.size(); ++i) {
        if (m_compressorStates[i]) {
            // Operating temperature
            m_condenserTemps[i] = 35.0 + 5.0 * qSin(m_simulationTime * 0.4 + i);
            m_pheTemps[i] = 15.0 + 3.0 * qSin(m_simulationTime * 0.6 + i);
        } else {
            // Ambient temperature
            m_condenserTemps[i] = 25.0;
            m_pheTemps[i] = 25.0;
        }
    }
}

void DataModel::simulateChannels(double deltaTime)
{
    // Simulate channel flow rates
    double baseFlow = m_flowRate / 4.0; // Distribute flow among open channels
    int openChannels = 0;
    
    for (int i = 0; i < m_channelStates.size(); ++i) {
        if (m_channelStates[i]) {
            openChannels++;
        }
    }
    
    if (openChannels > 0) {
        double flowPerChannel = m_flowRate / openChannels;
        for (int i = 0; i < m_channelStates.size(); ++i) {
            if (m_channelStates[i]) {
                m_channelFlowRates[i] = flowPerChannel * (0.9 + 0.1 * qSin(m_simulationTime + i));
            } else {
                m_channelFlowRates[i] = 0.0;
            }
        }
    } else {
        for (int i = 0; i < m_channelFlowRates.size(); ++i) {
            m_channelFlowRates[i] = 0.0;
        }
    }
}
