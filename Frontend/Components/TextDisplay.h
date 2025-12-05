#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <QLabel>
#include <QString>

class TextDisplay : public QLabel {
    Q_OBJECT

public:
    TextDisplay(QWidget* parent = nullptr);
    ~TextDisplay();

    void setText(const QString& text);
    void appendWord(const QString& word);
    void clear();

private:
    void updateStyleSheet();
};

#endif // TEXTDISPLAY_H
