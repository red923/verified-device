#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QVector>
#include <QString>
#include <QMap>

struct ObjectData
{
    int id;
    QString type;           // Drone, Avion, Obus, Hélicoptère, etc.
    double minLength;
    double maxLength;
    double minWidth;
    double maxWidth;
    double minHeight;
    double maxHeight;
    double minWeight;
    double maxWeight;
    double minSpeed;
    double maxSpeed;
    int minRotors;
    int maxRotors;
    QString material;       // Matériau typique
    QString propulsion;     // Type de propulsion
    QString description;
};

class Database
{
public:
    Database(const QString &dbPath = ":memory:");
    ~Database();

    bool initialize();
    void populateDefaultObjects();
    QVector<ObjectData> getAllObjects();
    ObjectData getObjectById(int id);
    bool addObject(const ObjectData &obj);
    bool updateObject(const ObjectData &obj);
    bool deleteObject(int id);

private:
    QSqlDatabase db;
    QString dbPath;
    bool createTables();
};

#endif // DATABASE_H
