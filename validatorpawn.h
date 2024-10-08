#ifndef VALIDATORPAWN_H
#define VALIDATORPAWN_H

#include <QObject>

#include "validatorbase.h"

class ValidatorPawn : public ValidatorBase
{
    Q_OBJECT
public:
    explicit ValidatorPawn(QObject *parent = nullptr);

private:
    QSet<QString> getValidIndecies(int startInd, QString board) override;

signals:
};

#endif // VALIDATORPAWN_H
