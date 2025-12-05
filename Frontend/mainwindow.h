#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QProgressBar>
#include <QStringList>
#include <QByteArray>
#include <QListWidgetItem>

#include "../Backend/Ngrams/manager/NGramManager.hpp"
#include "../Backend/Ngrams/manager/PredictionSession.hpp"
#include "../Backend/Ngrams/tipos/bigrams/Bigram.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOptionActivated(QListWidgetItem* item);
    void onBackClicked();
    void onRestartClicked();
    void onAddClicked();

private:
    void setupUI();
    void setupBackend();
    void updateOptions();

    QLabel* m_textDisplay;
    QLabel* m_currentWordLabel;
    QListWidget* m_optionsList;
    QPushButton* m_backButton;
    QPushButton* m_restartButton;
    QLineEdit* m_inputLine;
    QPushButton* m_addButton;
    QSpinBox* m_predictionCountSpinner;
    QProgressBar* m_progressBar;

    NGramManager m_manager;
    Bigram m_modelo;
    PredictionSession m_session;
    QByteArray m_dbPath; // stored DB path bytes for passing to backend

    QString m_currentText;
    QStringList m_history;
    int m_currentPredictionCount;
};

#endif // MAINWINDOW_H
