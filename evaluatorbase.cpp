#include "evaluatorbase.h"

EvaluatorBase::EvaluatorBase(QChar lightCh, QChar darkCh, float baseV, QObject *parent)
    : QObject{parent}
{
    lightChar = lightCh;
    darkChar = darkCh;
    baseVal = baseV;
}

bool EvaluatorBase::isEvaluatorResponsibility(QChar ch)
{
    return ch == lightChar || ch == darkChar;
}
