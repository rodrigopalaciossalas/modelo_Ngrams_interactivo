#ifndef OPTIONSLIST_H
#define OPTIONSLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include "PredictionButton.h"

class OptionsList : public QWidget {
    Q_OBJECT

public:
    OptionsList(QWidget* parent = nullptr);
    ~OptionsList();

    void setOptions(const QStringList& words, const QList<int>& frequencies);
    void clearOptions();
    QString getSelectedOption() const { return m_selectedOption; }

signals:
    void optionSelected(const QString& word, int frequency, int index);
    void moreOptions();

private slots:
    void onOptionClicked(const QString& text, int frequency);

private:
    QVBoxLayout* m_layout;
    QList<PredictionButton*> m_buttons;
    QString m_selectedOption;
};

#endif // OPTIONSLIST_H
