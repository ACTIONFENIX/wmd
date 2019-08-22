#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <string>
#include <QMessageBox>

enum class ErrorType
{
    Normal,
    Critical
};

//Shows error message when constructor is called
class ErrorHandler
{
public:
    ErrorHandler(const std::string& err_text, ErrorType err_type = ErrorType::Normal);
};

#endif // ERRORHANDLER_H
