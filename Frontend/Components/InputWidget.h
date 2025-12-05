#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

class InputWidget : public QWidget {
    Q_OBJECT

public:
    InputWidget(QWidget* parent = nullptr);
    ~InputWidget();

    QString getText() const;
    void clear();

signals:
    void addWord(const QString& word);

private slots:
    void onAddClicked();
    void onReturnPressed();

private:
    QLineEdit* m_input;
    QPushButton* m_addBtn;
    void updateStyleSheet();
};

#endif // INPUTWIDGET_H
