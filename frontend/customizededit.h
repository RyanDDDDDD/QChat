#ifndef CUSTOMIZEDEDIT_H
#define CUSTOMIZEDEDIT_H

#include <QLineEdit>

class CustomizedEdit : public QLineEdit
{
    Q_OBJECT
public:
    CustomizedEdit(QWidget* parent = nullptr);
    void SetMaxLength(int maxLen);

protected:
    void focusOutEvent(QFocusEvent* event) override;

private:
    void limitTextLength(QString text);

signals:
    void sig_foucus_out();

private:
    int _max_len;

};

#endif // CUSTOMIZEDEDIT_H
