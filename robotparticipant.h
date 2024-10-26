#ifndef ROBOTPARTICIPANT_H
#define ROBOTPARTICIPANT_H

#include <QObject>
#include <QTcpSocket>
#include <QList>
#include <QThreadPool>

#include "participant.h"
#include "board.h"
#include "validatorbase.h"
#include "ToastTypeEnum.h"

class RobotParticipant : public Participant
{
    Q_OBJECT
public:
    explicit RobotParticipant(ParticipantSideEnum pS , QObject *parent = nullptr);
    bool usingThisSocket(QTcpSocket* soc) override;
    void sendNotification(ToastTypeEnum tt, QString msg) override;

signals:

public slots:
    void stepHappenedSlot(QString step, Participant::ParticipantSideEnum newTurnColor) override;
    void undoNeedsApprovalSlot(Participant::ParticipantSideEnum approvingSideColor) override;
    void undoHappenedSlot(QString newStepsSoFar, Participant::ParticipantSideEnum nextC) override;

protected slots:
    void gameStartedSlot(Participant::ParticipantSideEnum nextOnTurn, QString stepsSoFar) override;

private slots:
    void stepCalculationDoneSlot(QString step);

private:
    void makeNextStep(QString lastStep = "");
    ValidatorBase* findValidator(QChar piece);
    void executeJoinedSteps(QString joinedSteps);

    Board* board;
    QList<ValidatorBase*> validators;
    QThreadPool* pool;
};

#endif // ROBOTPARTICIPANT_H
