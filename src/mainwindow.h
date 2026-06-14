#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
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

    // Input widgets
    QDoubleSpinBox *lengthInput;
    QDoubleSpinBox *widthInput;
    QDoubleSpinBox *heightInput;
    QDoubleSpinBox *weightInput;
    QDoubleSpinBox *maxSpeedInput;
    QSpinBox *numRotorsInput;
    QComboBox *materialCombo;
    QComboBox *propulsionCombo;

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
