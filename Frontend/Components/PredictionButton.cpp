#include "PredictionButton.h"
#include <QVBoxLayout>

PredictionButton::PredictionButton(const QString& text, int frequency, QWidget* parent)
    : QPushButton(parent), m_frequency(frequency), m_selected(false) {
    
    setText(QString("%1  [%2]").arg(text).arg(frequency));
    setMinimumHeight(50);
    setMaximumHeight(60);
    setCursor(Qt::PointingHandCursor);
    
    connect(this, &QPushButton::clicked, this, &PredictionButton::onClicked);
    updateStyleSheet();
}

PredictionButton::~PredictionButton() {}

void PredictionButton::setSelected(bool selected) {
    m_selected = selected;
    updateStyleSheet();
}

void PredictionButton::onClicked() {
    emit clicked(text().split(" [")[0], m_frequency);
}

void PredictionButton::updateStyleSheet() {
    if (m_selected) {
        setStyleSheet(
            "PredictionButton {"
            "  background-color: #4CAF50;"
            "  color: white;"
            "  border: 2px solid #45a049;"
            "  border-radius: 5px;"
            "  font-weight: bold;"
            "  padding: 8px;"
            "}"
            "PredictionButton:hover {"
            "  background-color: #45a049;"
            "}"
        );
    } else {
        setStyleSheet(
            "PredictionButton {"
            "  background-color: #2a2a2a;"
            "  color: #e0e0e0;"
            "  border: 1px solid #444;"
            "  border-radius: 5px;"
            "  padding: 8px;"
            "}"
            "PredictionButton:hover {"
            "  background-color: #3a3a3a;"
            "  border: 1px solid #666;"
            "}"
        );
    }
}
