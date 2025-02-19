#include "statelabel.h"

StateLabel::StateLabel(QWidget* parent):QLabel(parent),_curstate(ClickedState::Normal)
{
 setCursor(Qt::PointingHandCursor);
}

void StateLabel::mousePressEvent(QMouseEvent* event)  {
    if (event->button() == Qt::LeftButton) {
        if(_curstate == ClickedState::Selected){
            qDebug()<<"PressEvent , already to selected press: "<< _selected_press;
            //emit clicked();
            QLabel::mousePressEvent(event);
            return;
        }

        if(_curstate == ClickedState::Normal){
            qDebug()<<"PressEvent , change to selected press: "<< _selected_press;
            _curstate = ClickedState::Selected;
            setProperty("state",_selected_press);
            repolish(this);
            update();
        }

        return;
    }

    QLabel::mousePressEvent(event);
}

void StateLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if(_curstate == ClickedState::Normal){
            //qDebug()<<"ReleaseEvent , change to normal hover: "<< _normal_hover;
            setProperty("state",_normal_hover);
            repolish(this);
            update();

        }else{
            //qDebug()<<"ReleaseEvent , change to select hover: "<< _selected_hover;
            setProperty("state",_selected_hover);
            repolish(this);
            update();
        }
        emit clicked();
        return;
    }

    QLabel::mousePressEvent(event);
}

void StateLabel::enterEvent(QEnterEvent* event) {
    if(_curstate == ClickedState::Normal){
         //qDebug()<<"enter , change to normal hover: "<< _normal_hover;
        setProperty("state",_normal_hover);
        repolish(this);
        update();

    }else{
         //qDebug()<<"enter , change to selected hover: "<< _selected_hover;
        setProperty("state",_selected_hover);
        repolish(this);
        update();
    }

    QLabel::enterEvent(event);
}

void StateLabel::leaveEvent(QEvent* event){
    if(_curstate == ClickedState::Normal){
        // qDebug()<<"leave , change to normal : "<< _normal;
        setProperty("state",_normal);
        repolish(this);
        update();

    }else{
        // qDebug()<<"leave , change to select normal : "<< _selected;
        setProperty("state",_selected);
        repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}

void StateLabel::SetState(QString normal, QString hover, QString press,
                            QString select, QString select_hover, QString select_press)
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

ClickedState StateLabel::GetCurState(){
    return _curstate;
}

void StateLabel::ClearState()
{
    _curstate = ClickedState::Normal;
    setProperty("state",_normal);
    repolish(this);
    update();
}

void StateLabel::SetSelected(bool bselected)
{
    if(bselected){
        _curstate = ClickedState::Selected;
        setProperty("state",_selected);
        repolish(this);
        update();
        return;
    }

    _curstate = ClickedState::Normal;
    setProperty("state",_normal);
    repolish(this);
    update();
    return;

}



