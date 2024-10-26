#ifndef CUSTOMEXCEPTION_H
#define CUSTOMEXCEPTION_H

#include <QException>

#include "ToastTypeEnum.h"

class CustomException : public QException
{
public:
    explicit CustomException(QString m, ToastTypeEnum tt);
    void raise() const;
    QException *clone() const;
    const char *what() const noexcept;

    QString getMessage() const;
    ToastTypeEnum getToastType() const;

signals:

private:
    QString message;
    ToastTypeEnum toastType;
};

#endif // CUSTOMEXCEPTION_H
