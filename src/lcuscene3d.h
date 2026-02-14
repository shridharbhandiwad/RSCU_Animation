#ifndef LCUSCENE3D_H
#define LCUSCENE3D_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>
#include <QVector>

class DataModel;

namespace Qt3DCore {
    class QEntity;
    class QTransform;
}

namespace Qt3DRender {
    class QMaterial;
    class QMesh;
}

namespace Qt3DExtras {
    class QPhongMaterial;
    class QCylinderMesh;
    class QSphereMesh;
    class QCuboidMesh;
    class QTorusMesh;
}

class LCUScene3D : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    explicit LCUScene3D(DataModel *dataModel, Qt3DCore::QEntity *parent = nullptr);
    
    void updateAnimations(double deltaTime);
    void setupCamera(Qt3DRender::QCamera *camera);

private:
    void setupScene();
    void setupLighting();
    void createCoolantSystem();
    void createRefrigerantSystem();
    void createChannelSystem();
    void createPipingConnections();
    void createLabels();
    void createFloor();
    
    // Helper methods for creating 3D objects
    Qt3DCore::QEntity* createCylinder(const QVector3D &position, float radius, float length, 
                                       const QColor &color, Qt3DCore::QEntity *parent = nullptr);
    Qt3DCore::QEntity* createBox(const QVector3D &position, const QVector3D &size, 
                                  const QColor &color, Qt3DCore::QEntity *parent = nullptr);
    Qt3DCore::QEntity* createSphere(const QVector3D &position, float radius, 
                                     const QColor &color, Qt3DCore::QEntity *parent = nullptr);
    Qt3DCore::QEntity* createPipe(const QVector3D &start, const QVector3D &end, 
                                   float radius, const QColor &color, Qt3DCore::QEntity *parent = nullptr);
    Qt3DCore::QEntity* createTorus(const QVector3D &position, float radius, float minorRadius,
                                    const QColor &color, Qt3DCore::QEntity *parent = nullptr);
    
    DataModel *m_dataModel;
    
    // Scene entities
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DCore::QEntity *m_lightEntity;
    
    // Coolant system components
    Qt3DCore::QEntity *m_tankEntity;
    Qt3DExtras::QPhongMaterial *m_tankMaterial;
    Qt3DCore::QEntity *m_heaterEntity;
    Qt3DExtras::QPhongMaterial *m_heaterMaterial;
    QVector<Qt3DCore::QEntity*> m_pumpEntities;
    QVector<Qt3DCore::QTransform*> m_pumpTransforms;
    QVector<Qt3DExtras::QPhongMaterial*> m_pumpMaterials;
    QVector<Qt3DCore::QEntity*> m_coolantPipeEntities;
    
    // Channel system
    QVector<Qt3DCore::QEntity*> m_valveEntities;
    QVector<Qt3DCore::QTransform*> m_valveTransforms;
    QVector<Qt3DExtras::QPhongMaterial*> m_valveMaterials;
    QVector<Qt3DCore::QEntity*> m_channelPipeEntities;
    
    // Refrigerant system
    QVector<Qt3DCore::QEntity*> m_heatExchangerEntities;
    QVector<Qt3DExtras::QPhongMaterial*> m_heatExchangerMaterials;
    QVector<Qt3DCore::QEntity*> m_condenserEntities;
    QVector<Qt3DExtras::QPhongMaterial*> m_condenserMaterials;
    QVector<Qt3DCore::QEntity*> m_blowerEntities;
    QVector<Qt3DCore::QTransform*> m_blowerTransforms;
    QVector<Qt3DExtras::QPhongMaterial*> m_blowerMaterials;
    QVector<Qt3DCore::QEntity*> m_solenoidValveEntities;
    QVector<Qt3DExtras::QPhongMaterial*> m_solenoidValveMaterials;
    QVector<Qt3DCore::QEntity*> m_refrigerantPipeEntities;
    
    // Animation tracking
    double m_pumpRotation;
    double m_blowerRotation;
};

#endif // LCUSCENE3D_H
