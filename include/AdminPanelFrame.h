#ifndef ADMINPANELFRAME_H
#define ADMINPANELFRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>

class MyApp;

class AdminPanelFrame : public wxFrame
{
public:
    AdminPanelFrame(const wxString &title, MyApp *mainApp);

private:
    MyApp *mainApp;

    wxListCtrl *userList;
    wxTextCtrl *idInput;
    wxTextCtrl *usernameInput;
    wxTextCtrl *passwordInput;
    wxChoice *roleInput;
    wxStaticText *statusText;

    void LoadUsers();

    void OnUpdateUser(wxCommandEvent &event);
    void OnRemoveUser(wxCommandEvent &event);
    void OnLogout(wxCommandEvent &event);
    void onRemoveUserWorkTime(wxCommandEvent &event);
};

#endif
