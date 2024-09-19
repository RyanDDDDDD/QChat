#include "clickedlabel.h"
#include <QMouseEvent>
#include <QEnterEvent>

ClickedLabel::ClickedLabel(QWidget* parent)
    :QLabel(parent),
    _curState(ClickedState::Normal)
{
    setCursor(Qt::PointingHandCursor);
}

void ClickedLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if(_curState == ClickedState::Normal){
            qDebug()<<"clicked , change to selected hover: "<< _selected_hover;
            _curState = ClickedState::Selected;
            setProperty("state",_selected_hover);
            repolish(this);
            update();
        }else{
            qDebug()<<"clicked , change to normal hover: "<< _normal_hover;
            _curState = ClickedState::Normal;
            setProperty("state",_normal_hover);
            repolish(this);
            update();
        }
        emit clicked();
    }

    QLabel::mousePressEvent(event);
}

void ClickedLabel::enterEvent(QEnterEvent *event)
{
    if(_curState == ClickedState::Normal){
        qDebug()<<"enter , change to normal hover: "<< _normal_hover;
        setProperty("state",_normal_hover);
        repolish(this);
        update();
    }else{
        qDebug()<<"enter , change to selected hover: "<< _selected_hover;
        setProperty("state",_selected_hover);
        repolish(this);
        update();
    }

    QLabel::enterEvent(event);
}

void ClickedLabel::leaveEvent(QEvent *event)
{
    if(_curState == ClickedState::Normal){
        qDebug()<<"leave , change to normal : "<< _normal;
        setProperty("state",_normal);
        repolish(this);
        update();
    }else{
        qDebug()<<"leave , change to normal hover: "<< _selected;
        setProperty("state",_selected);
        repolish(this);
        update();
    }

    QLabel::leaveEvent(event);
}

void ClickedLabel::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    _normal = normal;
    _normal_hover = hover;
    _normal_press = press;
    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;

    setProperty("state",normal);

    repolish(this);
}

ClickedState ClickedLabel::GetCurState()
{
    return _curState;
}
