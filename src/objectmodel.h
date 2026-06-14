#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QString>
#include <QVector>
#include <QPair>

struct ObjectSpecification
{
    double length;      // cm
    double width;       // cm
    double height;      // cm
    double weight;      // kg
    double maxSpeed;    // km/h
    int numRotors;
    QString material;
    QString propulsion;
};

struct IdentificationResult
{
    QString objectType;
    int confidence;     // 0-100
    QString description;
    QVector<QPair<QString, QString>> details;  // Propriété, Valeur
};

#endif // OBJECTMODEL_H
