#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QChar>

#include "participant.h"

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(QObject *parent = nullptr);

    void executeStep(QString step);
    bool checkIfPlayerUsedOwnPiece(int from, Participant::ParticipantSideEnum pS);
    void restartBoard();
    QChar charAtIndex(int ind);

    QString getActiveBoard() const;

private:
    void checkIfDameCreated(int targetInd, QChar currentChar);
    QString activeBoard;
    const QString initialBoard =
        "xpxpxpxp"
        "pxpxpxpx"
        "xpxpxpxp"
        "xxxxxxxx"
        "xxxxxxxx"
        "PxPxPxPx"
        "xPxPxPxP"
        "PxPxPxPx";

signals:
};

#endif // BOARD_H
