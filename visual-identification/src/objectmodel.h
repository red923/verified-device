#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QString>
#include <QVector>
#include <QPair>

struct VisualSpecification
{
    QString wingShape;       // Forme des ailes
    int numWings;           // Nombre d'ailes
    QString fuselageShape;   // Forme du fuselage
    int numRotors;          // Nombre de rotors
    int numEngines;         // Nombre de moteurs
    bool hasStabilizer;     // A des stabilisateurs
    bool hasWeapons;        // Aspect armé
    QString sizeCategory;   // Catégorie de taille
};

struct IdentificationResult
{
    QString objectType;
    int confidence;     // 0-100
    QString description;
    QVector<QPair<QString, QString>> details;  // Propriété, Valeur
};

#endif // OBJECTMODEL_H
