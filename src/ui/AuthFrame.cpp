#include "../../include/AuthFrame.h"
#include "../../include/UserFrame.h"
#include "../../include/AdminPanelFrame.h"
#include "../../include/MyApp.h"
#include "../../include/AuthManager.h"
#include "../../include/User.h"
#include "../../include/Toast.h"
#include "../utils/Exceptions.h"

AuthFrame::AuthFrame(const wxString &title, MyApp *mainApp)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)),
      mainApp(mainApp)
{
    wxPanel *panel = new wxPanel(this);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    wxStaticText *usernameLabel = new wxStaticText(panel, wxID_ANY, "Username:");
    usernameInput = new wxTextCtrl(panel, wxID_ANY);

    wxStaticText *passwordLabel = new wxStaticText(panel, wxID_ANY, "Password:");
    passwordInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);

    wxButton *loginButton = new wxButton(panel, wxID_ANY, "Login");
    wxButton *registerButton = new wxButton(panel, wxID_ANY, "Register");

    vbox->Add(usernameLabel, 0, wxALL | wxEXPAND, 5);
    vbox->Add(usernameInput, 0, wxALL | wxEXPAND, 5);
    vbox->Add(passwordLabel, 0, wxALL | wxEXPAND, 5);
    vbox->Add(passwordInput, 0, wxALL | wxEXPAND, 5);
    vbox->Add(loginButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
    vbox->Add(registerButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

    panel->SetSizer(vbox);

    loginButton->Bind(wxEVT_BUTTON, &AuthFrame::OnLogin, this);
    registerButton->Bind(wxEVT_BUTTON, &AuthFrame::OnRegister, this);
}

void AuthFrame::OnLogin(wxCommandEvent &event)
{
    std::string username = usernameInput->GetValue().ToStdString();
    std::string password = passwordInput->GetValue().ToStdString();
    AuthManager *authManager = mainApp->getAuthManager();

    try
    {
        User user = authManager->loginUser(username, password);
        if (user.role == UserRole::Owner)
        {
            try
            {
                cout << "Opening admin panel..." << endl;
                mainApp->getAdminPanelFrame()->Show();
            }
            catch (const std::exception &e)
            {
                cout << "Error while Opening admin panel" << e.what() << endl;
            }
            Close();
            return;
        }

        mainApp->getUserFrame()->Show();
        mainApp->getUserFrame()->LoadUserData();

        Close();
    }
    catch (const ClientException &e)
    {
        Toast::Show(this, e.what(), *wxRED);
        std::cout << "ClientError, cannot login user: " << e.what() << endl;
    }
    catch (const std::runtime_error &e)
    {
        Toast::Show(this, "Server error", *wxRED);
        std::cout << "Error, cannot login user: " << e.what() << endl;
    }
}

void AuthFrame::OnRegister(wxCommandEvent &event)
{
    std::string username = usernameInput->GetValue().ToStdString();
    std::string password = passwordInput->GetValue().ToStdString();

    AuthManager *authManager = mainApp->getAuthManager();
    try
    {
        User newUser = authManager->registerUser(username, password, UserRole::User);
        Toast::Show(this, "Registration successful!", *wxGREEN);
    }
    catch (const ClientException &e)
    {
        Toast::Show(this, e.what(), *wxRED);
        std::cout << "ClientError, cannot register user: " << e.what() << endl;
    }
    catch (const std::runtime_error &e)
    {
        Toast::Show(this, "Server error", *wxRED);
        std::cout << "Error, cannot register user: " << e.what() << endl;
    }
}
