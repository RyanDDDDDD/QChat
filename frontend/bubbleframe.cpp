#include "bubbleframe.h"

#include <QPainter>

const int WIDTH_TRAIANGLE = 8;

BubbleFrame::BubbleFrame(ChatRole role, QWidget *parent)
    :QFrame(parent)
    ,m_role(role)
    ,m_margin(3)
{
    m_pHLayout = new QHBoxLayout();
    if(m_role == ChatRole::Self)
        m_pHLayout->setContentsMargins(m_margin, m_margin, WIDTH_TRAIANGLE + m_margin, m_margin);
    else
        m_pHLayout->setContentsMargins(WIDTH_TRAIANGLE + m_margin, m_margin, m_margin, m_margin);

    this->setLayout(m_pHLayout);
}

void BubbleFrame::setMargin(int margin)
{

}

void BubbleFrame::setWidget(QWidget *w)
{
    if(m_pHLayout->count() > 0)
        return ;
    else{
        m_pHLayout->addWidget(w);
    }
}

void BubbleFrame::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    if(m_role == ChatRole::Other)
    {
        QColor bk_color(Qt::white);
        painter.setBrush(QBrush(bk_color));
        QRect bk_rect = QRect(WIDTH_TRAIANGLE, 0, this->width()-WIDTH_TRAIANGLE, this->height());
        painter.drawRoundedRect(bk_rect,5,5);
        QPointF points[3] = {
            QPointF(bk_rect.x(), 12),
            QPointF(bk_rect.x(), 10+WIDTH_TRAIANGLE +2),
            QPointF(bk_rect.x()-WIDTH_TRAIANGLE, 10+WIDTH_TRAIANGLE-WIDTH_TRAIANGLE/2),
        };
        painter.drawPolygon(points, 3);
    }
    else
    {
        QColor bk_color(158,234,106);
        painter.setBrush(QBrush(bk_color));
        QRect bk_rect = QRect(0, 0, this->width()-WIDTH_TRAIANGLE, this->height());
        painter.drawRoundedRect(bk_rect,5,5);
        QPointF points[3] = {
            QPointF(bk_rect.x()+bk_rect.width(), 12),
            QPointF(bk_rect.x()+bk_rect.width(), 12+WIDTH_TRAIANGLE +2),
            QPointF(bk_rect.x()+bk_rect.width()+WIDTH_TRAIANGLE, 10+WIDTH_TRAIANGLE-WIDTH_TRAIANGLE/2),
        };
        painter.drawPolygon(points, 3);

    }

    return QFrame::paintEvent(e);
}
