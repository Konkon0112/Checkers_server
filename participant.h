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
    bool isPlayerSide(ParticipantSideEnum pC);

    ParticipantTypeEnum getPType() const;

    void setPSide(ParticipantSideEnum newPSide);

    ParticipantSideEnum getPSide() const;

    virtual bool usingThisSocket(QTcpSocket* soc) = 0;

protected:
    PacketTypeKeeperService* ptKeeper;
    ParticipantTypeEnum pType;
    ParticipantSideEnum pSide;

private:

signals:
    void playerQuitSignal();
    void stepInitiatedSignal(QString step);
    void approveUndoSignal();

public slots:

    virtual void gameStartedSlot(Participant::ParticipantSideEnum nextOnTurn, QString stepsSoFar) = 0;

    virtual void stepHappenedSlot(QString step) = 0;

    virtual void undoApprovedSlot() = 0;

    virtual void turnChangedSignal(Participant::ParticipantSideEnum nextOnTurn) = 0;

protected slots:



    //[packet type]#[color on turn]
    virtual void turnChangedSlot() = 0;
};

#endif // PARTICIPANT_H
