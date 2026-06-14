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

    IdentificationResult identify(const ObjectSpecification &spec);

private:
    Database *db;

    // Helper functions
    int calculateConfidence(const ObjectSpecification &spec, const ObjectData &obj);
    bool isInRange(double value, double min, double max, double tolerance = 0.0);
    QString formatValue(const QString &label, const QString &value);
};

#endif // IDENTIFIER_H
