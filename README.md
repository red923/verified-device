# Système d'Identification d'Objets (Qt5 C++)

Un programme Qt5 en C++ permettant d'identifier automatiquement les objets (drones, avions, obus, hélicoptères, etc.) en comparant leurs caractéristiques avec une base de données.

## Fonctionnalités

✅ **Interface Graphique Intuitive**
- Entrée facile des paramètres de l'objet
- Affichage des résultats en temps réel
- Tableau détaillé des caractéristiques identifiées

✅ **Base de Données Complète**
- 12 types d'objets prédéfinis
- Drones, avions, hélicoptères, obus, fusées
- Plages de dimensions, poids, vitesse
- SQLite pour stockage persistant

✅ **Algorithme Intelligent de Comparaison**
- Calcul de confiance basé sur critères multiples
- Poids adapté pour chaque paramètre
- Tolérance sur les mesures

✅ **Paramètres Gérés**
- Longueur, largeur, hauteur (cm)
- Poids (kg)
- Vitesse maximale (km/h)
- Nombre de rotors
- Type de matériau
- Type de propulsion

## Structure du Projet

```
src/
├── main.cpp           # Point d'entrée
├── mainwindow.h/cpp   # Interface principale
├── database.h/cpp     # Gestion base de données SQLite
├── identifier.h/cpp   # Algorithme d'identification
└── objectmodel.h      # Modèles de données

CMakeLists.txt         # Configuration de build
```

## Types d'Objets Disponibles

### Drones
- Quadricoptère (petit drone civil)
- Drone Cargo (livraison)

### Avions
- Avion léger
- Avion commercial
- Avion militaire

### Hélicoptères
- Hélicoptère léger
- Hélicoptère militaire

### Projectiles
- Obus 105mm
- Obus 155mm
- Missile guidé

### Fusées
- Fusée sonde
- Fusée lourde

## Compilation et Exécution

### Prérequis
- Qt5 (Core, Gui, Widgets, Sql)
- CMake 3.16+
- Compilateur C++17

### Build
```bash
mkdir build
cd build
cmake ..
make
```

### Exécution
```bash
./ObjectIdentification
```

## Utilisation

1. **Entrez les paramètres de l'objet**
   - Remplissez les dimensions (cm)
   - Entrez le poids (kg)
   - Spécifiez la vitesse maximale (km/h)
   - Sélectionnez le matériau et la propulsion

2. **Cliquez sur "Identifier"**
   - Le système compare les données
   - Affiche le type d'objet détecté
   - Affiche la confiance du résultat (0-100%)
   - Liste les caractéristiques correspondantes

3. **Réinitialisez avec "Réinitialiser"**
   - Réinitialise tous les champs
   - Efface les résultats précédents

## Algorithme d'Identification

Le système calcule un score de confiance basé sur :

| Critère | Poids | Description |
|---------|-------|-------------|
| Longueur | 20% | Comparaison avec plage standard |
| Poids | 20% | Comparaison avec plage standard |
| Vitesse | 15% | Plage de vitesse maximale |
| Largeur | 15% | Comparaison avec plage standard |
| Hauteur | 15% | Comparaison avec plage standard |
| Rotors | 10% | Nombre de rotors (si applicable) |
| Matériau | 5% | Type de matériau |
| Propulsion | 5% | Type de propulsion |

Le score final est plafonné à 100% et l'objet avec la confiance la plus élevée est sélectionné.

## Exemple d'Utilisation

**Entrée:**
- Longueur: 50 cm
- Largeur: 50 cm
- Hauteur: 30 cm
- Poids: 2.5 kg
- Vitesse max: 60 km/h
- Rotors: 4
- Matériau: Composite
- Propulsion: Électrique

**Résultat:**
- **Type identifié:** Drone Quadricoptère
- **Confiance:** 95%
- **Description:** Petit drone de surveillance ou loisir

## Améliorations Futures

- [ ] Support de caméra pour capture automatique
- [ ] Apprentissage machine (ML) pour améliorer la précision
- [ ] Export des résultats en PDF/CSV
- [ ] Historique des identifications
- [ ] Ajout manuel de nouveaux types d'objets
- [ ] Intégration avec API GPS
- [ ] Support multi-langues

## License

Ce projet est fourni à titre d'exemple éducatif.
