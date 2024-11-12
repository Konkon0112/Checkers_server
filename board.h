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

    /**
    * @brief Execute the step passed, without validation check.
    *
    * @return The string representation of a step.
    */
    void executeStep(QString step);

    /**
    * @brief Check if the piece at index 'from' is the same color as 'pS'.
    */
    bool checkIfPlayerUsedOwnPiece(int from, Participant::ParticipantSideEnum pS);

    /**
    * @brief Reset the board to starting position.
    */
    void restartBoard();

    /**
    * @brief Returns the character at the given index.
    */
    QChar charAtIndex(int ind);

    /**
    * @brief Returns the string representation of the whole board.
    */
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
