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
        NONE, // For spectators
    };
    explicit Participant(ParticipantTypeEnum pT,
                         ParticipantSideEnum pS,
                         QObject *parent = nullptr);

    bool isPlayerType(ParticipantTypeEnum pT);

protected:
    PacketTypeKeeperService* ptKeeper;
    ParticipantTypeEnum pType;
    ParticipantSideEnum pSide;

private:

signals:
    void playerQuitSignal();
    void stepInitiatedSignal(QString step);
    void undoInitiatedSignal();
    void undoApprovedSignal();

protected slots:
    virtual void gameStartedSlot() = 0;
    virtual void stepHappenedSlot() = 0;
    virtual void gameOverSlot() = 0;
    virtual void turnChangedSlot() = 0;
};

#endif // PARTICIPANT_H
