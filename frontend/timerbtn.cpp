#include "timerbtn.h"
#include <QMouseEvent>

TimerBtn::TimerBtn(QWidget *parent)
    :QPushButton(parent)
    ,_counter(10)
{
    _timer = new QTimer(this);

    connect(_timer, &QTimer::timeout, [this]{
        _counter--;
        if (_counter <= 0) {
            _timer->stop();
            _counter = 10;
            this->setText("get code");
            this->setEnabled(true);
            return;
        }

        this->setText(QString::number(_counter));
    });
}

TimerBtn::~TimerBtn()
{
    _timer->stop();
}

void TimerBtn::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        this->setEnabled(false);
        this->setText(QString::number(_counter));
        _timer->start(1000);
        emit clicked();
    }

    QPushButton::mouseReleaseEvent(e);
}