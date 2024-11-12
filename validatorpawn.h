#ifndef VALIDATORPAWN_H
#define VALIDATORPAWN_H

#include <QObject>

#include "validatorbase.h"

class ValidatorPawn : public ValidatorBase
{
    Q_OBJECT
public:
    explicit ValidatorPawn(QObject *parent = nullptr);
    QSet<QString> getValidSteps(int startInd, QString board) override;

private:

signals:
};

#endif // VALIDATORPAWN_H
