#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QScrollBar>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QTimer>

class ChatView : public QWidget
{
    Q_OBJECT
public:
    ChatView(QWidget* parent = nullptr);
    void appendChatItem(QWidget* item);
    void prependChatItem(QWidget* item);
    void insertChatItem(QWidget* before, QWidget* item);
    void removeAllItem();

protected:
    bool eventFilter(QObject* o, QEvent* e) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void initStyleSheet();

private slots:
    void onVScrollBarMoved(int min, int max);

private:
    QVBoxLayout* m_pVl;
    QScrollArea* m_pScrollArea;
    bool isAppended;
};

#endif // CHATVIEW_H
