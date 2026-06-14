# Système d'Identification Visuelle d'Appareils (Qt5 C++)

Un programme Qt5 en C++ permettant d'identifier les appareils en vol **basé uniquement sur l'observation visuelle** de leurs caractéristiques (forme des ailes, fuselage, rotors, etc.).

## 🎯 Concept

Le programme se concentre sur ce que l'on peut **observer facilement** lors du vol d'un appareil:
- **Forme des ailes** (delta, elliptique, rotors, etc.)
- **Forme du fuselage** (compact, élancé, cylindrique, etc.)
- **Nombre d'ailes/rotors/moteurs** visibles
- **Présence de stabilisateurs** (queue visible)
- **Aspect armé** (missiles, mitrailleuses visibles)
- **Taille relative** approximative

## ✨ Caractéristiques

✅ **Interface Simple et Intuitive**
- Entrée minimale (seulement ce qu'on peut observer)
- Pas besoin de mesures exactes
- Sélections prédéfinies (dropdowns)
- Validation des champs

✅ **Base de Données Visuelles**
- 16 types d'appareils prédéfinis
- Drones (Quadricoptère, Hexacoptère, Cargo)
- Avions (Civil, Commercial, Militaire, Bombardier)
- Hélicoptères (Léger, d'Attaque, Transport)
- Projectiles (Obus, Missiles)
- Fusées (Sonde, Lourde)

✅ **Algorithme de Correspondance Intelligent**
- Calcul de confiance basé sur critères visuels
- Correspondance floue (fuzzy matching)
- Poids adapté pour chaque caractéristique

✅ **Résultats Détaillés**
- Type d'appareil identifié
- Score de confiance (0-100%)
- Description complète
- Caractéristiques typiques de l'objet

## 📋 Paramètres Visuels Observables

### 1. **Forme des Ailes**
- Delta (Triangulaire) → Avions militaires, missiles
- Elliptique (Arrondie) → Avions civils, certains hélicoptères
- Rectangulaire → Avions commerciaux, transporteurs
- Rotative (Rotors) → Drones, hélicoptères
- Courte/Stubby → Drones compacts
- Longue/Fine → Planeurs, avions de reconnaissance
- Aucune (Obus/Missile) → Projectiles, fusées

### 2. **Nombre d'Ailes/Surfaces**
- 0 = Rotors ou missiles
- 1 = Monoplan (un plan portant)
- 2 = Biplan (deux plans portants)
- 4 = Quadricoptère

### 3. **Forme du Fuselage**
- Mince et effilé → Avions militaires, missiles
- Élancé (Avion civil) → Avions de ligne
- Compact (Drone) → Drones
- Bulbeux → Hélicoptères de transport
- Cylindrique → Obus, missiles, fusées

### 4. **Nombre de Rotors/Hélices Visibles**
- 2 = Hélicoptères classiques
- 4 = Quadricoptères (drones)
- 6 = Hexacoptères (drones professionnels)
- 8+ = Drones lourds

### 5. **Nombre de Moteurs/Réacteurs Visibles**
- 1 = Petit appareil, avion léger
- 2 = Avion commerciaux, gros hélicoptères
- 4 = Gros avions commerciaux, bombardiers

### 6. **Stabilisateurs**
- Oui = Avions, hélicoptères (queue visible)
- Non = Drones, missiles (équilibre aérodynamique intégré)

### 7. **Aspect Armé**
- Oui = Avions militaires, hélicoptères d'attaque, missiles
- Non = Appareils civils, drones civils

### 8. **Taille Relative**
- Très petit (< 50cm) → Drones commerciaux, petits obus
- Petit (50cm - 2m) → Drones professionnels, missiles
- Moyen (2m - 10m) → Avions légers, hélicoptères, fusées petites
- Grand (10m - 30m) → Avions commerciaux, gros hélicoptères
- Très grand (> 30m) → Grands avions, fusées lourdes

## 🚀 Types d'Appareils Reconnaissables

### Drones
- Drone Quadricoptère (4 rotors)
- Drone Hexacoptère (6 rotors)
- Drone Cargo (plusieurs rotors)

### Avions
- Avion Léger Civil (1 aile, 1 moteur, queue)
- Avion Commercial (1 aile, 2-4 réacteurs)
- Avion Militaire Chasse (aile delta, aspect armé)
- Avion Bombardier (gros, armé)

### Hélicoptères
- Hélicoptère Léger (2 rotors, civil)
- Hélicoptère Militaire d'Attaque (2 rotors, armé)
- Hélicoptère Transport (gros, fuselage bulbeux)

### Projectiles
- Obus Standard (cylindrique, ailettes)
- Missile Guidé (cylindrique, réacteur visible)
- Missile Anti-Aérien (ailettes dirigeantes)

### Fusées
- Fusée Sonde (scientifique, ailettes)
- Fusée Lourde (lanceur spatial)

## 📊 Algorithme de Correspondance

| Critère | Poids | Description |
|---------|-------|-------------|
| Forme des ailes | 25% | Plus caractéristique |
| Forme du fuselage | 20% | Très distinctif |
| Nombre d'ailes | 15% | Monoplan vs biplan |
| Nombre de rotors | 15% | Signature de drone |
| Nombre de moteurs | 10% | Puissance apparente |
| Stabilisateurs | 7% | Présence/absence |
| Aspect armé | 5% | Civil vs militaire |
| Taille catégorie | 3% | Ordre de grandeur |

**Minimum de 3 paramètres requis** pour identification valide.

## 🔧 Compilation et Exécution

### Prérequis
- Qt5 (Core, Gui, Widgets, Sql)
- CMake 3.16+
- Compilateur C++17

### Build
```bash
cd visual-identification
mkdir build
cd build
cmake ..
make
```

### Exécution
```bash
./VisualIdentification
```

## 💡 Exemples d'Utilisation

### Exemple 1: Drone Quadricoptère
- **Forme des ailes:** Rotative
- **Nombre d'ailes:** 0
- **Forme du fuselage:** Compact
- **Rotors:** 4
- **Moteurs:** 4
- **Stabilisateurs:** Non
- **Armé:** Non
- **Taille:** Très petit (< 50cm)

**Résultat:** ✅ **Drone Quadricoptère** (Confiance: 95%)

---

### Exemple 2: Avion Militaire
- **Forme des ailes:** Delta
- **Nombre d'ailes:** 1
- **Forme du fuselage:** Mince et effilé
- **Rotors:** 0
- **Moteurs:** 1-2
- **Stabilisateurs:** Oui
- **Armé:** Oui
- **Taille:** Moyen (2m - 10m)

**Résultat:** ✅ **Avion Militaire Chasse** (Confiance: 92%)

---

### Exemple 3: Missile Guidé
- **Forme des ailes:** Aucune
- **Nombre d'ailes:** 0
- **Forme du fuselage:** Cylindrique
- **Rotors:** 0
- **Moteurs:** 1
- **Stabilisateurs:** Oui (ailettes)
- **Armé:** Oui
- **Taille:** Petit (50cm - 2m)

**Résultat:** ✅ **Missile Guidé** (Confiance: 88%)

## 🎮 Utilisation

1. **Observer l'appareil en vol**
2. **Sélectionner les caractéristiques observables** (minimum 3)
3. **Cliquer sur "Identifier"**
4. **Voir le résultat avec confiance et détails**

## 📈 Améliorations Futures

- [ ] Intégration caméra pour capture automatique
- [ ] Machine Learning pour reconnaissance d'image
- [ ] Historique des identifications
- [ ] Ajout de nouveaux types manuellement
- [ ] Détection du bruit moteur
- [ ] Integration GPS/radar
- [ ] Statistiques et rapports

## 📝 Licence

Projet éducatif et de démonstration.
