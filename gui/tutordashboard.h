#ifndef TUTORDASHBOARD_H
#define TUTORDASHBOARD_H

#include <QDialog>
#include "backend/Decode.h"
#include <string>
#include <vector>
using std::string;

namespace Ui {
class tutordashboard;
}

class tutordashboard : public QDialog
{
    Q_OBJECT

public:
    explicit tutordashboard(const string& username, QWidget *parent = nullptr);
    void ViewRequests(const string &tutorUsername);
    bool tutorTeaches(const Tutor &t, const string &subject);
    void AcceptRequest(const string &tutorUsername);
    void UpdateRequestStatus(int reqID, const string &tutorUsername);
    void ViewRatings(const string &tutorUsername);
    void ViewSessions(const string &tutorUsername);
    ~tutordashboard();

private slots:
    void on_refresh_btn_clicked();
    void on_logout_btn_clicked();
    void on_acceptreq_btn_clicked();
    void on_endsession_btn_clicked();

private:
    Ui::tutordashboard *ui;
    string username;
};

#endif // TUTORDASHBOARD_H
