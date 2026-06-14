#include "identifier.h"
#include <QDebug>
#include <cmath>

Identifier::Identifier(Database *db)
    : db(db)
{
}

Identifier::~Identifier()
{
}

IdentificationResult Identifier::identify(const ObjectSpecification &spec)
{
    IdentificationResult bestResult;
    bestResult.objectType = "Inconnu";
    bestResult.confidence = 0;
    bestResult.description = "Aucune correspondance trouvée";

    QVector<ObjectData> allObjects = db->getAllObjects();

    for (const auto &obj : allObjects) {
        int confidence = calculateConfidence(spec, obj);

        if (confidence > bestResult.confidence) {
            bestResult.confidence = confidence;
            bestResult.objectType = obj.type;
            bestResult.description = obj.description;
            bestResult.details.clear();

            // Add details
            bestResult.details.append({"Type", obj.type});
            bestResult.details.append({"Description", obj.description});
            bestResult.details.append({"Matériau typique", obj.material});
            bestResult.details.append({"Propulsion", obj.propulsion});
            bestResult.details.append({"Plage de longueur (cm)", 
                                      QString("%1 - %2").arg(obj.minLength).arg(obj.maxLength)});
            bestResult.details.append({"Plage de poids (kg)", 
                                      QString("%1 - %2").arg(obj.minWeight).arg(obj.maxWeight)});
            bestResult.details.append({"Plage de vitesse (km/h)", 
                                      QString("%1 - %2").arg(obj.minSpeed).arg(obj.maxSpeed)});
        }
    }

    return bestResult;
}

int Identifier::calculateConfidence(const ObjectSpecification &spec, const ObjectData &obj)
{
    int confidence = 0;
    int totalCriteria = 0;

    // Check length (weight: 20%)
    totalCriteria++;
    double tolerance = (obj.maxLength - obj.minLength) * 0.2; // 20% tolerance
    if (isInRange(spec.length, obj.minLength, obj.maxLength, tolerance)) {
        double diff = std::abs(spec.length - (obj.minLength + obj.maxLength) / 2);
        double maxDiff = (obj.maxLength - obj.minLength) / 2 + tolerance;
        confidence += 20 * (1.0 - diff / maxDiff);
    }

    // Check width (weight: 15%)
    totalCriteria++;
    tolerance = (obj.maxWidth - obj.minWidth) * 0.2;
    if (isInRange(spec.width, obj.minWidth, obj.maxWidth, tolerance)) {
        double diff = std::abs(spec.width - (obj.minWidth + obj.maxWidth) / 2);
        double maxDiff = (obj.maxWidth - obj.minWidth) / 2 + tolerance;
        confidence += 15 * (1.0 - diff / maxDiff);
    }

    // Check height (weight: 15%)
    totalCriteria++;
    tolerance = (obj.maxHeight - obj.minHeight) * 0.2;
    if (isInRange(spec.height, obj.minHeight, obj.maxHeight, tolerance)) {
        double diff = std::abs(spec.height - (obj.minHeight + obj.maxHeight) / 2);
        double maxDiff = (obj.maxHeight - obj.minHeight) / 2 + tolerance;
        confidence += 15 * (1.0 - diff / maxDiff);
    }

    // Check weight (weight: 20%)
    totalCriteria++;
    tolerance = (obj.maxWeight - obj.minWeight) * 0.2;
    if (isInRange(spec.weight, obj.minWeight, obj.maxWeight, tolerance)) {
        double diff = std::abs(spec.weight - (obj.minWeight + obj.maxWeight) / 2);
        double maxDiff = (obj.maxWeight - obj.minWeight) / 2 + tolerance;
        confidence += 20 * (1.0 - diff / maxDiff);
    }

    // Check max speed (weight: 15%)
    totalCriteria++;
    tolerance = (obj.maxSpeed - obj.minSpeed) * 0.2;
    if (isInRange(spec.maxSpeed, obj.minSpeed, obj.maxSpeed, tolerance)) {
        double diff = std::abs(spec.maxSpeed - (obj.minSpeed + obj.maxSpeed) / 2);
        double maxDiff = (obj.maxSpeed - obj.minSpeed) / 2 + tolerance;
        confidence += 15 * (1.0 - diff / maxDiff);
    }

    // Check rotors (weight: 10%)
    if (obj.minRotors > 0 || obj.maxRotors > 0) {
        totalCriteria++;
        if (spec.numRotors >= obj.minRotors && spec.numRotors <= obj.maxRotors) {
            confidence += 10;
        }
    }

    // Check material (weight: 5%)
    if (!obj.material.isEmpty() && !spec.material.isEmpty()) {
        totalCriteria++;
        if (spec.material.toLower() == obj.material.toLower() ||
            obj.material.toLower().contains(spec.material.toLower())) {
            confidence += 5;
        }
    }

    // Check propulsion (weight: 5%)
    if (!obj.propulsion.isEmpty() && !spec.propulsion.isEmpty()) {
        totalCriteria++;
        if (spec.propulsion.toLower() == obj.propulsion.toLower() ||
            obj.propulsion.toLower().contains(spec.propulsion.toLower())) {
            confidence += 5;
        }
    }

    return std::min(100, confidence);
}

bool Identifier::isInRange(double value, double min, double max, double tolerance)
{
    return value >= (min - tolerance) && value <= (max + tolerance);
}

QString Identifier::formatValue(const QString &label, const QString &value)
{
    return QString("%1: %2").arg(label, value);
}
