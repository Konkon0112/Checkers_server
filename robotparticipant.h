#ifndef ROBOTPARTICIPANT_H
#define ROBOTPARTICIPANT_H

#include <QObject>
#include <QTcpSocket>
#include <QList>

#include "participant.h"
#include "board.h"
#include "validatorbase.h"

class RobotParticipant : public Participant
{
    Q_OBJECT
public:
    explicit RobotParticipant(ParticipantSideEnum pS , QObject *parent = nullptr);
    bool usingThisSocket(QTcpSocket* soc) override;

signals:

public slots:
    void stepHappenedSlot(QString step) override;
    void turnChangedSlot(Participant::ParticipantSideEnum nextOnTurn) override;
    void undoNeedsApprovalSlot(Participant::ParticipantSideEnum approvingSideColor) override;
    void undoHappenedSlot(QString newStepsSoFar) override;

protected slots:
    void gameStartedSlot(Participant::ParticipantSideEnum nextOnTurn, QString stepsSoFar) override;

private:
    void makeNextStep();
    ValidatorBase* findValidator(QChar piece);
    void executeJoinedSteps(QString joinedSteps);

    Board* board;
    QList<ValidatorBase*> validators;
    QString lastStepMade = "";
};

#endif // ROBOTPARTICIPANT_H
