#include "ActionBar.h"

ActionBar::ActionBar(QWidget* parent)
    : QWidget(parent) {
    
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(10);
    
    m_backBtn = new QPushButton("← Atrás", this);
    m_restartBtn = new QPushButton("⟲ Reiniciar", this);
    
    m_backBtn->setMinimumHeight(40);
    m_restartBtn->setMinimumHeight(40);
    
    layout->addWidget(m_backBtn);
    layout->addWidget(m_restartBtn);
    
    connect(m_backBtn, &QPushButton::clicked, this, &ActionBar::backClicked);
    connect(m_restartBtn, &QPushButton::clicked, this, &ActionBar::restartClicked);
    
    updateStyleSheet();
}

ActionBar::~ActionBar() {}

void ActionBar::setupButtons() {
    // Configuración adicional si es necesaria
}

void ActionBar::updateStyleSheet() {
    const QString buttonStyle =
        "QPushButton {"
        "  background-color: #2a2a2a;"
        "  color: #e0e0e0;"
        "  border: 1px solid #444;"
        "  border-radius: 5px;"
        "  padding: 8px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #3a3a3a;"
        "  border: 1px solid #666;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #1a1a1a;"
        "}";
    
    m_backBtn->setStyleSheet(buttonStyle);
    m_restartBtn->setStyleSheet(buttonStyle);
}
