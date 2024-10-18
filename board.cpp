#include <QtMath>
#include <QString>
#include "board.h"

Board::Board(QObject *parent)
    : QObject{parent}
{
    restartBoard();
}

void Board::executeStep(QString step)
{
    if(step == "") return;
    QStringList stepDissasembled = step.split('-');
    if(stepDissasembled.length() == 1){
        stepDissasembled = step.split('x');
    }

    int from = stepDissasembled[0].toInt();
    int to = stepDissasembled[1].toInt();
    if(step.contains("x")){
        int sX = from / 8;
        int sY = from % 8;
        int tX = to / 8;
        int tY = to % 8;

        int dX = sX - tX;
        int dY = sY - tY;

        dX = dX / qFabs<int>(dX);
        dY = dY / qFabs<int>(dY);

        tX = tX + dX;
        tY = tY + dY;

        int tIndInStr = tX * 8 + tY;

        activeBoard.replace(tIndInStr, 1, 'x');
    }
    activeBoard.replace(to, 1, activeBoard.at(from));
    activeBoard.replace(from, 1, 'x');
    checkIfDameCreated(to, charAtIndex(to));
}

bool Board::checkIfPlayerUsedOwnPiece(int from, Participant::ParticipantSideEnum pS)
{
    if(pS == Participant::ParticipantSideEnum::DARK){
        return activeBoard.at(from).isLower();
    } else {
       return activeBoard.at(from).isUpper();
    }
}

void Board::restartBoard()
{
    activeBoard = initialBoard.mid(0, initialBoard.size());
}

QChar Board::charAtIndex(int ind)
{
    return activeBoard.at(ind);
}

QString Board::getActiveBoard() const
{
    return activeBoard;
}

void Board::checkIfDameCreated(int targetInd, QChar currentChar)
{
    if(targetInd >= 56 && currentChar == 'p'){
        activeBoard.replace(targetInd, 1, 'd');
    } else if(targetInd <= 7 && currentChar == 'P'){
        activeBoard.replace(targetInd, 1, 'D');
    }
}
