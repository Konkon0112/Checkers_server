#ifndef PACKETTYPEKEEPERSERVICE_H
#define PACKETTYPEKEEPERSERVICE_H

#include <QObject>
#include <QMap>
#include <QList>

class PacketTypeKeeperService : public QObject
{
    Q_OBJECT
public:
    enum class PacketTypeEnum {
        JOIN_NEW_SINGLE_GAME,
        CONTINUE_SINGLE_GAME,
        JOIN_MULTI_GAME,
        SEND_STEPS_SO_FAR,
        QUIT_GAME,
        GAME_OVER,
        INITIATE_STEP,
        STEP_HAPPENED,
        UNDO_STEP_INITIATED,
        UNDO_STEP_APPROVED,
    };

    explicit PacketTypeKeeperService(QObject *parent = nullptr);

    QString enumToString(PacketTypeEnum pT);
    PacketTypeEnum stringToEnum(QString pTInString);

    QString shouldServerHandle(QString packet);
    QString shouldParticipantHandle(QString packet);
private:
    QList<PacketTypeEnum> serverList;
    QList<PacketTypeEnum> participantList;

    QMap<PacketTypeEnum, QString> packetMap;
    QString isServerHandledPacket(PacketTypeEnum pt);
    QString isParticipantHandledPacket(PacketTypeEnum pt);
signals:
};

#endif // PACKETTYPEKEEPERSERVICE_H
