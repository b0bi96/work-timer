#ifndef USER_FRAME_H
#define USER_FRAME_H

#include <wx/wx.h>
#include <wx/timer.h>
#include "MyApp.h"
#include "User.h"

class UserFrame : public wxFrame
{
private:
    MyApp *mainApp;
    User currentUser;
    wxTextCtrl *usernameInput;
    wxTextCtrl *passwordInput;
    wxStaticText *timerDisplay;
    wxButton *updateButton;
    wxButton *startWorkButton;
    wxButton *endWorkButton;
    wxTimer *workTimer;
    long startTime;
    long endTime;
    long elapsedSeconds = 0;

    void OnUpdateUser(wxCommandEvent &event);
    void OnStartWork(wxCommandEvent &event);
    void OnEndWork(wxCommandEvent &event);
    void OnTimer(wxTimerEvent &event);
    void OnLogout(wxCommandEvent &event);
    void UpdateTimerDisplay(long seconds = 0);

public:
    UserFrame(const wxString &title, MyApp *app);
    void LoadUserData();

    wxDECLARE_EVENT_TABLE();
};

#endif
