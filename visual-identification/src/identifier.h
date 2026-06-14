#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "database.h"
#include "objectmodel.h"
#include <QString>
#include <QVector>

class Identifier
{
public:
    Identifier(Database *db);
    ~Identifier();

    IdentificationResult identifyFromVisuals(const VisualSpecification &spec);

private:
    Database *db;

    // Helper functions
    int calculateVisualConfidence(const VisualSpecification &spec, const VisualObjectData &obj);
    bool matchesWingShape(const QString &inputShape, const QString &dbShape);
    bool matchesFuselageShape(const QString &inputShape, const QString &dbShape);
    bool matchesSize(const QString &inputSize, const QString &dbSize);
};

#endif // IDENTIFIER_H
