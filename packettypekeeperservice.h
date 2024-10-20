#ifndef PACKETTYPEKEEPERSERVICE_H
#define PACKETTYPEKEEPERSERVICE_H

#include <QObject>
#include <QMap>
#include <QList>
#include "participant.h"
#include "ToastTypeEnum.h"

class PacketTypeKeeperService : public QObject
{
    Q_OBJECT
public:
    enum class PacketTypeEnum {
        JOIN_NEW_SINGLE_GAME,
        CONTINUE_SINGLE_GAME,
        JOIN_MULTI_GAME,
        QUIT_GAME,
        GAME_STARTED,
        GAME_OVER,
        INITIATE_STEP,
        STEP_HAPPENED,
        UNDO_STEP_INITIATED,
        UNDO_STEP_APPROVED,
        REJECT_UNDO,
        APPROVE_UNDO,
        NOTIFICATION,
        UNKNOWN, // Need this in case some unknown string gets send from client
    };

    explicit PacketTypeKeeperService(QObject *parent = nullptr);

    QString enumToStringPacketType(PacketTypeEnum pT);
    PacketTypeEnum stringToEnumPacketType(QString pTInString);

    QString enumToStringPieceColor(Participant::ParticipantSideEnum pC);
    Participant::ParticipantSideEnum stringToEnumPieceColor(QString pCInString);

    QString enumToStringToastType(ToastTypeEnum t);
    ToastTypeEnum stringToEnumToastType(QString tInString);

    /**
    * @brief Finds out if the received packet should be handled by the server.
    *
    * @return The corresponding enum in QString if it should not be handled by the server then returns "".
    */
    QString shouldServerHandle(QString packet);
    /**
    * @brief Finds out if the received packet should be handled by the participant object.
    *
    * @return The corresponding enum in QString if it should not be handled by the participant object then returns "".
    */
    QString shouldParticipantHandle(QString packet);

    QByteArray getPacketSeparator();
private:
    QList<PacketTypeEnum> serverList;
    QList<PacketTypeEnum> participantList;

    QMap<PacketTypeEnum, QString> packetMap;
    QMap<Participant::ParticipantSideEnum, QString> pieceColorMap;
    QMap<ToastTypeEnum, QString> toastTypeMap;
    QString isServerHandledPacket(PacketTypeEnum pt);
    QString isParticipantHandledPacket(PacketTypeEnum pt);
signals:
};

#endif // PACKETTYPEKEEPERSERVICE_H
