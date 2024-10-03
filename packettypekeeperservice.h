#ifndef PACKETTYPEKEEPERSERVICE_H
#define PACKETTYPEKEEPERSERVICE_H

#include <QObject>
#include <QMap>

class PacketTypeKeeperService : public QObject
{
    Q_OBJECT
public:
    enum class PacketTypeEnum {
        JOIN_NEW_SINGLE_GAME,
        CONTINUE_SINGLE_GAME,
        JOIN_MULTI_GAME,
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

private:
    QMap<PacketTypeEnum, QString> packetMap;

signals:
};

#endif // PACKETTYPEKEEPERSERVICE_H
