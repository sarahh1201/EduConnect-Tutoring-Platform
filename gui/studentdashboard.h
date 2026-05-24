#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H

#include <QDialog>
#include "backend/Decode.h"
#include <string>
#include <vector>
using std::string;

namespace Ui {
class studentdashboard;
}

class studentdashboard : public QDialog
{
    Q_OBJECT

public:
    explicit studentdashboard(const string& username, QWidget *parent = nullptr);
    QString joinSubjects(const vector<string>& subjects);
    void ViewSessions(const string &studentUsername);
    ~studentdashboard();

private slots:
    void on_logout_btn_clicked();
    void on_searchtutor_btn_clicked();
    void on_createreq_btn_clicked();
    void on_endsession_btn_clicked();
    void on_refresh_btn_clicked();
    void on_ratetutor_btn_clicked();

private:
    Ui::studentdashboard *ui;
    string username;
};

#endif // STUDENTDASHBOARD_H
