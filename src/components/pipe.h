#ifndef PIPE_H
#define PIPE_H

#include "basecomponent.h"
#include <QPointF>
#include <QVector>

class Pipe : public BaseComponent
{
    Q_OBJECT

public:
    explicit Pipe(QObject *parent = nullptr);
    
    void setPath(const QVector<QPointF> &points);
    QVector<QPointF> getPath() const { return m_points; }
    
    void setFlowing(bool flowing);
    bool isFlowing() const { return m_flowing; }
    
    void setFlowDirection(bool forward); // true = forward, false = reverse
    void setWidth(double width);
    
    void setFluidColor(const QColor &color);
    
    void updateAnimation(double deltaTime) override;

protected:
    void paintComponent(QPainter *painter) override;

private:
    QVector<QPointF> m_points;
    bool m_flowing;
    bool m_flowForward;
    double m_width;
    QColor m_fluidColor;
    double m_flowOffset;
};

#endif // PIPE_H
