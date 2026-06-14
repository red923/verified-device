#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QVector>
#include <QString>
#include <QMap>

struct VisualObjectData
{
    int id;
    QString type;              // Drone, Avion, Obus, etc.
    QString wingShape;         // Forme typique des ailes
    int numWingsMin;
    int numWingsMax;
    QString fuselageShape;     // Forme du fuselage
    int numRotorsMin;
    int numRotorsMax;
    int numEnginesMin;
    int numEnginesMax;
    bool typicallyHasStabilizer;
    bool typicallyArmed;       // Aspect armé típique
    QString sizeCategory;      // Catégorie de taille
    QString description;
};

class Database
{
public:
    Database(const QString &dbPath = ":memory:");
    ~Database();

    bool initialize();
    void populateDefaultObjects();
    QVector<VisualObjectData> getAllObjects();
    VisualObjectData getObjectById(int id);
    bool addObject(const VisualObjectData &obj);
    bool updateObject(const VisualObjectData &obj);
    bool deleteObject(int id);

private:
    QSqlDatabase db;
    QString dbPath;
    bool createTables();
};

#endif // DATABASE_H
