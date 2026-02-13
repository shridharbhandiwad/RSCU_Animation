# Integration Guide - External Data Sources

This guide explains how to integrate external data sources with the Liquid Cooling Unit Simulator.

## Architecture Overview

```
External Data Source → Data Adapter → DataModel → LCUScene → Display
```

The `DataModel` class is the central hub for all system parameters. External systems can update the model, and the UI will automatically reflect the changes.

## Method 1: Direct API Calls

The simplest integration method is calling the DataModel API directly.

### Example: Serial Port Integration

```cpp
#include "datamodel.h"
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialDataAdapter : public QObject
{
    Q_OBJECT
    
public:
    SerialDataAdapter(DataModel *model, QObject *parent = nullptr)
        : QObject(parent), m_model(model)
    {
        m_serialPort = new QSerialPort(this);
        connect(m_serialPort, &QSerialPort::readyRead, 
                this, &SerialDataAdapter::onDataReceived);
    }
    
    bool connectToPort(const QString &portName, int baudRate = 9600)
    {
        m_serialPort->setPortName(portName);
        m_serialPort->setBaudRate(baudRate);
        m_serialPort->setDataBits(QSerialPort::Data8);
        m_serialPort->setParity(QSerialPort::NoParity);
        m_serialPort->setStopBits(QSerialPort::OneStop);
        
        if (m_serialPort->open(QIODevice::ReadWrite)) {
            qDebug() << "Connected to" << portName;
            return true;
        }
        return false;
    }
    
private slots:
    void onDataReceived()
    {
        QByteArray data = m_serialPort->readAll();
        parseAndUpdate(data);
    }
    
private:
    void parseAndUpdate(const QByteArray &data)
    {
        // Example: Parse CSV format data
        // "TEMP:22.5,PRESS:2.5,FLOW:100.0,PUMP0:1,CH0:1"
        
        QString str = QString::fromUtf8(data).trimmed();
        QStringList params = str.split(',');
        
        for (const QString &param : params) {
            QStringList parts = param.split(':');
            if (parts.size() == 2) {
                QString key = parts[0];
                QString value = parts[1];
                
                if (key == "TEMP") {
                    m_model->setSupplyTemp(value.toDouble());
                } else if (key == "PRESS") {
                    m_model->setSystemPressure(value.toDouble());
                } else if (key == "FLOW") {
                    m_model->setFlowRate(value.toDouble());
                } else if (key.startsWith("PUMP")) {
                    int pumpId = key.mid(4).toInt();
                    m_model->setPumpState(pumpId, value.toInt() == 1);
                } else if (key.startsWith("CH")) {
                    int channelId = key.mid(2).toInt();
                    m_model->setChannelState(channelId, value.toInt() == 1);
                }
            }
        }
    }
    
    QSerialPort *m_serialPort;
    DataModel *m_model;
};

// Usage in main.cpp:
// SerialDataAdapter *adapter = new SerialDataAdapter(dataModel);
// adapter->connectToPort("COM3", 9600);
```

## Method 2: TCP/IP Network Integration

For remote monitoring systems:

```cpp
#include "datamodel.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

class NetworkDataAdapter : public QObject
{
    Q_OBJECT
    
public:
    NetworkDataAdapter(DataModel *model, QObject *parent = nullptr)
        : QObject(parent), m_model(model)
    {
        m_socket = new QTcpSocket(this);
        connect(m_socket, &QTcpSocket::connected, 
                this, &NetworkDataAdapter::onConnected);
        connect(m_socket, &QTcpSocket::readyRead, 
                this, &NetworkDataAdapter::onDataReceived);
        connect(m_socket, &QTcpSocket::disconnected, 
                this, &NetworkDataAdapter::onDisconnected);
    }
    
    void connectToServer(const QString &host, quint16 port)
    {
        qDebug() << "Connecting to" << host << ":" << port;
        m_socket->connectToHost(host, port);
    }
    
private slots:
    void onConnected()
    {
        qDebug() << "Connected to server";
    }
    
    void onDisconnected()
    {
        qDebug() << "Disconnected from server";
    }
    
    void onDataReceived()
    {
        QByteArray data = m_socket->readAll();
        parseJsonData(data);
    }
    
private:
    void parseJsonData(const QByteArray &data)
    {
        // Example JSON format:
        // {
        //   "coolant": {"supply_temp": 22.5, "pressure": 2.5, "flow": 100.0},
        //   "pumps": [{"id": 0, "running": true}],
        //   "channels": [{"id": 0, "open": true, "flow": 50.0}],
        //   "refrigerant": [{"id": 0, "compressor": true, "blower": true}]
        // }
        
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isObject()) {
            return;
        }
        
        QJsonObject root = doc.object();
        
        // Parse coolant data
        if (root.contains("coolant")) {
            QJsonObject coolant = root["coolant"].toObject();
            if (coolant.contains("supply_temp")) {
                m_model->setSupplyTemp(coolant["supply_temp"].toDouble());
            }
            if (coolant.contains("return_temp")) {
                m_model->setReturnTemp(coolant["return_temp"].toDouble());
            }
            if (coolant.contains("pressure")) {
                m_model->setSystemPressure(coolant["pressure"].toDouble());
            }
            if (coolant.contains("flow")) {
                m_model->setFlowRate(coolant["flow"].toDouble());
            }
        }
        
        // Parse pumps
        if (root.contains("pumps")) {
            QJsonArray pumps = root["pumps"].toArray();
            for (const QJsonValue &val : pumps) {
                QJsonObject pump = val.toObject();
                int id = pump["id"].toInt();
                bool running = pump["running"].toBool();
                m_model->setPumpState(id, running);
            }
        }
        
        // Parse channels
        if (root.contains("channels")) {
            QJsonArray channels = root["channels"].toArray();
            for (const QJsonValue &val : channels) {
                QJsonObject channel = val.toObject();
                int id = channel["id"].toInt();
                bool open = channel["open"].toBool();
                double flow = channel["flow"].toDouble();
                m_model->setChannelState(id, open);
                m_model->setChannelFlowRate(id, flow);
            }
        }
        
        // Parse refrigerant loops
        if (root.contains("refrigerant")) {
            QJsonArray loops = root["refrigerant"].toArray();
            for (const QJsonValue &val : loops) {
                QJsonObject loop = val.toObject();
                int id = loop["id"].toInt();
                bool compressor = loop["compressor"].toBool();
                bool blower = loop["blower"].toBool();
                m_model->setCompressorState(id, compressor);
                m_model->setBlowerState(id, blower);
            }
        }
    }
    
    QTcpSocket *m_socket;
    DataModel *m_model;
};

// Usage:
// NetworkDataAdapter *adapter = new NetworkDataAdapter(dataModel);
// adapter->connectToServer("192.168.1.100", 8080);
```

## Method 3: Shared Memory Integration

For inter-process communication on Windows:

```cpp
#include "datamodel.h"
#include <QSharedMemory>
#include <QTimer>

struct LCUSharedData {
    bool systemRunning;
    double supplyTemp;
    double returnTemp;
    double systemPressure;
    double flowRate;
    bool pumpStates[2];
    bool channelStates[4];
    double channelFlowRates[4];
    bool compressorStates[3];
    bool blowerStates[3];
};

class SharedMemoryAdapter : public QObject
{
    Q_OBJECT
    
public:
    SharedMemoryAdapter(DataModel *model, QObject *parent = nullptr)
        : QObject(parent), m_model(model)
    {
        m_sharedMemory = new QSharedMemory("LCU_Data", this);
        
        if (!m_sharedMemory->attach()) {
            if (!m_sharedMemory->create(sizeof(LCUSharedData))) {
                qWarning() << "Failed to create shared memory";
                return;
            }
        }
        
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &SharedMemoryAdapter::readSharedData);
        m_timer->start(100); // Read every 100ms
    }
    
private slots:
    void readSharedData()
    {
        if (!m_sharedMemory->lock()) {
            return;
        }
        
        const LCUSharedData *data = static_cast<const LCUSharedData*>(m_sharedMemory->constData());
        
        // Update data model
        m_model->setSystemRunning(data->systemRunning);
        m_model->setSupplyTemp(data->supplyTemp);
        m_model->setReturnTemp(data->returnTemp);
        m_model->setSystemPressure(data->systemPressure);
        m_model->setFlowRate(data->flowRate);
        
        for (int i = 0; i < 2; ++i) {
            m_model->setPumpState(i, data->pumpStates[i]);
        }
        
        for (int i = 0; i < 4; ++i) {
            m_model->setChannelState(i, data->channelStates[i]);
            m_model->setChannelFlowRate(i, data->channelFlowRates[i]);
        }
        
        for (int i = 0; i < 3; ++i) {
            m_model->setCompressorState(i, data->compressorStates[i]);
            m_model->setBlowerState(i, data->blowerStates[i]);
        }
        
        m_sharedMemory->unlock();
    }
    
private:
    QSharedMemory *m_sharedMemory;
    QTimer *m_timer;
    DataModel *m_model;
};

// Usage:
// SharedMemoryAdapter *adapter = new SharedMemoryAdapter(dataModel);
```

## Method 4: File-Based Integration

For systems that output data to files:

```cpp
#include "datamodel.h"
#include <QFileSystemWatcher>
#include <QFile>
#include <QTextStream>

class FileDataAdapter : public QObject
{
    Q_OBJECT
    
public:
    FileDataAdapter(DataModel *model, const QString &filePath, QObject *parent = nullptr)
        : QObject(parent), m_model(model), m_filePath(filePath)
    {
        m_watcher = new QFileSystemWatcher(this);
        m_watcher->addPath(filePath);
        connect(m_watcher, &QFileSystemWatcher::fileChanged, 
                this, &FileDataAdapter::onFileChanged);
        
        // Read initial data
        readFile();
    }
    
private slots:
    void onFileChanged(const QString &path)
    {
        Q_UNUSED(path);
        readFile();
    }
    
private:
    void readFile()
    {
        QFile file(m_filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }
        
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split('=');
            if (parts.size() == 2) {
                QString key = parts[0].trimmed();
                QString value = parts[1].trimmed();
                
                if (key == "supply_temp") {
                    m_model->setSupplyTemp(value.toDouble());
                } else if (key == "return_temp") {
                    m_model->setReturnTemp(value.toDouble());
                } else if (key == "pressure") {
                    m_model->setSystemPressure(value.toDouble());
                } else if (key == "flow_rate") {
                    m_model->setFlowRate(value.toDouble());
                }
                // Add more parameters as needed
            }
        }
        
        file.close();
    }
    
    QFileSystemWatcher *m_watcher;
    DataModel *m_model;
    QString m_filePath;
};

// Usage:
// FileDataAdapter *adapter = new FileDataAdapter(dataModel, "C:/data/lcu_data.txt");
```

## Complete Integration Example

Here's a complete example showing how to modify `main.cpp` to add external data support:

```cpp
#include <QApplication>
#include "mainwindow.h"
#include "datamodel.h"

// Choose your integration method and include the appropriate adapter
// #include "serialdataadapter.h"
// #include "networkdataadapter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Liquid Cooling Unit Simulator");
    app.setApplicationVersion("1.0.0");
    
    MainWindow window;
    
    // Option 1: Use built-in simulation (default)
    // No additional code needed
    
    // Option 2: Add serial port integration
    // SerialDataAdapter *serialAdapter = new SerialDataAdapter(window.dataModel());
    // serialAdapter->connectToPort("COM3", 9600);
    
    // Option 3: Add network integration
    // NetworkDataAdapter *networkAdapter = new NetworkDataAdapter(window.dataModel());
    // networkAdapter->connectToServer("192.168.1.100", 8080);
    
    // Option 4: Add file watching
    // FileDataAdapter *fileAdapter = new FileDataAdapter(window.dataModel(), "C:/data/lcu_data.txt");
    
    window.show();
    
    return app.exec();
}
```

## Data Format Specifications

### CSV Format (Serial/File)
```
TEMP:22.5,PRESS:2.5,FLOW:100.0,PUMP0:1,PUMP1:0,CH0:1,CH1:1,CH2:0,CH3:0
```

### JSON Format (Network)
```json
{
  "timestamp": "2026-02-13T12:00:00",
  "system_running": true,
  "coolant": {
    "supply_temp_c": 22.5,
    "return_temp_c": 28.3,
    "system_pressure_bar": 2.5,
    "return_pressure_bar": 2.0,
    "flow_rate_lpm": 100.0,
    "tank_level_percent": 75.0,
    "heater_power_kw": 15.0
  },
  "pumps": [
    {"id": 0, "running": true},
    {"id": 1, "running": false}
  ],
  "channels": [
    {"id": 0, "open": true, "flow_rate_lpm": 50.0},
    {"id": 1, "open": true, "flow_rate_lpm": 50.0},
    {"id": 2, "open": false, "flow_rate_lpm": 0.0},
    {"id": 3, "open": false, "flow_rate_lpm": 0.0}
  ],
  "refrigerant_loops": [
    {
      "id": 0,
      "compressor_running": true,
      "solenoid_valve_open": true,
      "blower_running": true,
      "blower_speed_percent": 75.0,
      "condenser_temp_c": 38.5,
      "phe_temp_c": 18.2
    }
  ]
}
```

### INI Format (File)
```ini
[System]
running=true
cooling_capacity_kw=30

[Coolant]
supply_temp=22.5
return_temp=28.3
system_pressure=2.5
flow_rate=100.0
tank_level=75.0

[Pump0]
running=true

[Pump1]
running=false

[Channel0]
open=true
flow_rate=50.0

[Refrigerant0]
compressor=true
blower=true
condenser_temp=38.5
```

## Adding to CMakeLists.txt

If you add adapter files, update CMakeLists.txt:

```cmake
# Add to SOURCES
set(SOURCES
    ...
    src/serialdataadapter.cpp
    src/networkdataadapter.cpp
)

# Add to HEADERS
set(HEADERS
    ...
    src/serialdataadapter.h
    src/networkdataadapter.h
)

# Add Qt modules if needed
find_package(Qt6 REQUIRED COMPONENTS 
    Core Gui Widgets 
    SerialPort  # For serial communication
    Network     # For TCP/IP communication
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::SerialPort  # If using serial
    Qt6::Network     # If using network
)
```

## Disabling Built-in Simulation

To disable the built-in simulation when using external data:

In `datamodel.cpp`, comment out the simulation update:

```cpp
void DataModel::updateSimulation(double deltaTime)
{
    if (!m_systemRunning) {
        return;
    }
    
    m_simulationTime += deltaTime;
    
    // Comment out these lines when using external data:
    // simulateCoolantSystem(deltaTime);
    // simulateRefrigerantSystem(deltaTime);
    // simulateChannels(deltaTime);
    
    emit dataChanged();
}
```

## Troubleshooting

**Data not updating:**
- Verify data source is sending correctly
- Check connection status
- Add debug output in adapter's parse function

**Animation lag:**
- Reduce update frequency in adapter
- Use separate thread for data acquisition
- Buffer data and batch updates

**Incorrect values:**
- Verify data format matches parser
- Check units (°C vs °F, Bar vs PSI, etc.)
- Add validation and range checks

## Support

For integration assistance, refer to:
- Qt documentation: https://doc.qt.io
- Project source code comments
- test_data.json for example data formats
