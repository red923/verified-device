#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include "database.h"
#include "identifier.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onIdentifyClicked();
    void onClearClicked();
    void onInitializeDatabase();

private:
    void setupUI();
    void connectSignals();

    // Input widgets - Caractéristiques visuelles simples
    QComboBox *wingShapeCombo;      // Forme des ailes
    QSpinBox *numWingsInput;         // Nombre d'ailes
    QComboBox *fuselageShapeCombo;   // Forme du fuselage
    QSpinBox *numRotorsInput;        // Nombre de rotors/hélices
    QSpinBox *numEnginesInput;       // Nombre de moteurs visibles
    QCheckBox *hasStabilizerCheck;   // A des stabilisateurs
    QCheckBox *hasWeaponsCheck;      // Aspect armé
    QComboBox *sizeCombo;            // Taille relative

    // Output widgets
    QLabel *resultLabel;
    QTableWidget *resultTable;
    QPushButton *identifyBtn;
    QPushButton *clearBtn;

    // Backend
    Database db;
    Identifier identifier;
};

#endif // MAINWINDOW_H
