#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <QObject>
#include <QTcpSocket>


class PacketTypeKeeperService;
class Participant : public QObject
{
    Q_OBJECT
public:
    enum class ParticipantTypeEnum {
        PLAYER,
        SPECTATOR,
    };
    enum class ParticipantSideEnum {
        DARK,
        LIGHT,
        NONE,
    };
    explicit Participant(ParticipantTypeEnum pT,
                         ParticipantSideEnum pS,
                         QObject *parent = nullptr);

    bool isPlayerType(ParticipantTypeEnum pT);

private:
    PacketTypeKeeperService* ptKeeper;
    ParticipantTypeEnum pType;
    ParticipantSideEnum pSide;

signals:

private slots:
    void handlingReadyReadSlot();
};

#endif // PARTICIPANT_H
