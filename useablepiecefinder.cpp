#include "useablepiecefinder.h"
#include "validatordame.h"
#include "validatorpawn.h"

UseablePieceFinder::UseablePieceFinder(QObject *parent)
    : QObject{parent}
{
    validators.append(new ValidatorPawn(this));
    validators.append(new ValidatorDame(this));
}

QSet<int> UseablePieceFinder::findUseablePieceIndecies(QString board, Participant::ParticipantSideEnum userPiece)
{
    QSet<int> piecesThatCanTake;
    QSet<int> piecesThatCannotTake;
    for(int i = 0; i < board.length(); i++){
        QChar ch = board[i];
        if((ch == 'p' || ch == 'd') &&
            userPiece == Participant::ParticipantSideEnum::DARK){
            ValidatorBase* validator = findResponsibleValidator(board[i]);
            QSet<QString> steps = validator->getValidIndecies(i, board);
            if(steps.isEmpty()) continue;
            bool canTake = containsStepThatCanTake(steps);

            if(canTake){
                piecesThatCanTake.insert(i);
            } else {
                piecesThatCannotTake.insert(i);
            }

        } else if((ch == 'P' || ch == 'D') &&
                   userPiece == Participant::ParticipantSideEnum::LIGHT){
            ValidatorBase* validator = findResponsibleValidator(board[i]);
            QSet<QString> steps = validator->getValidIndecies(i, board);
            if(steps.isEmpty()) continue;

            bool canTake = containsStepThatCanTake(steps);

            if(canTake){
                piecesThatCanTake.insert(i);
            } else {
                piecesThatCannotTake.insert(i);
            }
        }
    }
    if(piecesThatCanTake.isEmpty()){
        return piecesThatCannotTake;
    } else {
        return piecesThatCanTake;
    }
}

ValidatorBase *UseablePieceFinder::findResponsibleValidator(QChar ch)
{
    ValidatorBase* res;
    for(int i = 0; i < validators.length(); i++){
        if(validators.at(i)->isValidatorsResponsibility(ch)) {
            res = validators[i];
        }
    }
    return res;
}

bool UseablePieceFinder::containsStepThatCanTake(QSet<QString> validSteps)
{
    if(validSteps.isEmpty()) return false;
    QString step = *validSteps.cbegin();

    // It is not empty for sure,
    // If a piece can take it's set will only contain the taking steps.
    // That's why it is enough to check the first one.
    return step.contains('x');
}
