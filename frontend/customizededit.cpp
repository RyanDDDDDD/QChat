#include "customizededit.h"

CustomizedEdit::CustomizedEdit(QWidget* parent)
{
    connect(this, &QLineEdit::textChanged, this, &CustomizedEdit::limitTextLength);
}

void CustomizedEdit::SetMaxLength(int maxLen)
{
    _max_len = maxLen;
}


void CustomizedEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);

    emit sig_foucus_out();
}

void CustomizedEdit::limitTextLength(QString text) {
    if(_max_len <= 0){
        return;
    }

    QByteArray byteArray = text.toUtf8();

    if (byteArray.size() > _max_len) {
        byteArray = byteArray.left(_max_len);
        this->setText(QString::fromUtf8(byteArray));
    }
}
