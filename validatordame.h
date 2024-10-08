#ifndef VALIDATORDAME_H
#define VALIDATORDAME_H

#include <QObject>
#include "validatorbase.h"

class ValidatorDame : public ValidatorBase
{
    Q_OBJECT
public:
    explicit ValidatorDame(QObject *parent = nullptr);

private:
    QSet<QString> getValidIndecies(int startInd, QString board) override;

signals:
};

#endif // VALIDATORDAME_H
