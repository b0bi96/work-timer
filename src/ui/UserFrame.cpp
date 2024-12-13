#include "../utils/Exceptions.h"
#include "../../include/MyApp.h"
#include "../../include/AuthManager.h"
#include "../../include/DatabaseManager.h"
#include "../../include/AuthFrame.h"
#include "../../include/Toast.h"
#include "../../include/UserFrame.h"
#include "../../include/User.h"
#include <wx/timer.h>

wxBEGIN_EVENT_TABLE(UserFrame, wxFrame)
    EVT_TIMER(wxID_ANY, UserFrame::OnTimer)
        wxEND_EVENT_TABLE()

            UserFrame::UserFrame(const wxString &title, MyApp *app)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)),
      mainApp(app)
{
    wxPanel *panel = new wxPanel(this);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    // Username input
    vbox->Add(new wxStaticText(panel, wxID_ANY, "Username:"), 0, wxALL | wxCENTER, 5);
    usernameInput = new wxTextCtrl(panel, wxID_ANY);
    vbox->Add(usernameInput, 0, wxALL | wxEXPAND, 5);

    // Password input
    vbox->Add(new wxStaticText(panel, wxID_ANY, "Password:"), 0, wxALL | wxCENTER, 5);
    passwordInput = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    vbox->Add(passwordInput, 0, wxALL | wxEXPAND, 5);

    updateButton = new wxButton(panel, wxID_ANY, "Update Info");
    vbox->Add(updateButton, 0, wxALL | wxALIGN_CENTER, 5);
    updateButton->Bind(wxEVT_BUTTON, &UserFrame::OnUpdateUser, this);

    startWorkButton = new wxButton(panel, wxID_ANY, "Start Work");
    endWorkButton = new wxButton(panel, wxID_ANY, "End Work");
    vbox->Add(startWorkButton, 0, wxALL | wxALIGN_CENTER, 5);
    vbox->Add(endWorkButton, 0, wxALL | wxALIGN_CENTER, 5);
    startWorkButton->Bind(wxEVT_BUTTON, &UserFrame::OnStartWork, this);
    endWorkButton->Bind(wxEVT_BUTTON, &UserFrame::OnEndWork, this);

    wxButton *logoutButton = new wxButton(panel, wxID_ANY, "Logout");
    vbox->Add(logoutButton, 0, wxALL | wxALIGN_CENTER, 5);

    logoutButton->Bind(wxEVT_BUTTON, &UserFrame::OnLogout, this);

    workTimer = new wxTimer(this);
    timerDisplay = new wxStaticText(panel, wxID_ANY, "00:00:00", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    vbox->Add(timerDisplay, 0, wxALL | wxEXPAND, 5);

    panel->SetSizer(vbox);
    LoadUserData();
}

void UserFrame::LoadUserData()
{
    currentUser = mainApp->getCurrentUser();
    usernameInput->SetValue(currentUser.username);
    passwordInput->SetValue("");

    elapsedSeconds = static_cast<long>(currentUser.workTime);
    UpdateTimerDisplay(elapsedSeconds);
}

void UserFrame::OnUpdateUser(wxCommandEvent &event)
{
    try
    {
        currentUser.username = usernameInput->GetValue().ToStdString();
        currentUser.password = passwordInput->GetValue().ToStdString();
        mainApp->getDatabaseManager()->updateUserById(currentUser.id, currentUser.username, currentUser.password, nullptr, nullptr);
        Toast::Show(this, "Info updated successfully!", *wxGREEN);
    }
    catch (const ClientException &e)
    {
        Toast::Show(this, e.what(), *wxRED);
        std::cout << "ClientError, UserFrame::OnUpdateUser: " << e.what() << endl;
    }
    catch (const std::runtime_error &e)
    {
        Toast::Show(this, "Server error", *wxRED);
        std::cout << "Error, UserFrame::OnUpdateUser " << e.what() << endl;
    }
}

void UserFrame::OnStartWork(wxCommandEvent &event)
{
    if (!workTimer->IsRunning())
    {
        startTime = wxGetLocalTime();
        workTimer->Start(1000);
        Toast::Show(this, "Work started!", *wxGREEN);
    }
}

void UserFrame::OnEndWork(wxCommandEvent &event)
{
    if (workTimer->IsRunning())
    {
        workTimer->Stop();
        endTime = wxGetLocalTime();
        long currentSessionSeconds = endTime - startTime;
        elapsedSeconds += currentSessionSeconds;
        mainApp->getDatabaseManager()->updateWorkTime(currentUser.id, elapsedSeconds);
        UpdateTimerDisplay(elapsedSeconds);
        Toast::Show(this, "Work ended! Time saved.", *wxGREEN);
    }
}

void UserFrame::OnTimer(wxTimerEvent &event)
{
    long currentTime = wxGetLocalTime();
    long currentSessionTime = currentTime - startTime;
    long totalWorkTime = elapsedSeconds + currentSessionTime;
    UpdateTimerDisplay(totalWorkTime);
}

void UserFrame::UpdateTimerDisplay(long seconds)
{
    long hours = seconds / 3600;
    long minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;
    timerDisplay->SetLabel(wxString::Format("%02ld:%02ld:%02ld", hours, minutes, seconds));
}

void UserFrame::OnLogout(wxCommandEvent &event)
{
    try
    {
        if (workTimer->IsRunning())
        {
            workTimer->Stop();
            endTime = wxGetLocalTime();
            long currentSessionSeconds = endTime - startTime;
            elapsedSeconds += currentSessionSeconds;
            mainApp->getDatabaseManager()->updateWorkTime(currentUser.id, elapsedSeconds);
            UpdateTimerDisplay(elapsedSeconds);
            Toast::Show(this, "Work ended! Time saved.", *wxGREEN);
        }

        mainApp->getAuthManager()->logOut();
        Toast::Show(this, "Logged out successfully", *wxGREEN);

        this->Close(true);
        mainApp->getAuthFrame()->Show();
    }
    catch (const ClientException &e)
    {
        Toast::Show(this, e.what(), *wxRED);
        std::cout << "ClientError, UserFrame::OnLogout: " << e.what() << endl;
    }
    catch (const std::runtime_error &e)
    {
        Toast::Show(this, "Server error", *wxRED);
        std::cout << "Error, UserFrame::OnLogout " << e.what() << endl;
    }
}
