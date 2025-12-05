#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QFileInfo>
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
}

MainWindow::~MainWindow() {
    m_session.finalizar();
}

void MainWindow::setupBackend() {
    QStringList candidates;
    QString appDir = QCoreApplication::applicationDirPath();
    
    // Build absolute paths from app directory going up to project root
    // App is at: D:/programacion/cc1/modelo_Ngrams_interactivo/Frontend/build/Windows_manual-Debug
    // We need: D:/programacion/cc1/modelo_Ngrams_interactivo/Backend/data/2grams_english.txt
    
    QString projectRoot = QDir(appDir).filePath("../../../../..");
    QDir(projectRoot).canonicalPath(); // normalize path
    
    candidates << QDir(projectRoot).filePath("Backend/data/2grams_english.txt");
    candidates << QDir(projectRoot).filePath("../Backend/data/2grams_english.txt");
    candidates << QDir(appDir).filePath("../Backend/data/2grams_english.txt");
    candidates << QDir(appDir).filePath("../../Backend/data/2grams_english.txt");
    candidates << QDir(appDir).filePath("../../../Backend/data/2grams_english.txt");
    candidates << QDir(appDir).filePath("../../../../Backend/data/2grams_english.txt");
    candidates << "D:/programacion/cc1/modelo_Ngrams_interactivo/Backend/data/2grams_english.txt";

    std::cerr << "[DEBUG] App dir: " << appDir.toStdString() << std::endl;
    
    QString chosen;
    for (int i = 0; i < candidates.size(); i++) {
        bool exists = QFile::exists(candidates[i]);
        std::cerr << "[DEBUG] Try " << i << ": " << candidates[i].toStdString() << " [" << (exists ? "OK" : "X") << "]" << std::endl;
        if (exists) { chosen = candidates[i]; break; }
    }

    if (chosen.isEmpty()) {
        std::cerr << "[ERROR] DB file not found in any candidate path" << std::endl;
        return;
    }

    m_dbPath = chosen.toUtf8();
    std::cerr << "[DEBUG] Using DB: " << m_dbPath.constData() << std::endl;
    m_manager.inicializarModelo(&m_modelo, m_dbPath.constData());
    std::cerr << "[DEBUG] Model initialized" << std::endl;
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

    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(m_optionsList, &QListWidget::itemActivated, this, &MainWindow::onOptionActivated);
    connect(m_backButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(m_restartButton, &QPushButton::clicked, this, &MainWindow::onRestartClicked);

    setCentralWidget(centralWidget);
}

void MainWindow::updateOptions() {
    int cantOps = 0;
    char** opciones = m_session.obtenerOpciones(cantOps);
    std::cerr << "[DEBUG] obtenerOpciones returned " << cantOps << " options" << std::endl;
    m_optionsList->clear();
    if (opciones && cantOps > 0) {
        for (int i = 0; i < cantOps; ++i) {
            std::cerr << "[DEBUG] Option " << i << ": " << opciones[i] << std::endl;
            QString word = QString::fromUtf8(opciones[i]);
            m_optionsList->addItem(word);
        }
        for (int i = 0; i < cantOps; ++i) delete[] opciones[i];
        delete[] opciones;
    } else {
        std::cerr << "[DEBUG] No options" << std::endl;
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
        // update current word label and options
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

    std::cerr << "[DEBUG] Adding word: " << cleanWord.toUtf8().constData() << std::endl;

    const char* ruta = m_dbPath.isEmpty() ? nullptr : m_dbPath.constData();
    if (m_session.obtenerCantidadPalabras() == 0) {
        std::cerr << "[DEBUG] Starting session" << std::endl;
        if (!m_session.iniciar(&m_manager, &m_modelo, cleanWord.toUtf8().constData(), 3, ruta)) {
            std::cerr << "[DEBUG] Session failed" << std::endl;
            return;
        }
        std::cerr << "[DEBUG] Session OK" << std::endl;
    }

    m_inputLine->clear();
    char* texto = m_session.obtenerTextoCompleto();
    if (texto) { m_textDisplay->setText(QString::fromUtf8(texto)); delete[] texto; }
    m_currentWordLabel->setText(QString("Palabra actual: %1").arg(QString::fromUtf8(m_session.obtenerPalabraActual())));
    std::cerr << "[DEBUG] Calling updateOptions" << std::endl;
    updateOptions();
}
