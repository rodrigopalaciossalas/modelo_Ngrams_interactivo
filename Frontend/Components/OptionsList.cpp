#include "OptionsList.h"

OptionsList::OptionsList(QWidget* parent)
    : QWidget(parent) {
    
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(10);
    m_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);
    
    setStyleSheet(
        "OptionsList {"
        "  background-color: transparent;"
        "}"
    );
}

OptionsList::~OptionsList() {
    clearOptions();
}

void OptionsList::setOptions(const QStringList& words, const QList<int>& frequencies) {
    clearOptions();
    
    for (int i = 0; i < words.size() && i < frequencies.size(); ++i) {
        PredictionButton* btn = new PredictionButton(words[i], frequencies[i], this);
        m_buttons.append(btn);
        m_layout->addWidget(btn);
        
        connect(btn, &PredictionButton::clicked, this, [this, i, btn](const QString& text, int freq) {
            for (auto b : m_buttons) {
                if (b != btn) b->setSelected(false);
            }
            btn->setSelected(true);
            m_selectedOption = text;
            emit optionSelected(text, freq, i);
        });
    }
    
    m_layout->addStretch();
}

void OptionsList::clearOptions() {
    while (!m_buttons.isEmpty()) {
        PredictionButton* btn = m_buttons.takeFirst();
        m_layout->removeWidget(btn);
        delete btn;
    }
    m_selectedOption.clear();
}

void OptionsList::onOptionClicked(const QString& text, int frequency) {
    // Determine which button emitted the signal
    PredictionButton* btn = qobject_cast<PredictionButton*>(sender());
    int idx = -1;
    if (btn) {
        idx = m_buttons.indexOf(btn);
        // Mark selected state
        for (auto b : m_buttons) {
            if (b != btn) b->setSelected(false);
        }
        btn->setSelected(true);
    }

    m_selectedOption = text;
    emit optionSelected(text, frequency, idx);
}
