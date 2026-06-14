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
        CREATE TABLE IF NOT EXISTS visual_objects (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT NOT NULL UNIQUE,
            wing_shape TEXT,
            num_wings_min INTEGER,
            num_wings_max INTEGER,
            fuselage_shape TEXT,
            num_rotors_min INTEGER,
            num_rotors_max INTEGER,
            num_engines_min INTEGER,
            num_engines_max INTEGER,
            typically_has_stabilizer INTEGER,
            typically_armed INTEGER,
            size_category TEXT,
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
    QVector<VisualObjectData> objects = {
        // Drones
        {0, "Drone Quadricoptère", "Rotative", 0, 0, "Compact", 4, 4, 4, 4, false, false, "Très petit (< 50cm)", "Petit drone de surveillance avec 4 rotors"},
        {0, "Drone Hexacoptère", "Rotative", 0, 0, "Compact", 6, 6, 6, 6, false, false, "Petit (50cm - 2m)", "Drone professionnel avec 6 rotors"},
        {0, "Drone Cargo", "Rotative", 0, 0, "Compact", 4, 8, 4, 8, false, false, "Petit (50cm - 2m)", "Drone de livraison avec plusieurs rotors"},
        
        // Avions
        {0, "Avion Léger Civil", "Elliptique", 1, 1, "Élancé", 0, 0, 1, 1, true, false, "Moyen (2m - 10m)", "Petit avion civil avec queue visible"},
        {0, "Avion Commercial", "Elliptique", 1, 1, "Élancé", 0, 0, 2, 4, true, false, "Très grand (> 30m)", "Avion de ligne avec réacteurs"},
        {0, "Avion Militaire Chasse", "Delta", 1, 1, "Mince et effilé", 0, 0, 1, 2, true, true, "Moyen (2m - 10m)", "Avion de chase avec silhouette agressive"},
        {0, "Avion Bombardier", "Rectangulaire", 1, 1, "Élancé", 0, 0, 2, 4, true, true, "Grand (10m - 30m)", "Bombardier lourd avec armes visibles"},
        
        // Hélicoptères
        {0, "Hélicoptère Léger", "Rotative", 0, 0, "Compact", 2, 2, 1, 2, true, false, "Petit (50cm - 2m)", "Hélicoptère civil léger avec 2 rotors"},
        {0, "Hélicoptère Militaire d'Attaque", "Rotative", 0, 0, "Mince et effilé", 2, 2, 2, 2, true, true, "Moyen (2m - 10m)", "Hélicoptère d'attaque avec armement"},
        {0, "Hélicoptère Transport", "Rotative", 0, 0, "Bulbeux", 2, 2, 2, 3, true, false, "Grand (10m - 30m)", "Gros hélicoptère de transport"},
        
        // Projectiles
        {0, "Obus Standard", "Aucune", 0, 0, "Cylindrique", 0, 0, 0, 0, true, true, "Petit (50cm - 2m)", "Projectile d'artillerie avec ailettes"},
        {0, "Missile Guidé Petit", "Aucune", 0, 0, "Cylindrique", 0, 0, 1, 1, true, true, "Petit (50cm - 2m)", "Petit missile avec réacteur"},
        {0, "Missile Guidé Moyen", "Aucune", 0, 0, "Cylindrique", 0, 0, 1, 1, true, true, "Moyen (2m - 10m)", "Missile de croisière"},
        {0, "Missile Anti-Aérien", "Aucune", 0, 0, "Cylindrique", 0, 0, 1, 1, true, true, "Petit (50cm - 2m)", "Missile guidé avec ailettes dirigeantes"},
        
        // Fusées
        {0, "Fusée Sonde", "Aucune", 0, 0, "Cylindrique", 0, 0, 1, 1, true, false, "Moyen (2m - 10m)", "Fusée scientifique avec ailettes"},
        {0, "Fusée Lourde", "Aucune", 0, 0, "Cylindrique", 0, 0, 1, 4, true, false, "Très grand (> 30m)", "Lanceur spatial lourd"},
    };

    for (const auto &obj : objects) {
        if (!addObject(obj)) {
            qWarning() << "Impossible d'ajouter" << obj.type;
        }
    }
}

QVector<VisualObjectData> Database::getAllObjects()
{
    QVector<VisualObjectData> objects;
    QSqlQuery query("SELECT * FROM visual_objects");

    while (query.next()) {
        VisualObjectData obj;
        obj.id = query.value("id").toInt();
        obj.type = query.value("type").toString();
        obj.wingShape = query.value("wing_shape").toString();
        obj.numWingsMin = query.value("num_wings_min").toInt();
        obj.numWingsMax = query.value("num_wings_max").toInt();
        obj.fuselageShape = query.value("fuselage_shape").toString();
        obj.numRotorsMin = query.value("num_rotors_min").toInt();
        obj.numRotorsMax = query.value("num_rotors_max").toInt();
        obj.numEnginesMin = query.value("num_engines_min").toInt();
        obj.numEnginesMax = query.value("num_engines_max").toInt();
        obj.typicallyHasStabilizer = query.value("typically_has_stabilizer").toInt() == 1;
        obj.typicallyArmed = query.value("typically_armed").toInt() == 1;
        obj.sizeCategory = query.value("size_category").toString();
        obj.description = query.value("description").toString();

        objects.append(obj);
    }

    return objects;
}

VisualObjectData Database::getObjectById(int id)
{
    VisualObjectData obj = {0, "", "", 0, 0, "", 0, 0, 0, 0, false, false, "", ""};
    QSqlQuery query;
    query.prepare("SELECT * FROM visual_objects WHERE id = :id");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        obj.id = query.value("id").toInt();
        obj.type = query.value("type").toString();
        obj.wingShape = query.value("wing_shape").toString();
        obj.numWingsMin = query.value("num_wings_min").toInt();
        obj.numWingsMax = query.value("num_wings_max").toInt();
        obj.fuselageShape = query.value("fuselage_shape").toString();
        obj.numRotorsMin = query.value("num_rotors_min").toInt();
        obj.numRotorsMax = query.value("num_rotors_max").toInt();
        obj.numEnginesMin = query.value("num_engines_min").toInt();
        obj.numEnginesMax = query.value("num_engines_max").toInt();
        obj.typicallyHasStabilizer = query.value("typically_has_stabilizer").toInt() == 1;
        obj.typicallyArmed = query.value("typically_armed").toInt() == 1;
        obj.sizeCategory = query.value("size_category").toString();
        obj.description = query.value("description").toString();
    }

    return obj;
}

bool Database::addObject(const VisualObjectData &obj)
{
    QSqlQuery query;
    query.prepare("INSERT INTO visual_objects (type, wing_shape, num_wings_min, num_wings_max, fuselage_shape, "
                  "num_rotors_min, num_rotors_max, num_engines_min, num_engines_max, typically_has_stabilizer, "
                  "typically_armed, size_category, description) "
                  "VALUES (:type, :wing_shape, :num_wings_min, :num_wings_max, :fuselage_shape, "
                  ":num_rotors_min, :num_rotors_max, :num_engines_min, :num_engines_max, :typically_has_stabilizer, "
                  ":typically_armed, :size_category, :description)");

    query.addBindValue(obj.type);
    query.addBindValue(obj.wingShape);
    query.addBindValue(obj.numWingsMin);
    query.addBindValue(obj.numWingsMax);
    query.addBindValue(obj.fuselageShape);
    query.addBindValue(obj.numRotorsMin);
    query.addBindValue(obj.numRotorsMax);
    query.addBindValue(obj.numEnginesMin);
    query.addBindValue(obj.numEnginesMax);
    query.addBindValue(obj.typicallyHasStabilizer ? 1 : 0);
    query.addBindValue(obj.typicallyArmed ? 1 : 0);
    query.addBindValue(obj.sizeCategory);
    query.addBindValue(obj.description);

    if (!query.exec()) {
        qWarning() << "Erreur insertion:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::updateObject(const VisualObjectData &obj)
{
    QSqlQuery query;
    query.prepare("UPDATE visual_objects SET wing_shape=:wing_shape, num_wings_min=:num_wings_min, num_wings_max=:num_wings_max, "
                  "fuselage_shape=:fuselage_shape, num_rotors_min=:num_rotors_min, num_rotors_max=:num_rotors_max, "
                  "num_engines_min=:num_engines_min, num_engines_max=:num_engines_max, typically_has_stabilizer=:typically_has_stabilizer, "
                  "typically_armed=:typically_armed, size_category=:size_category, description=:description WHERE id=:id");

    query.addBindValue(obj.wingShape);
    query.addBindValue(obj.numWingsMin);
    query.addBindValue(obj.numWingsMax);
    query.addBindValue(obj.fuselageShape);
    query.addBindValue(obj.numRotorsMin);
    query.addBindValue(obj.numRotorsMax);
    query.addBindValue(obj.numEnginesMin);
    query.addBindValue(obj.numEnginesMax);
    query.addBindValue(obj.typicallyHasStabilizer ? 1 : 0);
    query.addBindValue(obj.typicallyArmed ? 1 : 0);
    query.addBindValue(obj.sizeCategory);
    query.addBindValue(obj.description);
    query.addBindValue(obj.id);

    return query.exec();
}

bool Database::deleteObject(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM visual_objects WHERE id=:id");
    query.addBindValue(id);
    return query.exec();
}
