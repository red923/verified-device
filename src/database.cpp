#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QFileInfo>

Database::Database(const QString &dbPath)
    : dbPath(dbPath.isEmpty() ? ":memory:" : dbPath)
{
}

Database::~Database()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool Database::initialize()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    if (dbPath != ":memory:") {
        db.setDatabaseName(dbPath);
    }

    if (!db.open()) {
        qWarning() << "Erreur: Impossible d'ouvrir la base de données" << db.lastError().text();
        return false;
    }

    return createTables();
}

bool Database::createTables()
{
    QSqlQuery query;

    QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS objects (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT NOT NULL UNIQUE,
            min_length REAL,
            max_length REAL,
            min_width REAL,
            max_width REAL,
            min_height REAL,
            max_height REAL,
            min_weight REAL,
            max_weight REAL,
            min_speed REAL,
            max_speed REAL,
            min_rotors INTEGER,
            max_rotors INTEGER,
            material TEXT,
            propulsion TEXT,
            description TEXT
        )
    )";

    if (!query.exec(createTableSQL)) {
        qWarning() << "Erreur création table:" << query.lastError().text();
        return false;
    }

    return true;
}

void Database::populateDefaultObjects()
{
    QVector<ObjectData> objects = {
        // Drone
        {0, "Drone Quadricoptère", 20, 100, 20, 100, 5, 50, 0.5, 5, 20, 80, 4, 4, "Composite", "Électrique", "Petit drone de surveillance ou loisir"},
        {0, "Drone Cargo", 100, 300, 100, 300, 30, 150, 10, 100, 30, 100, 4, 8, "Composite/Aluminium", "Électrique", "Drone de livraison ou professionnel"},
        
        // Avion
        {0, "Avion Léger", 600, 1000, 600, 1200, 250, 400, 1000, 3000, 200, 400, 0, 0, "Aluminium", "Moteur à Explosion", "Petit avion civil ou militaire"},
        {0, "Avion Commercial", 5000, 7000, 6000, 8000, 1700, 2500, 50000, 150000, 800, 1000, 0, 0, "Aluminium", "Réaction", "Avion de ligne à réaction"},
        {0, "Avion Militaire", 4000, 6000, 3000, 5000, 1200, 1800, 15000, 40000, 1500, 2500, 0, 0, "Titane/Aluminium", "Réaction", "Avion de chasse ou bombardier"},
        
        // Hélicoptère
        {0, "Hélicoptère Léger", 1200, 1600, 1000, 1400, 900, 1200, 1500, 3000, 200, 250, 2, 2, "Aluminium", "Moteur à Explosion", "Hélicoptère civil léger"},
        {0, "Hélicoptère Militaire", 1500, 1800, 1200, 1600, 1000, 1400, 4000, 8000, 250, 350, 2, 2, "Acier/Titane", "Turbine", "Hélicoptère d'attaque ou transport"},
        
        // Obus
        {0, "Obus de 105mm", 45, 50, 10, 12, 10, 12, 15, 20, 800, 1000, 0, 0, "Acier", "Poudre", "Projectile d'artillerie légère"},
        {0, "Obus de 155mm", 65, 75, 15, 18, 15, 18, 40, 50, 900, 1100, 0, 0, "Acier", "Poudre", "Projectile d'artillerie standard"},
        {0, "Missile Guidé", 300, 600, 30, 60, 30, 60, 100, 500, 2000, 4000, 0, 0, "Titane/Composite", "Réaction", "Missile de croisière ou air-sol"},
        
        // Fusée
        {0, "Fusée Sonde", 500, 1500, 100, 300, 100, 300, 100, 1000, 3000, 8000, 0, 0, "Aluminium/Titane", "Réaction", "Fusée scientifique"},
        {0, "Fusée Lourde", 50000, 70000, 3000, 4000, 3000, 4000, 500000, 2000000, 10000, 28000, 0, 0, "Aluminium/Titane", "Réaction", "Lanceur spatial lourd"},
    };

    for (const auto &obj : objects) {
        if (!addObject(obj)) {
            qWarning() << "Impossible d'ajouter" << obj.type;
        }
    }
}

QVector<ObjectData> Database::getAllObjects()
{
    QVector<ObjectData> objects;
    QSqlQuery query("SELECT * FROM objects");

    while (query.next()) {
        ObjectData obj;
        obj.id = query.value("id").toInt();
        obj.type = query.value("type").toString();
        obj.minLength = query.value("min_length").toDouble();
        obj.maxLength = query.value("max_length").toDouble();
        obj.minWidth = query.value("min_width").toDouble();
        obj.maxWidth = query.value("max_width").toDouble();
        obj.minHeight = query.value("min_height").toDouble();
        obj.maxHeight = query.value("max_height").toDouble();
        obj.minWeight = query.value("min_weight").toDouble();
        obj.maxWeight = query.value("max_weight").toDouble();
        obj.minSpeed = query.value("min_speed").toDouble();
        obj.maxSpeed = query.value("max_speed").toDouble();
        obj.minRotors = query.value("min_rotors").toInt();
        obj.maxRotors = query.value("max_rotors").toInt();
        obj.material = query.value("material").toString();
        obj.propulsion = query.value("propulsion").toString();
        obj.description = query.value("description").toString();

        objects.append(obj);
    }

    return objects;
}

ObjectData Database::getObjectById(int id)
{
    ObjectData obj = {0, "", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "", "", ""};
    QSqlQuery query;
    query.prepare("SELECT * FROM objects WHERE id = :id");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        obj.id = query.value("id").toInt();
        obj.type = query.value("type").toString();
        obj.minLength = query.value("min_length").toDouble();
        obj.maxLength = query.value("max_length").toDouble();
        obj.minWidth = query.value("min_width").toDouble();
        obj.maxWidth = query.value("max_width").toDouble();
        obj.minHeight = query.value("min_height").toDouble();
        obj.maxHeight = query.value("max_height").toDouble();
        obj.minWeight = query.value("min_weight").toDouble();
        obj.maxWeight = query.value("max_weight").toDouble();
        obj.minSpeed = query.value("min_speed").toDouble();
        obj.maxSpeed = query.value("max_speed").toDouble();
        obj.minRotors = query.value("min_rotors").toInt();
        obj.maxRotors = query.value("max_rotors").toInt();
        obj.material = query.value("material").toString();
        obj.propulsion = query.value("propulsion").toString();
        obj.description = query.value("description").toString();
    }

    return obj;
}

bool Database::addObject(const ObjectData &obj)
{
    QSqlQuery query;
    query.prepare("INSERT INTO objects (type, min_length, max_length, min_width, max_width, min_height, max_height, "
                  "min_weight, max_weight, min_speed, max_speed, min_rotors, max_rotors, material, propulsion, description) "
                  "VALUES (:type, :min_length, :max_length, :min_width, :max_width, :min_height, :max_height, "
                  ":min_weight, :max_weight, :min_speed, :max_speed, :min_rotors, :max_rotors, :material, :propulsion, :description)");

    query.addBindValue(obj.type);
    query.addBindValue(obj.minLength);
    query.addBindValue(obj.maxLength);
    query.addBindValue(obj.minWidth);
    query.addBindValue(obj.maxWidth);
    query.addBindValue(obj.minHeight);
    query.addBindValue(obj.maxHeight);
    query.addBindValue(obj.minWeight);
    query.addBindValue(obj.maxWeight);
    query.addBindValue(obj.minSpeed);
    query.addBindValue(obj.maxSpeed);
    query.addBindValue(obj.minRotors);
    query.addBindValue(obj.maxRotors);
    query.addBindValue(obj.material);
    query.addBindValue(obj.propulsion);
    query.addBindValue(obj.description);

    if (!query.exec()) {
        qWarning() << "Erreur insertion:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::updateObject(const ObjectData &obj)
{
    QSqlQuery query;
    query.prepare("UPDATE objects SET min_length=:min_length, max_length=:max_length, "
                  "min_width=:min_width, max_width=:max_width, min_height=:min_height, max_height=:max_height, "
                  "min_weight=:min_weight, max_weight=:max_weight, min_speed=:min_speed, max_speed=:max_speed, "
                  "min_rotors=:min_rotors, max_rotors=:max_rotors, material=:material, propulsion=:propulsion, "
                  "description=:description WHERE id=:id");

    query.addBindValue(obj.minLength);
    query.addBindValue(obj.maxLength);
    query.addBindValue(obj.minWidth);
    query.addBindValue(obj.maxWidth);
    query.addBindValue(obj.minHeight);
    query.addBindValue(obj.maxHeight);
    query.addBindValue(obj.minWeight);
    query.addBindValue(obj.maxWeight);
    query.addBindValue(obj.minSpeed);
    query.addBindValue(obj.maxSpeed);
    query.addBindValue(obj.minRotors);
    query.addBindValue(obj.maxRotors);
    query.addBindValue(obj.material);
    query.addBindValue(obj.propulsion);
    query.addBindValue(obj.description);
    query.addBindValue(obj.id);

    return query.exec();
}

bool Database::deleteObject(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM objects WHERE id=:id");
    query.addBindValue(id);
    return query.exec();
}
