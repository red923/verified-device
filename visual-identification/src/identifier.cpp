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

IdentificationResult Identifier::identifyFromVisuals(const VisualSpecification &spec)
{
    IdentificationResult bestResult;
    bestResult.objectType = "Inconnu";
    bestResult.confidence = 0;
    bestResult.description = "Impossible d'identifier l'objet avec les données fournies";

    QVector<VisualObjectData> allObjects = db->getAllObjects();

    for (const auto &obj : allObjects) {
        int confidence = calculateVisualConfidence(spec, obj);

        if (confidence > bestResult.confidence) {
            bestResult.confidence = confidence;
            bestResult.objectType = obj.type;
            bestResult.description = obj.description;
            bestResult.details.clear();

            // Add details
            bestResult.details.append({"Type d'objet", obj.type});
            bestResult.details.append({"Description", obj.description});
            bestResult.details.append({"Forme des ailes", obj.wingShape});
            bestResult.details.append({"Forme du fuselage", obj.fuselageShape});
            bestResult.details.append({"Nombre de rotors", QString("%1-%2").arg(obj.numRotorsMin).arg(obj.numRotorsMax)});
            bestResult.details.append({"Nombre de moteurs", QString("%1-%2").arg(obj.numEnginesMin).arg(obj.numEnginesMax)});
            bestResult.details.append({"Stabilisateurs", obj.typicallyHasStabilizer ? "Oui" : "Non"});
            bestResult.details.append({"Aspect armé", obj.typicallyArmed ? "Oui" : "Non"});
            bestResult.details.append({"Catégorie de taille", obj.sizeCategory});
        }
    }

    return bestResult;
}

int Identifier::calculateVisualConfidence(const VisualSpecification &spec, const VisualObjectData &obj)
{
    int confidence = 0;
    int totalCriteria = 0;

    // 1. Wing Shape (weight: 25%)
    totalCriteria++;
    if (matchesWingShape(spec.wingShape, obj.wingShape)) {
        confidence += 25;
    } else if ((spec.wingShape.contains("Rotative") && obj.wingShape == "Rotative") ||
               (spec.wingShape.contains("Delta") && obj.wingShape == "Delta") ||
               (spec.wingShape.contains("Cylindrique") && obj.wingShape == "Aucune")) {
        confidence += 15; // Partial match
    }

    // 2. Fuselage Shape (weight: 20%)
    totalCriteria++;
    if (matchesFuselageShape(spec.fuselageShape, obj.fuselageShape)) {
        confidence += 20;
    } else if ((spec.fuselageShape.contains("Compact") && obj.fuselageShape == "Compact") ||
               (spec.fuselageShape.contains("Cylindrique") && obj.fuselageShape == "Cylindrique")) {
        confidence += 10; // Partial match
    }

    // 3. Number of Wings (weight: 15%)
    if (obj.numWingsMin > 0 || obj.numWingsMax > 0) {
        totalCriteria++;
        if (spec.numWings >= obj.numWingsMin && spec.numWings <= obj.numWingsMax) {
            confidence += 15;
        }
    }

    // 4. Number of Rotors (weight: 15%)
    if (obj.numRotorsMin > 0 || obj.numRotorsMax > 0) {
        totalCriteria++;
        if (spec.numRotors >= obj.numRotorsMin && spec.numRotors <= obj.numRotorsMax) {
            confidence += 15;
        } else if (std::abs(spec.numRotors - obj.numRotorsMin) <= 1 || std::abs(spec.numRotors - obj.numRotorsMax) <= 1) {
            confidence += 8; // Close match
        }
    }

    // 5. Number of Engines (weight: 10%)
    totalCriteria++;
    if (spec.numEngines >= obj.numEnginesMin && spec.numEngines <= obj.numEnginesMax) {
        confidence += 10;
    } else if (std::abs(spec.numEngines - obj.numEnginesMin) <= 1) {
        confidence += 5; // Close match
    }

    // 6. Stabilizer (weight: 7%)
    totalCriteria++;
    if (spec.hasStabilizer == obj.typicallyHasStabilizer) {
        confidence += 7;
    } else if (!spec.hasStabilizer && obj.typicallyHasStabilizer) {
        confidence += 3; // Might not see it clearly
    }

    // 7. Weapons (weight: 5%)
    totalCriteria++;
    if (spec.hasWeapons == obj.typicallyArmed) {
        confidence += 5;
    } else if (spec.hasWeapons && !obj.typicallyArmed) {
        confidence += 2; // False positive
    }

    // 8. Size Category (weight: 3%)
    totalCriteria++;
    if (matchesSize(spec.sizeCategory, obj.sizeCategory)) {
        confidence += 3;
    }

    return std::min(100, confidence);
}

bool Identifier::matchesWingShape(const QString &inputShape, const QString &dbShape)
{
    if (inputShape == dbShape) return true;
    
    // Fuzzy matching
    if (inputShape.contains("Rotative") && dbShape == "Rotative") return true;
    if (inputShape.contains("Delta") && dbShape == "Delta") return true;
    if (inputShape.contains("Elliptique") && (dbShape == "Elliptique" || dbShape == "Rotative")) return true;
    if (inputShape.contains("Rectangulaire") && dbShape == "Rectangulaire") return true;
    if (inputShape.contains("Aucune") && dbShape == "Aucune") return true;
    
    return false;
}

bool Identifier::matchesFuselageShape(const QString &inputShape, const QString &dbShape)
{
    if (inputShape == dbShape) return true;
    
    // Fuzzy matching
    if (inputShape.contains("Cylindrique") && dbShape == "Cylindrique") return true;
    if (inputShape.contains("Compact") && (dbShape == "Compact" || dbShape == "Bulbeux")) return true;
    if (inputShape.contains("Élancé") && dbShape == "Élancé") return true;
    if (inputShape.contains("Mince") && (dbShape == "Mince et effilé" || dbShape == "Élancé")) return true;
    
    return false;
}

bool Identifier::matchesSize(const QString &inputSize, const QString &dbSize)
{
    return inputSize == dbSize;
}
