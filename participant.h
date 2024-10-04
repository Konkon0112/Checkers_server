#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <QObject>
#include <QTcpSocket>

#include "packettypekeeperservice.h"

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
    PacketTypeKeeperService* ptKeeper;
    ParticipantTypeEnum pType;
    ParticipantSideEnum pSide;

signals:

private slots:
    void handlingReadyReadSlot();
};

#endif // PARTICIPANT_H
