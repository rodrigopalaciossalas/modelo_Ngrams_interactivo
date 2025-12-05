#include "TextDisplay.h"

TextDisplay::TextDisplay(QWidget* parent)
    : QLabel(parent) {
    setMinimumHeight(80);
    setMaximumHeight(100);
    setWordWrap(true);
    setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    setText("Ingresa una palabra para comenzar...");
    updateStyleSheet();
}

TextDisplay::~TextDisplay() {}

void TextDisplay::setText(const QString& text) {
    QLabel::setText(text);
}

void TextDisplay::appendWord(const QString& word) {
    QString current = text();
    if (current == "Ingresa una palabra para comenzar...") {
        setText(word);
    } else {
        setText(current + " " + word);
    }
}

void TextDisplay::clear() {
    setText("Ingresa una palabra para comenzar...");
}

void TextDisplay::updateStyleSheet() {
    setStyleSheet(
        "TextDisplay {"
        "  background-color: #1e1e1e;"
        "  color: #00ff88;"
        "  border: 2px solid #333;"
        "  border-radius: 8px;"
        "  padding: 15px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "}"
    );
}
