#include "InputWidget.h"

InputWidget::InputWidget(QWidget* parent)
    : QWidget(parent) {
    
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(5);
    
    m_input = new QLineEdit(this);
    m_input->setPlaceholderText("Nueva palabra: ");
    m_input->setMinimumHeight(35);
    
    m_addBtn = new QPushButton("+", this);
    m_addBtn->setMaximumWidth(50);
    m_addBtn->setMinimumHeight(35);
    
    layout->addWidget(m_input);
    layout->addWidget(m_addBtn);
    
    connect(m_addBtn, &QPushButton::clicked, this, &InputWidget::onAddClicked);
    connect(m_input, &QLineEdit::returnPressed, this, &InputWidget::onReturnPressed);
    
    updateStyleSheet();
}

InputWidget::~InputWidget() {}

QString InputWidget::getText() const {
    return m_input->text();
}

void InputWidget::clear() {
    m_input->clear();
}

void InputWidget::onAddClicked() {
    if (!m_input->text().trimmed().isEmpty()) {
        emit addWord(m_input->text().trimmed());
        clear();
    }
}

void InputWidget::onReturnPressed() {
    onAddClicked();
}

void InputWidget::updateStyleSheet() {
    m_input->setStyleSheet(
        "QLineEdit {"
        "  background-color: #2a2a2a;"
        "  color: #e0e0e0;"
        "  border: 1px solid #444;"
        "  border-radius: 5px;"
        "  padding: 5px 10px;"
        "}"
        "QLineEdit:focus {"
        "  border: 1px solid #00ff88;"
        "}"
    );
    
    m_addBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #00ff88;"
        "  color: #000;"
        "  border: none;"
        "  border-radius: 5px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #00dd77;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #00cc66;"
        "}"
    );
}
