#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QDialog>
#include "backend/Decode.h"
#include "ui_admindashboard.h"

namespace Ui {
class admindashboard;
}

class admindashboard : public QDialog
{
    Q_OBJECT

public:
    explicit admindashboard(QWidget *parent = nullptr);
    void ViewSessions();
    void ViewRequests();
    void ViewTutors();
    void ViewStudents();

    ~admindashboard();

private slots:
    void on_logout_btn_clicked();
    void on_refresh_btn_clicked();

private:
    Ui::admindashboard *ui;
};

#endif // ADMINDASHBOARD_H
