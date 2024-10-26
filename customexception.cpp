#include "customexception.h"

CustomException::CustomException(QString m, ToastTypeEnum tt)
{
    message = m;
    toastType = tt;
}

void CustomException::raise() const
{
    throw *this;
}

QException *CustomException::clone() const
{
    return new CustomException(message, toastType);
}

QString CustomException::getMessage() const
{
    return message;
}

ToastTypeEnum CustomException::getToastType() const
{
    return toastType;
}

const char *CustomException::what() const noexcept
{
    return message.toStdString().c_str();
}
