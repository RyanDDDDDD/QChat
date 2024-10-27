#include "tcpmgr.h"
#include <QAbstractSocket>
#include <QJsonDocument>
// #include "usermgr.h"

TcpMgr::~TcpMgr()
{

}

TcpMgr::TcpMgr()
    :_host(""),_port(0),_b_recv_pending(false),
    _message_id(0), _message_len(0)
{
    QObject::connect(&_socket, &QTcpSocket::connected, [&]{
        qDebug() << "Connected to server!";
        emit sig_con_success(true);
    });

    QObject::connect(&_socket, &QTcpSocket::readyRead, [&]{
        _buffer.append(_socket.readAll());

        QDataStream stream(&_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_5_0);

        forever {
            //check if it can retrieve header from buffer
            if (!_b_recv_pending) {
                if (_buffer.size() < static_cast<int>(sizeof(quint16) * 2))
                    return;

                //fetch message id and message len from buffer
                stream >> _message_id >> _message_len;

                _buffer = _buffer.mid(sizeof(quint16) * 2);
                qDebug() << "Message ID: " << _message_id << ", Length:" << _message_len;
            }

            if (_buffer.size() < _message_len) {
                _b_recv_pending = true;
                return;
            }

            _b_recv_pending = false;
            //read message body
            QByteArray messageBody = _buffer.mid(0, _message_len);
            qDebug() << "Receive body msg id " << messageBody;

            _buffer = _buffer.mid(_message_len);
            handleMsg(ReqId(_message_id), _message_id, messageBody);
        }
    });

    QObject::connect(&_socket,QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
                     [&](QTcpSocket::SocketError socketError){
        qDebug() << "Error: " << _socket.errorString();
        switch (socketError) {
            case QTcpSocket::ConnectionRefusedError:
                qDebug() << "Connection Refuesd!";
                emit sig_con_success(false);
                break;
            case QTcpSocket::RemoteHostClosedError:
                qDebug() << "Remote Host Closed Connection!";
                break;
            case QTcpSocket::HostNotFoundError:
                qDebug() << "Host Not Found!";
                emit sig_con_success(false);
                break;
            case QTcpSocket::SocketTimeoutError:
                qDebug() << "Connection Timeout!";
                emit sig_con_success(false);
                break;
            case QTcpSocket::NetworkError:
                qDebug() << "Network Error!";
                break;
            default:
                qDebug() << "Other Error!";
                break;
        }
    });

    QObject::connect(&_socket, &QTcpSocket::disconnected, [&]{
        qDebug() << "Disconnected from server.";
    });

    QObject::connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);

    initHandler();
}

void TcpMgr::initHandler()
{
    _handlers.insert(ID_CHAT_LOGIN_RSP, [this](ReqId id, int len, QByteArray data){
        qDebug() << "handle id is " << id << " data is " << data;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        if (jsonDoc.isNull()){
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Login Failed, err is Json Parse Err" << err;
            emit sig_login_failed(err);
            return;
        }

        // UserMgr::GetInstance()->SetUid(jsonObj["uid"].toInt());
        // UserMgr::GetInstance()->SetName(jsonObj["name"].toString());
        // UserMgr::GetInstance()->SetToken(jsonObj["token"].toString());

        emit sig_switch_chatdlg();
    });
}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    auto find_iter = _handlers.find(id);
    if (find_iter == _handlers.end()){
        qDebug() << "not found id ["<< id << "] to handle";
        return;
    }

    find_iter.value()(id, len, data);
}

void TcpMgr::slot_tcp_connect(ServerInfo si)
{
    qDebug() << "Receive tcp connect signal";
    qDebug() << "Connecting to server...";

    _host = si.Host;
    _port = static_cast<uint16_t>(si.Port.toUInt());
    _socket.connectToHost(_host, _port);
}

void TcpMgr::slot_send_data(ReqId reqId, QString data)
{
    uint16_t id = reqId;

    QByteArray dataBytes = data.toUtf8();

    quint16 len = static_cast<quint16>(data.size());

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setByteOrder(QDataStream::BigEndian);

    out << id << len;

    block.append(dataBytes);

    _socket.write(block);
}


