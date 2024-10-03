#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <QObject>
#include <QTcpSocket>

class Participant : public QObject
{
    Q_OBJECT
public:
    enum ParticipantTypeEnum {
        PLAYER,
        SPECTATOR,
    };
    enum ParticipantSideEnum {
        DARK,
        LIGHT,
        NONE,
    };
    explicit Participant(QTcpSocket* s,
                         ParticipantTypeEnum pT,
                         ParticipantSideEnum pS,
                         QObject *parent = nullptr);

private:
    QTcpSocket* socket;
    ParticipantTypeEnum pType;
    ParticipantSideEnum pSide;

signals:
};

#endif // PARTICIPANT_H
