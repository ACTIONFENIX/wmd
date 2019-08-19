#include "errorhandler.h"
#include <QApplication>

ErrorHandler::ErrorHandler(const std::string &err_text, ErrorType err_type)
{
    QMessageBox msg_box;
    msg_box.setWindowTitle("Error");
    msg_box.setText(err_text.c_str());
    msg_box.exec();
    if (err_type == ErrorType::Critical)
    {
        QApplication::quit();
    }
}
