#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), db(""), identifier(&db)
{
    setWindowTitle("Système d'Identification d'Objets");
    setWindowIcon(QIcon(":/icons/app.png"));
    setGeometry(100, 100, 1200, 700);

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
    QGroupBox *inputGroup = new QGroupBox("Données d'Entrée", this);
    QVBoxLayout *inputLayout = new QVBoxLayout(inputGroup);

    // Length
    QHBoxLayout *lengthLayout = new QHBoxLayout();
    lengthLayout->addWidget(new QLabel("Longueur (cm):"));
    lengthInput = new QDoubleSpinBox();
    lengthInput->setRange(0, 10000);
    lengthInput->setValue(100);
    lengthLayout->addWidget(lengthInput);
    inputLayout->addLayout(lengthLayout);

    // Width
    QHBoxLayout *widthLayout = new QHBoxLayout();
    widthLayout->addWidget(new QLabel("Largeur (cm):"));
    widthInput = new QDoubleSpinBox();
    widthInput->setRange(0, 10000);
    widthInput->setValue(100);
    widthLayout->addWidget(widthInput);
    inputLayout->addLayout(widthLayout);

    // Height
    QHBoxLayout *heightLayout = new QHBoxLayout();
    heightLayout->addWidget(new QLabel("Hauteur (cm):"));
    heightInput = new QDoubleSpinBox();
    heightInput->setRange(0, 10000);
    heightInput->setValue(50);
    heightLayout->addWidget(heightInput);
    inputLayout->addLayout(heightLayout);

    // Weight
    QHBoxLayout *weightLayout = new QHBoxLayout();
    weightLayout->addWidget(new QLabel("Poids (kg):"));
    weightInput = new QDoubleSpinBox();
    weightInput->setRange(0, 100000);
    weightInput->setValue(2000);
    weightLayout->addWidget(weightInput);
    inputLayout->addLayout(weightLayout);

    // Max Speed
    QHBoxLayout *speedLayout = new QHBoxLayout();
    speedLayout->addWidget(new QLabel("Vitesse Max (km/h):"));
    maxSpeedInput = new QDoubleSpinBox();
    maxSpeedInput->setRange(0, 10000);
    maxSpeedInput->setValue(200);
    speedLayout->addWidget(maxSpeedInput);
    inputLayout->addLayout(speedLayout);

    // Number of Rotors
    QHBoxLayout *rotorLayout = new QHBoxLayout();
    rotorLayout->addWidget(new QLabel("Nombre de Rotors:"));
    numRotorsInput = new QSpinBox();
    numRotorsInput->setRange(0, 16);
    numRotorsInput->setValue(4);
    rotorLayout->addWidget(numRotorsInput);
    inputLayout->addLayout(rotorLayout);

    // Material
    QHBoxLayout *materialLayout = new QHBoxLayout();
    materialLayout->addWidget(new QLabel("Matériau:"));
    materialCombo = new QComboBox();
    materialCombo->addItems({"Acier", "Aluminium", "Titane", "Composite", "Inconnu"});
    materialLayout->addWidget(materialCombo);
    inputLayout->addLayout(materialLayout);

    // Propulsion
    QHBoxLayout *propulsionLayout = new QHBoxLayout();
    propulsionLayout->addWidget(new QLabel("Type de Propulsion:"));
    propulsionCombo = new QComboBox();
    propulsionCombo->addItems({"Moteur à Explosion", "Électrique", "Réaction", "Hélice", "Inconnu"});
    propulsionLayout->addWidget(propulsionCombo);
    inputLayout->addLayout(propulsionLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    identifyBtn = new QPushButton("Identifier");
    clearBtn = new QPushButton("Réinitialiser");
    buttonLayout->addWidget(identifyBtn);
    buttonLayout->addWidget(clearBtn);
    inputLayout->addLayout(buttonLayout);

    inputLayout->addStretch();
    inputGroup->setMaximumWidth(400);

    // Right panel - Output
    QGroupBox *outputGroup = new QGroupBox("Résultat d'Identification", this);
    QVBoxLayout *outputLayout = new QVBoxLayout(outputGroup);

    resultLabel = new QLabel("Entrez les données et cliquez sur 'Identifier'");
    resultLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #333;");
    outputLayout->addWidget(resultLabel);

    resultTable = new QTableWidget();
    resultTable->setColumnCount(2);
    resultTable->setHorizontalHeaderLabels({"Propriété", "Valeur"});
    resultTable->horizontalHeader()->setStretchLastSection(true);
    resultTable->setMaximumHeight(300);
    outputLayout->addWidget(resultTable);

    outputLayout->addStretch();

    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(outputGroup);

    centralWidget->setLayout(mainLayout);
}

void MainWindow::connectSignals()
{
    connect(identifyBtn, &QPushButton::clicked, this, &MainWindow::onIdentifyClicked);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearClicked);
}

void MainWindow::onIdentifyClicked()
{
    ObjectSpecification spec;
    spec.length = lengthInput->value();
    spec.width = widthInput->value();
    spec.height = heightInput->value();
    spec.weight = weightInput->value();
    spec.maxSpeed = maxSpeedInput->value();
    spec.numRotors = numRotorsInput->value();
    spec.material = materialCombo->currentText();
    spec.propulsion = propulsionCombo->currentText();

    IdentificationResult result = identifier.identify(spec);

    // Update result label
    resultLabel->setText(QString("Type identifié: <span style='color: green; font-size: 16px;'><b>%1</b></span> (Confiance: %2%)").arg(result.objectType).arg(result.confidence));

    // Update result table
    resultTable->setRowCount(0);
    resultTable->insertRow(0);
    resultTable->setItem(0, 0, new QTableWidgetItem("Type d'Objet"));
    resultTable->setItem(0, 1, new QTableWidgetItem(result.objectType));

    resultTable->insertRow(1);
    resultTable->setItem(1, 0, new QTableWidgetItem("Confiance (%)"));
    resultTable->setItem(1, 1, new QTableWidgetItem(QString::number(result.confidence)));

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
    lengthInput->setValue(100);
    widthInput->setValue(100);
    heightInput->setValue(50);
    weightInput->setValue(2000);
    maxSpeedInput->setValue(200);
    numRotorsInput->setValue(4);
    materialCombo->setCurrentIndex(0);
    propulsionCombo->setCurrentIndex(0);
    resultLabel->setText("Entrez les données et cliquez sur 'Identifier'");
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
