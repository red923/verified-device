#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), db(""), identifier(&db)
{
    setWindowTitle("Système d'Identification Visuelle d'Appareils");
    setGeometry(100, 100, 1000, 650);

    // Initialize database
    onInitializeDatabase();

    // Setup UI
    setupUI();
    connectSignals();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Left panel - Input
    QGroupBox *inputGroup = new QGroupBox("Caractéristiques Visuelles Observées", this);
    QVBoxLayout *inputLayout = new QVBoxLayout(inputGroup);

    // Wing Shape
    QHBoxLayout *wingLayout = new QHBoxLayout();
    wingLayout->addWidget(new QLabel("Forme des ailes:"));
    wingShapeCombo = new QComboBox();
    wingShapeCombo->addItems({
        "-- Sélectionner --",
        "Delta (Triangulaire)",
        "Elliptique (Arrondie)",
        "Rectangulaire",
        "Rotative (Rotors)",
        "Courte/Stubby",
        "Longue/Fine",
        "Aucune (Obus/Missile)"
    });
    wingLayout->addWidget(wingShapeCombo);
    wingLayout->addStretch();
    inputLayout->addLayout(wingLayout);

    // Number of Wings
    QHBoxLayout *numWingsLayout = new QHBoxLayout();
    numWingsLayout->addWidget(new QLabel("Nombre d'ailes/surfaces:"));
    numWingsInput = new QSpinBox();
    numWingsInput->setRange(0, 4);
    numWingsInput->setValue(2);
    numWingsLayout->addWidget(numWingsInput);
    numWingsLayout->addWidget(new QLabel("(0=Rotors, 1=Monoplan, 2=Biplan, 4=Quadricopter)"));
    numWingsLayout->addStretch();
    inputLayout->addLayout(numWingsLayout);

    // Fuselage Shape
    QHBoxLayout *fuselageLayout = new QHBoxLayout();
    fuselageLayout->addWidget(new QLabel("Forme du fuselage:"));
    fuselageShapeCombo = new QComboBox();
    fuselageShapeCombo->addItems({
        "-- Sélectionner --",
        "Mince et effilé",
        "Élancé (Avion civil)",
        "Compact (Drone)",
        "Bulbeux",
        "Cylindrique (Obus/Missile)"
    });
    fuselageLayout->addWidget(fuselageShapeCombo);
    fuselageLayout->addStretch();
    inputLayout->addLayout(fuselageLayout);

    // Number of Rotors/Propellers
    QHBoxLayout *rotorLayout = new QHBoxLayout();
    rotorLayout->addWidget(new QLabel("Nombre de rotors/hélices visibles:"));
    numRotorsInput = new QSpinBox();
    numRotorsInput->setRange(0, 8);
    numRotorsInput->setValue(0);
    rotorLayout->addWidget(numRotorsInput);
    rotorLayout->addStretch();
    inputLayout->addLayout(rotorLayout);

    // Number of Engines
    QHBoxLayout *engineLayout = new QHBoxLayout();
    engineLayout->addWidget(new QLabel("Nombre de moteurs/réacteurs visibles:"));
    numEnginesInput = new QSpinBox();
    numEnginesInput->setRange(0, 4);
    numEnginesInput->setValue(1);
    engineLayout->addWidget(numEnginesInput);
    engineLayout->addStretch();
    inputLayout->addLayout(engineLayout);

    // Stabilizer
    QHBoxLayout *stabilizerLayout = new QHBoxLayout();
    hasStabilizerCheck = new QCheckBox("Possède des stabilisateurs/ailettes (queue visible)");
    hasStabilizerCheck->setChecked(true);
    stabilizerLayout->addWidget(hasStabilizerCheck);
    stabilizerLayout->addStretch();
    inputLayout->addLayout(stabilizerLayout);

    // Weapons/Armed appearance
    QHBoxLayout *weaponsLayout = new QHBoxLayout();
    hasWeaponsCheck = new QCheckBox("Aspect armé (missiles, mitrailleuses visibles)");
    hasWeaponsCheck->setChecked(false);
    weaponsLayout->addWidget(hasWeaponsCheck);
    weaponsLayout->addStretch();
    inputLayout->addLayout(weaponsLayout);

    // Size
    QHBoxLayout *sizeLayout = new QHBoxLayout();
    sizeLayout->addWidget(new QLabel("Taille relative:"));
    sizeCombo = new QComboBox();
    sizeCombo->addItems({
        "-- Sélectionner --",
        "Très petit (< 50cm)",
        "Petit (50cm - 2m)",
        "Moyen (2m - 10m)",
        "Grand (10m - 30m)",
        "Très grand (> 30m)"
    });
    sizeLayout->addWidget(sizeCombo);
    sizeLayout->addStretch();
    inputLayout->addLayout(sizeLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    identifyBtn = new QPushButton("🔍 Identifier");
    identifyBtn->setMinimumHeight(40);
    clearBtn = new QPushButton("🔄 Réinitialiser");
    clearBtn->setMinimumHeight(40);
    buttonLayout->addWidget(identifyBtn);
    buttonLayout->addWidget(clearBtn);
    inputLayout->addLayout(buttonLayout);

    inputLayout->addStretch();
    inputGroup->setMaximumWidth(500);

    // Right panel - Output
    QGroupBox *outputGroup = new QGroupBox("Résultat d'Identification", this);
    QVBoxLayout *outputLayout = new QVBoxLayout(outputGroup);

    resultLabel = new QLabel("Sélectionnez les caractéristiques et cliquez sur 'Identifier'");
    resultLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #333; padding: 10px; background-color: #f0f0f0; border-radius: 5px;");
    resultLabel->setWordWrap(true);
    resultLabel->setMinimumHeight(60);
    outputLayout->addWidget(resultLabel);

    resultTable = new QTableWidget();
    resultTable->setColumnCount(2);
    resultTable->setHorizontalHeaderLabels({"Propriété", "Valeur"});
    resultTable->horizontalHeader()->setStretchLastSection(true);
    resultTable->setMaximumHeight(400);
    outputLayout->addWidget(resultTable);

    outputLayout->addStretch();

    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(outputGroup, 1);

    centralWidget->setLayout(mainLayout);
}

void MainWindow::connectSignals()
{
    connect(identifyBtn, &QPushButton::clicked, this, &MainWindow::onIdentifyClicked);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearClicked);
}

void MainWindow::onIdentifyClicked()
{
    // Validation minimale
    if (wingShapeCombo->currentIndex() == 0 || fuselageShapeCombo->currentIndex() == 0 || sizeCombo->currentIndex() == 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner au moins les 3 paramètres principaux:\n- Forme des ailes\n- Forme du fuselage\n- Taille relative");
        return;
    }

    VisualSpecification spec;
    spec.wingShape = wingShapeCombo->currentText();
    spec.numWings = numWingsInput->value();
    spec.fuselageShape = fuselageShapeCombo->currentText();
    spec.numRotors = numRotorsInput->value();
    spec.numEngines = numEnginesInput->value();
    spec.hasStabilizer = hasStabilizerCheck->isChecked();
    spec.hasWeapons = hasWeaponsCheck->isChecked();
    spec.sizeCategory = sizeCombo->currentText();

    IdentificationResult result = identifier.identifyFromVisuals(spec);

    // Update result label with formatted output
    QString confidenceColor = result.confidence >= 80 ? "green" : (result.confidence >= 50 ? "orange" : "red");
    resultLabel->setText(
        QString("<b style='color: %1; font-size: 16px;'>%2</b><br/>"
                "<span style='color: %1;'>Confiance: %3%</span><br/>"
                "<span style='font-size: 12px;'>%4</span>")
        .arg(confidenceColor)
        .arg(result.objectType)
        .arg(result.confidence)
        .arg(result.description)
    );

    // Update result table
    resultTable->setRowCount(0);
    resultTable->insertRow(0);
    resultTable->setItem(0, 0, new QTableWidgetItem("Type d'objet identifié"));
    resultTable->setItem(0, 1, new QTableWidgetItem(result.objectType));

    resultTable->insertRow(1);
    resultTable->setItem(1, 0, new QTableWidgetItem("Confiance (%)"));
    QTableWidgetItem *confidenceItem = new QTableWidgetItem(QString::number(result.confidence));
    confidenceItem->setBackground(result.confidence >= 80 ? QColor(144, 238, 144) : (result.confidence >= 50 ? QColor(255, 200, 124) : QColor(255, 127, 127)));
    resultTable->setItem(1, 1, confidenceItem);

    resultTable->insertRow(2);
    resultTable->setItem(2, 0, new QTableWidgetItem("Description"));
    resultTable->setItem(2, 1, new QTableWidgetItem(result.description));

    int row = 3;
    for (const auto &detail : result.details) {
        resultTable->insertRow(row);
        resultTable->setItem(row, 0, new QTableWidgetItem(detail.first));
        resultTable->setItem(row, 1, new QTableWidgetItem(detail.second));
        row++;
    }
}

void MainWindow::onClearClicked()
{
    wingShapeCombo->setCurrentIndex(0);
    numWingsInput->setValue(2);
    fuselageShapeCombo->setCurrentIndex(0);
    numRotorsInput->setValue(0);
    numEnginesInput->setValue(1);
    hasStabilizerCheck->setChecked(true);
    hasWeaponsCheck->setChecked(false);
    sizeCombo->setCurrentIndex(0);
    resultLabel->setText("Sélectionnez les caractéristiques et cliquez sur 'Identifier'");
    resultTable->setRowCount(0);
}

void MainWindow::onInitializeDatabase()
{
    if (db.initialize()) {
        db.populateDefaultObjects();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible d'initialiser la base de données");
    }
}
