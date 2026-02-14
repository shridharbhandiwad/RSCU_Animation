#include "lcuscene3d.h"
#include "datamodel.h"
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QCamera>
#include <QtMath>

LCUScene3D::LCUScene3D(DataModel *dataModel, Qt3DCore::QEntity *parent)
    : Qt3DCore::QEntity(parent)
    , m_dataModel(dataModel)
    , m_pumpRotation(0.0)
    , m_blowerRotation(0.0)
{
    setupScene();
}

void LCUScene3D::setupCamera(Qt3DRender::QCamera *camera)
{
    // Position camera for a good view of the system
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 20, 40));
    camera->setUpVector(QVector3D(0, 1, 0));
    camera->setViewCenter(QVector3D(0, 0, 0));
}

void LCUScene3D::setupScene()
{
    setupLighting();
    createFloor();
    createCoolantSystem();
    createChannelSystem();
    createRefrigerantSystem();
    createPipingConnections();
}

void LCUScene3D::setupLighting()
{
    // Main light
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(this);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1.0f);
    lightEntity->addComponent(light);
    
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(0, 30, 20));
    lightEntity->addComponent(lightTransform);
    
    // Additional fill light
    Qt3DCore::QEntity *fillLightEntity = new Qt3DCore::QEntity(this);
    Qt3DRender::QPointLight *fillLight = new Qt3DRender::QPointLight(fillLightEntity);
    fillLight->setColor("white");
    fillLight->setIntensity(0.5f);
    fillLightEntity->addComponent(fillLight);
    
    Qt3DCore::QTransform *fillLightTransform = new Qt3DCore::QTransform(fillLightEntity);
    fillLightTransform->setTranslation(QVector3D(-20, 15, -10));
    fillLightEntity->addComponent(fillLightTransform);
}

void LCUScene3D::createFloor()
{
    // Create a floor plane
    Qt3DCore::QEntity *floorEntity = createBox(
        QVector3D(0, -1, 0),
        QVector3D(60, 0.5, 40),
        QColor(180, 180, 180),
        this
    );
}

void LCUScene3D::createCoolantSystem()
{
    // Create coolant tank (vertical cylinder)
    m_tankEntity = createCylinder(
        QVector3D(-15, 5, 0),
        3.0f,
        10.0f,
        QColor(100, 150, 200),
        this
    );
    
    // Rotate tank to be vertical (cylinder is horizontal by default)
    Qt3DCore::QTransform *tankTransform = m_tankEntity->findChild<Qt3DCore::QTransform*>();
    if (tankTransform) {
        tankTransform->setRotationX(90);
    }
    
    // Store tank material for state updates
    m_tankMaterial = m_tankEntity->findChild<Qt3DExtras::QPhongMaterial*>();
    
    // Create heater (box below tank)
    m_heaterEntity = createBox(
        QVector3D(-15, 0.5, 0),
        QVector3D(4, 1, 4),
        QColor(200, 50, 50),
        this
    );
    
    // Store heater material for state updates
    m_heaterMaterial = m_heaterEntity->findChild<Qt3DExtras::QPhongMaterial*>();
    
    // Create two pumps
    for (int i = 0; i < 2; ++i) {
        float xPos = -15 + (i * 6);
        Qt3DCore::QEntity *pumpEntity = createCylinder(
            QVector3D(xPos, 1.5, -6),
            1.5f,
            3.0f,
            QColor(80, 120, 160),
            this
        );
        
        // Add pump impeller (sphere in center)
        Qt3DCore::QEntity *impeller = createSphere(
            QVector3D(0, 0, 0),
            1.0f,
            QColor(150, 150, 150),
            pumpEntity
        );
        
        m_pumpEntities.append(pumpEntity);
        
        // Store transform for animation
        Qt3DCore::QTransform *pumpTransform = pumpEntity->findChild<Qt3DCore::QTransform*>();
        m_pumpTransforms.append(pumpTransform);
        
        // Store material for state updates
        Qt3DExtras::QPhongMaterial *pumpMaterial = pumpEntity->findChild<Qt3DExtras::QPhongMaterial*>();
        m_pumpMaterials.append(pumpMaterial);
    }
}

void LCUScene3D::createChannelSystem()
{
    // Create 12 valves (3 per channel, matching 2D layout)
    double startX = -10;
    double startZ = 10;
    double channelSpacing = 5;
    double valveSpacing = 3;
    
    for (int ch = 0; ch < 4; ++ch) {
        float zPos = startZ + (ch * channelSpacing);
        
        // Create 3 valves per channel
        for (int v = 0; v < 3; ++v) {
            float xPos = startX + (v * valveSpacing);
            
            // Create valve body (cylinder)
            Qt3DCore::QEntity *valveEntity = createCylinder(
                QVector3D(xPos, 2, zPos),
                0.6f,
                1.5f,
                QColor(120, 120, 120),
                this
            );
            
            // Add valve handle (sphere on top)
            Qt3DCore::QEntity *handle = createSphere(
                QVector3D(0, 1.2, 0),
                0.4f,
                QColor(200, 100, 50),
                valveEntity
            );
            
            m_valveEntities.append(valveEntity);
            
            Qt3DCore::QTransform *valveTransform = valveEntity->findChild<Qt3DCore::QTransform*>();
            m_valveTransforms.append(valveTransform);
            
            // Store material reference for state updates
            Qt3DExtras::QPhongMaterial *material = valveEntity->findChild<Qt3DExtras::QPhongMaterial*>();
            m_valveMaterials.append(material);
        }
    }
}

void LCUScene3D::createRefrigerantSystem()
{
    // Create 3 refrigerant loops
    for (int i = 0; i < 3; ++i) {
        float zPos = -5 + (i * 6);
        
        // Heat Exchanger (flat box)
        Qt3DCore::QEntity *heatExchanger = createBox(
            QVector3D(5, 3, zPos),
            QVector3D(3, 4, 2),
            QColor(180, 180, 180),
            this
        );
        m_heatExchangerEntities.append(heatExchanger);
        
        // Store material for state updates
        Qt3DExtras::QPhongMaterial *heMaterial = heatExchanger->findChild<Qt3DExtras::QPhongMaterial*>();
        m_heatExchangerMaterials.append(heMaterial);
        
        // Solenoid Valve (small cylinder)
        Qt3DCore::QEntity *solenoidValve = createCylinder(
            QVector3D(10, 3, zPos),
            0.5f,
            1.5f,
            QColor(100, 100, 150),
            this
        );
        m_solenoidValveEntities.append(solenoidValve);
        
        // Store material for energized state visualization
        Qt3DExtras::QPhongMaterial *svMaterial = solenoidValve->findChild<Qt3DExtras::QPhongMaterial*>();
        m_solenoidValveMaterials.append(svMaterial);
        
        // Condenser (larger box with fins)
        Qt3DCore::QEntity *condenser = createBox(
            QVector3D(15, 3, zPos),
            QVector3D(4, 5, 3),
            QColor(160, 160, 160),
            this
        );
        
        // Add condenser fins (small boxes)
        for (int j = 0; j < 5; ++j) {
            createBox(
                QVector3D(-2 + j * 1, 0, 0),
                QVector3D(0.1, 5, 3.5),
                QColor(140, 140, 140),
                condenser
            );
        }
        m_condenserEntities.append(condenser);
        
        // Store material for state updates
        Qt3DExtras::QPhongMaterial *condenserMaterial = condenser->findChild<Qt3DExtras::QPhongMaterial*>();
        m_condenserMaterials.append(condenserMaterial);
        
        // Blower (cylinder with cone for fan)
        Qt3DCore::QEntity *blowerEntity = new Qt3DCore::QEntity(this);
        
        // Blower housing
        Qt3DCore::QEntity *housing = createCylinder(
            QVector3D(20, 3, zPos),
            1.5f,
            2.0f,
            QColor(80, 80, 120),
            blowerEntity
        );
        
        // Fan blades (thin cylinders radiating from center)
        for (int b = 0; b < 6; ++b) {
            float angle = b * 60.0f;
            Qt3DCore::QEntity *blade = createBox(
                QVector3D(0, 0, 0),
                QVector3D(1.5, 0.2, 0.3),
                QColor(120, 120, 120),
                housing
            );
            
            Qt3DCore::QTransform *bladeTransform = blade->findChild<Qt3DCore::QTransform*>();
            if (bladeTransform) {
                bladeTransform->setRotationZ(angle);
            }
        }
        
        m_blowerEntities.append(blowerEntity);
        
        Qt3DCore::QTransform *blowerTransform = housing->findChild<Qt3DCore::QTransform*>();
        m_blowerTransforms.append(blowerTransform);
        
        // Store material for state updates
        Qt3DExtras::QPhongMaterial *blowerMaterial = housing->findChild<Qt3DExtras::QPhongMaterial*>();
        m_blowerMaterials.append(blowerMaterial);
    }
}

void LCUScene3D::createPipingConnections()
{
    // Main coolant pipe from tank
    createPipe(
        QVector3D(-15, 0, 0),
        QVector3D(-15, 0, -6),
        0.4f,
        QColor(100, 150, 200),
        this
    );
    
    // Pipes connecting pumps
    createPipe(
        QVector3D(-15, 1.5, -6),
        QVector3D(-9, 1.5, -6),
        0.4f,
        QColor(100, 150, 200),
        this
    );
    
    // Distribution manifold
    createPipe(
        QVector3D(-9, 1.5, -6),
        QVector3D(-10, 2, 5),
        0.4f,
        QColor(100, 150, 200),
        this
    );
    
    // Channel pipes
    for (int i = 0; i < 4; ++i) {
        float xPos = -10 + (i * 5);
        createPipe(
            QVector3D(xPos, 2, 5),
            QVector3D(xPos, 2, 10),
            0.3f,
            QColor(100, 150, 200),
            this
        );
    }
    
    // Refrigerant pipes
    for (int i = 0; i < 3; ++i) {
        float zPos = -5 + (i * 6);
        
        // PHE to solenoid
        createPipe(
            QVector3D(5, 3, zPos),
            QVector3D(10, 3, zPos),
            0.25f,
            QColor(200, 100, 100),
            this
        );
        
        // Solenoid to condenser
        createPipe(
            QVector3D(10, 3, zPos),
            QVector3D(15, 3, zPos),
            0.25f,
            QColor(200, 100, 100),
            this
        );
        
        // Condenser to blower
        createPipe(
            QVector3D(15, 3, zPos),
            QVector3D(20, 3, zPos),
            0.25f,
            QColor(200, 100, 100),
            this
        );
    }
}

void LCUScene3D::updateAnimations(double deltaTime)
{
    if (!m_dataModel) return;
    
    bool systemRunning = m_dataModel->isSystemRunning();
    
    // Update heater visual state
    if (m_heaterMaterial) {
        if (systemRunning) {
            // Active heater - brighter red/orange glow
            m_heaterMaterial->setDiffuse(QColor(255, 80, 20));
            m_heaterMaterial->setAmbient(QColor(200, 50, 10));
        } else {
            // Inactive heater - dull red
            m_heaterMaterial->setDiffuse(QColor(200, 50, 50));
            m_heaterMaterial->setAmbient(QColor(150, 40, 40));
        }
    }
    
    // Update pump rotations and states
    for (int i = 0; i < m_pumpEntities.size() && i < 2; ++i) {
        bool pumpRunning = m_dataModel->getPumpState(i) && systemRunning;
        
        // Update pump rotation animation
        if (pumpRunning && m_pumpTransforms[i]) {
            m_pumpRotation += deltaTime * 180.0; // degrees per second
            if (m_pumpRotation >= 360.0) {
                m_pumpRotation -= 360.0;
            }
            m_pumpTransforms[i]->setRotationZ(m_pumpRotation);
        }
        
        // Update pump color based on state
        if (i < m_pumpMaterials.size() && m_pumpMaterials[i]) {
            if (pumpRunning) {
                // Running pump - brighter blue
                m_pumpMaterials[i]->setDiffuse(QColor(100, 180, 240));
                m_pumpMaterials[i]->setAmbient(QColor(80, 120, 160));
            } else {
                // Idle pump - dull blue
                m_pumpMaterials[i]->setDiffuse(QColor(80, 120, 160));
                m_pumpMaterials[i]->setAmbient(QColor(60, 90, 120));
            }
        }
    }
    
    // Update channel valves (3 per channel, matching 2D scene)
    for (int ch = 0; ch < 4; ++ch) {
        bool channelOpen = m_dataModel->getChannelState(ch);
        
        for (int v = 0; v < 3; ++v) {
            int valveIndex = ch * 3 + v;
            if (valveIndex < m_valveMaterials.size() && m_valveMaterials[valveIndex]) {
                if (channelOpen && systemRunning) {
                    // Open valve - green
                    m_valveMaterials[valveIndex]->setDiffuse(QColor(50, 200, 50));
                    m_valveMaterials[valveIndex]->setAmbient(QColor(40, 150, 40));
                } else {
                    // Closed valve - gray
                    m_valveMaterials[valveIndex]->setDiffuse(QColor(120, 120, 120));
                    m_valveMaterials[valveIndex]->setAmbient(QColor(90, 90, 90));
                }
            }
        }
    }
    
    // Update refrigerant system (3 loops)
    for (int i = 0; i < 3; ++i) {
        bool compressorRunning = m_dataModel->getCompressorState(i);
        bool solenoidOpen = m_dataModel->getSolenoidValveState(i);
        bool blowerRunning = m_dataModel->getBlowerState(i);
        
        // Update heat exchanger state
        if (i < m_heatExchangerMaterials.size() && m_heatExchangerMaterials[i]) {
            if (compressorRunning) {
                // Active heat exchanger - cyan tint
                m_heatExchangerMaterials[i]->setDiffuse(QColor(150, 220, 250));
                m_heatExchangerMaterials[i]->setAmbient(QColor(120, 180, 200));
            } else {
                // Inactive heat exchanger - gray
                m_heatExchangerMaterials[i]->setDiffuse(QColor(180, 180, 180));
                m_heatExchangerMaterials[i]->setAmbient(QColor(140, 140, 140));
            }
        }
        
        // Update solenoid valve state (energized visualization)
        if (i < m_solenoidValveMaterials.size() && m_solenoidValveMaterials[i]) {
            if (solenoidOpen) {
                // Energized solenoid - bright yellow/green
                m_solenoidValveMaterials[i]->setDiffuse(QColor(150, 255, 100));
                m_solenoidValveMaterials[i]->setAmbient(QColor(100, 200, 70));
            } else {
                // De-energized solenoid - dark blue
                m_solenoidValveMaterials[i]->setDiffuse(QColor(100, 100, 150));
                m_solenoidValveMaterials[i]->setAmbient(QColor(70, 70, 110));
            }
        }
        
        // Update condenser state
        if (i < m_condenserMaterials.size() && m_condenserMaterials[i]) {
            if (compressorRunning) {
                // Active condenser - warmer color
                m_condenserMaterials[i]->setDiffuse(QColor(200, 180, 160));
                m_condenserMaterials[i]->setAmbient(QColor(160, 140, 120));
            } else {
                // Inactive condenser - gray
                m_condenserMaterials[i]->setDiffuse(QColor(160, 160, 160));
                m_condenserMaterials[i]->setAmbient(QColor(120, 120, 120));
            }
        }
        
        // Update blower rotation and state
        if (i < m_blowerEntities.size()) {
            if (blowerRunning && i < m_blowerTransforms.size() && m_blowerTransforms[i]) {
                // Animate blower rotation (faster than pumps)
                m_blowerRotation += deltaTime * 360.0;
                if (m_blowerRotation >= 360.0) {
                    m_blowerRotation -= 360.0;
                }
                m_blowerTransforms[i]->setRotationY(m_blowerRotation);
            }
            
            // Update blower color based on state
            if (i < m_blowerMaterials.size() && m_blowerMaterials[i]) {
                if (blowerRunning) {
                    // Running blower - brighter blue
                    m_blowerMaterials[i]->setDiffuse(QColor(100, 140, 200));
                    m_blowerMaterials[i]->setAmbient(QColor(80, 100, 150));
                } else {
                    // Idle blower - dark blue
                    m_blowerMaterials[i]->setDiffuse(QColor(80, 80, 120));
                    m_blowerMaterials[i]->setAmbient(QColor(60, 60, 90));
                }
            }
        }
    }
}

// Helper methods for creating 3D objects

Qt3DCore::QEntity* LCUScene3D::createCylinder(const QVector3D &position, float radius, 
                                                float length, const QColor &color, 
                                                Qt3DCore::QEntity *parent)
{
    if (!parent) parent = this;
    
    Qt3DCore::QEntity *entity = new Qt3DCore::QEntity(parent);
    
    Qt3DExtras::QCylinderMesh *mesh = new Qt3DExtras::QCylinderMesh();
    mesh->setRadius(radius);
    mesh->setLength(length);
    mesh->setRings(20);
    mesh->setSlices(20);
    
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);
    material->setAmbient(color.darker(120));
    material->setSpecular(QColor(255, 255, 255));
    material->setShininess(50.0f);
    
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation(position);
    
    entity->addComponent(mesh);
    entity->addComponent(material);
    entity->addComponent(transform);
    
    return entity;
}

Qt3DCore::QEntity* LCUScene3D::createBox(const QVector3D &position, const QVector3D &size, 
                                          const QColor &color, Qt3DCore::QEntity *parent)
{
    if (!parent) parent = this;
    
    Qt3DCore::QEntity *entity = new Qt3DCore::QEntity(parent);
    
    Qt3DExtras::QCuboidMesh *mesh = new Qt3DExtras::QCuboidMesh();
    mesh->setXExtent(size.x());
    mesh->setYExtent(size.y());
    mesh->setZExtent(size.z());
    
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);
    material->setAmbient(color.darker(120));
    material->setSpecular(QColor(255, 255, 255));
    material->setShininess(50.0f);
    
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation(position);
    
    entity->addComponent(mesh);
    entity->addComponent(material);
    entity->addComponent(transform);
    
    return entity;
}

Qt3DCore::QEntity* LCUScene3D::createSphere(const QVector3D &position, float radius, 
                                             const QColor &color, Qt3DCore::QEntity *parent)
{
    if (!parent) parent = this;
    
    Qt3DCore::QEntity *entity = new Qt3DCore::QEntity(parent);
    
    Qt3DExtras::QSphereMesh *mesh = new Qt3DExtras::QSphereMesh();
    mesh->setRadius(radius);
    mesh->setRings(20);
    mesh->setSlices(20);
    
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);
    material->setAmbient(color.darker(120));
    material->setSpecular(QColor(255, 255, 255));
    material->setShininess(50.0f);
    
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation(position);
    
    entity->addComponent(mesh);
    entity->addComponent(material);
    entity->addComponent(transform);
    
    return entity;
}

Qt3DCore::QEntity* LCUScene3D::createPipe(const QVector3D &start, const QVector3D &end, 
                                           float radius, const QColor &color, 
                                           Qt3DCore::QEntity *parent)
{
    if (!parent) parent = this;
    
    QVector3D direction = end - start;
    float length = direction.length();
    QVector3D center = (start + end) / 2.0f;
    
    Qt3DCore::QEntity *entity = new Qt3DCore::QEntity(parent);
    
    Qt3DExtras::QCylinderMesh *mesh = new Qt3DExtras::QCylinderMesh();
    mesh->setRadius(radius);
    mesh->setLength(length);
    mesh->setRings(10);
    mesh->setSlices(12);
    
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);
    material->setAmbient(color.darker(120));
    material->setSpecular(QColor(200, 200, 200));
    material->setShininess(30.0f);
    
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation(center);
    
    // Calculate rotation to align pipe with direction vector
    QVector3D yAxis(0, 1, 0);
    direction.normalize();
    
    // Calculate rotation axis and angle
    QVector3D rotationAxis = QVector3D::crossProduct(yAxis, direction);
    float rotationAngle = qRadiansToDegrees(qAcos(QVector3D::dotProduct(yAxis, direction)));
    
    if (rotationAxis.length() > 0.001f) {
        transform->setRotation(QQuaternion::fromAxisAndAngle(rotationAxis, rotationAngle));
    }
    
    entity->addComponent(mesh);
    entity->addComponent(material);
    entity->addComponent(transform);
    
    return entity;
}

Qt3DCore::QEntity* LCUScene3D::createTorus(const QVector3D &position, float radius, 
                                            float minorRadius, const QColor &color, 
                                            Qt3DCore::QEntity *parent)
{
    if (!parent) parent = this;
    
    Qt3DCore::QEntity *entity = new Qt3DCore::QEntity(parent);
    
    Qt3DExtras::QTorusMesh *mesh = new Qt3DExtras::QTorusMesh();
    mesh->setRadius(radius);
    mesh->setMinorRadius(minorRadius);
    mesh->setRings(30);
    mesh->setSlices(30);
    
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);
    material->setAmbient(color.darker(120));
    material->setSpecular(QColor(255, 255, 255));
    material->setShininess(50.0f);
    
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation(position);
    
    entity->addComponent(mesh);
    entity->addComponent(material);
    entity->addComponent(transform);
    
    return entity;
}
