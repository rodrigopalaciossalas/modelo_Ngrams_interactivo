#ifndef ACTIONBAR_H
#define ACTIONBAR_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

class ActionBar : public QWidget {
    Q_OBJECT

public:
    ActionBar(QWidget* parent = nullptr);
    ~ActionBar();

    QPushButton* getBackButton() const { return m_backBtn; }
    QPushButton* getRestartButton() const { return m_restartBtn; }

signals:
    void backClicked();
    void restartClicked();

private:
    QPushButton* m_backBtn;
    QPushButton* m_restartBtn;
    void setupButtons();
    void updateStyleSheet();
};

#endif // ACTIONBAR_H
