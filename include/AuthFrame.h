#ifndef AUTHFRAME_H
#define AUTHFRAME_H

#include <wx/wx.h>
#include "AdminPanelFrame.h"

class MyApp;
class AuthFrame : public wxFrame
{
public:
    AuthFrame(const wxString &title, MyApp *mainApp);

private:
    MyApp *mainApp;
    wxTextCtrl *usernameInput;
    wxTextCtrl *passwordInput;
    wxStaticText *statusText;

    void OnLogin(wxCommandEvent &event);
    void OnRegister(wxCommandEvent &event);
};

#endif
