#ifndef PREDICTIONBUTTON_H
#define PREDICTIONBUTTON_H

#include <QPushButton>
#include <QString>

class PredictionButton : public QPushButton {
    Q_OBJECT

public:
    PredictionButton(const QString& text, int frequency, QWidget* parent = nullptr);
    ~PredictionButton();

    int getFrequency() const { return m_frequency; }
    void setSelected(bool selected);
    bool isSelected() const { return m_selected; }

signals:
    void clicked(const QString& text, int frequency);

private slots:
    void onClicked();

private:
    int m_frequency;
    bool m_selected;
    void updateStyleSheet();
};

#endif // PREDICTIONBUTTON_H
