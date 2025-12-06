#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_modelo(), m_session(), m_manager(), m_currentText("")
{
    setWindowTitle("Word Predictor");
    setGeometry(100, 100, 600, 400);
    setupBackend();
    setupUI();
    m_spinner->setValue(3); // Default value
}

MainWindow::~MainWindow() {
    m_session.finalizar();
}

void MainWindow::setupBackend() {
    QStringList candidates;
    QString appDir = QCoreApplication::applicationDirPath();
    
    QString projectRoot = QDir(appDir).filePath("../../../../..");
    QDir(projectRoot).canonicalPath();
    
    candidates << QDir(projectRoot).filePath("Backend/data/2grams_english.txt");
    candidates << QDir(projectRoot).filePath("../Backend/data/2grams_english.txt");
    candidates << QDir(appDir).filePath("../Backend/data/2grams_english.txt");
    candidates << QDir(appDir).filePath("../../Backend/data/2grams_english.txt");
    candidates << QDir(appDir).filePath("../../../Backend/data/2grams_english.txt");
    candidates << QDir(appDir).filePath("../../../../Backend/data/2grams_english.txt");
    candidates << "D:/programacion/cc1/modelo_Ngrams_interactivo/Backend/data/2grams_english.txt";


    
    QString chosen;
    for (int i = 0; i < candidates.size(); i++) {
        bool exists = QFile::exists(candidates[i]);

        if (exists) { chosen = candidates[i]; break; }
    }

    if (chosen.isEmpty()) {
        std::cerr << "[ERROR] DB file not found in any candidate path" << std::endl;
        return;
    }

    m_dbPath = chosen.toUtf8();
    m_manager.inicializarModelo(&m_modelo, m_dbPath.constData());
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QLabel* titleLabel = new QLabel("Word Predictor", this);
    mainLayout->addWidget(titleLabel);

    m_textDisplay = new QLabel("Ingresa una palabra para comenzar...", this);
    m_textDisplay->setWordWrap(true);
    mainLayout->addWidget(m_textDisplay);

    m_currentWordLabel = new QLabel("Palabra actual: -", this);
    mainLayout->addWidget(m_currentWordLabel);

    m_optionsList = new QListWidget(this);
    mainLayout->addWidget(m_optionsList);

    QHBoxLayout* actionLayout = new QHBoxLayout();
    m_backButton = new QPushButton("Atrás", this);
    m_restartButton = new QPushButton("Reiniciar", this);
    actionLayout->addWidget(m_backButton);
    actionLayout->addWidget(m_restartButton);
    mainLayout->addLayout(actionLayout);

    QHBoxLayout* inputLayout = new QHBoxLayout();
    m_inputLine = new QLineEdit(this);
    m_addButton = new QPushButton("Añadir", this);
    inputLayout->addWidget(m_inputLine);
    inputLayout->addWidget(m_addButton);
    mainLayout->addLayout(inputLayout);

    m_spinner = new QSpinBox(this);
    m_spinner->setRange(1, 10);
    m_spinner->setPrefix("Predicciones: ");
    m_spinner->setMinimumWidth(120);
    inputLayout->addWidget(m_spinner);

    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(m_optionsList, &QListWidget::itemActivated, this, &MainWindow::onOptionActivated);
    connect(m_backButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(m_restartButton, &QPushButton::clicked, this, &MainWindow::onRestartClicked);
    
    connect(m_spinner, qOverload<int>(&QSpinBox::valueChanged), this, [this](int i){
        m_session.setCantidadOpciones(i);
        updateOptions();
    });

    connect(m_inputLine, &QLineEdit::returnPressed, this, &MainWindow::onAddClicked);

    setCentralWidget(centralWidget);
}

void MainWindow::updateOptions() {
    int cantOps = 0;
    char** opciones = m_session.obtenerOpciones(cantOps);

    m_optionsList->clear();
    if (opciones && cantOps > 0) {
        for (int i = 0; i < cantOps; ++i) {

            QString word = QString::fromUtf8(opciones[i]);

            int* freqs = m_session.obtenerUltimasFrecuencias(cantOps);
            int totalFreq = 0;
            for(int k=0; k<cantOps; k++) totalFreq += freqs[k];
            
            double porcentaje = 0.0;
            if (totalFreq > 0) {
                porcentaje = (double)freqs[i] / totalFreq * 100.0;
            }
            
            QString label = QString("%1 (%2%)").arg(word).arg(porcentaje, 0, 'f', 1);
            m_optionsList->addItem(label);
        }
        for (int i = 0; i < cantOps; ++i) delete[] opciones[i];
        delete[] opciones;
    } else {

    }
}

void MainWindow::onOptionActivated(QListWidgetItem* item) {
    if (!item) return;
    int row = m_optionsList->row(item);
    if (row < 0) return;
    if (m_session.seleccionarOpcion(row)) {
        char* texto = m_session.obtenerTextoCompleto();
        if (texto) {
            m_textDisplay->setText(QString::fromUtf8(texto));
            delete[] texto;
        }
        m_currentWordLabel->setText(QString("Palabra actual: %1").arg(QString::fromUtf8(m_session.obtenerPalabraActual())));
        updateOptions();
    }
}

void MainWindow::onBackClicked() {
    if (m_session.retroceder()) {
        char* texto = m_session.obtenerTextoCompleto();
        if (texto) { m_textDisplay->setText(QString::fromUtf8(texto)); delete[] texto; }
        m_currentWordLabel->setText(QString("Palabra actual: %1").arg(QString::fromUtf8(m_session.obtenerPalabraActual())));
        updateOptions();
    }
}

void MainWindow::onRestartClicked() {
    m_session.finalizar();
    m_textDisplay->setText("Ingresa una palabra para comenzar...");
    m_currentWordLabel->setText("Palabra actual: -");
    m_optionsList->clear();
    m_inputLine->clear();
}

void MainWindow::onAddClicked() {
    QString cleanWord = m_inputLine->text().trimmed().toLower();
    if (cleanWord.isEmpty()) return;

    QString validWord;
    for (QChar c : cleanWord) {
        if (c.isLetter()) {
            validWord.append(c);
        }
    }
    
    if (validWord.isEmpty()) return;
    cleanWord = validWord;



    const char* ruta = m_dbPath.isEmpty() ? nullptr : m_dbPath.constData();
    if (m_session.obtenerCantidadPalabras() == 0) {
        if (!m_session.iniciar(&m_manager, &m_modelo, cleanWord.toUtf8().constData(), m_spinner->value(), ruta)) {
            return;
        }
    } else {
        handleInputWord(cleanWord);
    }
    
    m_inputLine->clear();

    char* texto = m_session.obtenerTextoCompleto();
    if (texto) { m_textDisplay->setText(QString::fromUtf8(texto)); delete[] texto; }
    m_currentWordLabel->setText(QString("Palabra actual: %1").arg(QString::fromUtf8(m_session.obtenerPalabraActual())));
    
    updateOptions();
}

void MainWindow::handleInputWord(QString word) {
    m_session.seleccionarPalabra(word.toUtf8().constData());
}
