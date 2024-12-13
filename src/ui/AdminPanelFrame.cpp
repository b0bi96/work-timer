#include "../../include/AdminPanelFrame.h"
#include "../../include/MyApp.h"
#include "../../include/User.h"
#include "../../include/DatabaseManager.h"
#include "../../include/AuthManager.h"
#include "../../include/AuthFrame.h"
#include "../../include/Toast.h"
#include "../utils/Exceptions.h"

AdminPanelFrame::AdminPanelFrame(const wxString &title, MyApp *mainApp)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400)),
      mainApp(mainApp)
{
    wxPanel *panel = new wxPanel(this);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    userList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize(580, 200),
                              wxLC_REPORT | wxLC_SINGLE_SEL);
    userList->InsertColumn(0, "ID", wxLIST_FORMAT_LEFT, 50);
    userList->InsertColumn(1, "Username", wxLIST_FORMAT_LEFT, 150);
    userList->InsertColumn(2, "Role", wxLIST_FORMAT_LEFT, 100);
    userList->InsertColumn(3, "Work Time", wxLIST_FORMAT_LEFT, 100);
    vbox->Add(userList, 0, wxALL | wxEXPAND, 5);

    wxFlexGridSizer *formSizer = new wxFlexGridSizer(2, 5, 5);

    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Username:"), 0, wxALIGN_RIGHT);
    usernameInput = new wxTextCtrl(panel, wxID_ANY);
    formSizer->Add(usernameInput, 1, wxEXPAND);

    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Role:"), 0, wxALIGN_RIGHT);
    roleInput = new wxChoice(panel, wxID_ANY);
    roleInput->Append("Owner");
    roleInput->Append("User");
    formSizer->Add(roleInput, 1, wxEXPAND);

    formSizer->AddGrowableCol(1, 1);
    vbox->Add(formSizer, 0, wxALL | wxEXPAND, 10);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *updateButton = new wxButton(panel, wxID_ANY, "Update User");
    wxButton *removeButton = new wxButton(panel, wxID_ANY, "Remove User");
    wxButton *logoutButton = new wxButton(panel, wxID_ANY, "Logout");
    wxButton *removeWorkTimeButton = new wxButton(panel, wxID_ANY, "Remove work time");

    panel->SetSizer(vbox);

    buttonSizer->Add(updateButton, 1, wxALL | wxALIGN_CENTER, 5);
    buttonSizer->Add(removeButton, 1, wxALL | wxALIGN_CENTER, 5);
    buttonSizer->Add(removeWorkTimeButton, 1, wxALL | wxALIGN_CENTER, 5);
    buttonSizer->Add(logoutButton, 1, wxALL | wxALIGN_CENTER, 5);

    vbox->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);

    panel->SetSizer(vbox);

    LoadUsers();

    updateButton->Bind(wxEVT_BUTTON, &AdminPanelFrame::OnUpdateUser, this);
    removeButton->Bind(wxEVT_BUTTON, &AdminPanelFrame::OnRemoveUser, this);
    removeWorkTimeButton->Bind(wxEVT_BUTTON, &AdminPanelFrame::onRemoveUserWorkTime, this);
    logoutButton->Bind(wxEVT_BUTTON, &AdminPanelFrame::OnLogout, this);
}

void AdminPanelFrame::LoadUsers()
{
    userList->DeleteAllItems();

    DatabaseManager *db = mainApp->getDatabaseManager();
    vector<User> users = db->getAllUsers();

    for (const auto &user : users)
    {
        long index = userList->InsertItem(userList->GetItemCount(), wxString::Format("%d", user.id));
        userList->SetItem(index, 1, user.username);
        userList->SetItem(index, 2, user.role == UserRole::Owner ? "Owner" : "User");
        int hours = static_cast<int>(user.workTime) / 3600;
        int minutes = (static_cast<int>(user.workTime) % 3600) / 60;
        int seconds = static_cast<int>(user.workTime) % 60;
        wxString formattedTime = wxString::Format("%02d:%02d:%02d", hours, minutes, seconds);

        userList->SetItem(index, 3, formattedTime);
    }
}

void AdminPanelFrame::OnUpdateUser(wxCommandEvent &event)
{
    long itemIndex = userList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1)
    {
        Toast::Show(this, "Please select a user to update.", *wxRED);
        return;
    }

    wxString idStr = userList->GetItemText(itemIndex);
    long id;
    if (!idStr.ToLong(&id))
    {
        Toast::Show(this, "Invalid user ID.", *wxRED);
        return;
    }

    string username = usernameInput->GetValue().ToStdString();
    UserRole role = roleInput->GetSelection() == 0 ? UserRole::Owner : UserRole::User;

    try
    {
        DatabaseManager *db = mainApp->getDatabaseManager();
        db->updateUserById(id, username, "", &role, nullptr);
        LoadUsers();
        Toast::Show(this, "User updated successfully!", *wxGREEN);
    }
    catch (const ClientException &e)
    {
        Toast::Show(this, e.what(), *wxRED);
        std::cout << "ClientError, AdminPanelFrame::OnUpdateUser " << e.what() << endl;
    }
    catch (const std::exception &e)
    {
        Toast::Show(this, "Cannot update user!", *wxRED);
        std::cout << "Error, AdminPanelFrame::OnUpdateUser " << e.what() << endl;
    }
}

void AdminPanelFrame::OnRemoveUser(wxCommandEvent &event)
{
    long itemIndex = userList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1)
    {
        Toast::Show(this, "Please select a user to remove.", *wxRED);
        return;
    }

    wxString idStr = userList->GetItemText(itemIndex);
    long id;
    if (!idStr.ToLong(&id))
    {
        Toast::Show(this, "Invalid user ID.", *wxRED);
        return;
    }

    try
    {
        DatabaseManager *db = mainApp->getDatabaseManager();
        User user = db->getUserById(id);
        if (user.role == UserRole::Owner)
        {
            Toast::Show(this, "Cannot remove owner!", *wxRED);
            return;
        }

        db->removeUserById(id);
        LoadUsers();
        Toast::Show(this, "User removed successfully!", *wxGREEN);
    }
    catch (const ClientException &e)
    {
        Toast::Show(this, e.what(), *wxRED);
        std::cout << "ClientError, AdminPanelFrame::OnRemoveUser " << e.what() << endl;
    }
    catch (const std::exception &e)
    {
        Toast::Show(this, "Cannot remove user!", *wxRED);
        std::cout << "Error, AdminPanelFrame::OnRemoveUser " << e.what() << endl;
    }
}

void AdminPanelFrame::OnLogout(wxCommandEvent &event)
{
    try
    {
        mainApp->getAuthManager()->logOut();
        Toast::Show(this, "Logged out successfully", *wxGREEN);

        this->Close();
    }
    catch (const ClientException &e)
    {
        Toast::Show(this, e.what(), *wxRED);
        std::cout << "ClientError, AdminPanelFrame::OnLogout " << e.what() << endl;
    }
    catch (const std::exception &e)
    {
        Toast::Show(this, "Cannot logout!", *wxRED);
        std::cout << "Error, AdminPanelFrame::OnLogout " << e.what() << endl;
    }
}

void AdminPanelFrame::onRemoveUserWorkTime(wxCommandEvent &event)
{

    long itemIndex = userList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1)
    {
        Toast::Show(this, "Please select a user to remove.", *wxRED);
        return;
    }

    wxString idStr = userList->GetItemText(itemIndex);
    long id;
    if (!idStr.ToLong(&id))
    {
        Toast::Show(this, "Invalid user ID.", *wxRED);
        return;
    }

    try
    {
        DatabaseManager *db = mainApp->getDatabaseManager();
        db->updateWorkTime(id, 0);
        LoadUsers();
        Toast::Show(this, "Work time removed successfully.", *wxGREEN);
    }
    catch (const ClientException &e)
    {
        Toast::Show(this, e.what(), *wxRED);
        std::cout << "ClientError, AdminPanelFrame::onRemoveUserWorkTime" << e.what() << endl;
    }
    catch (const std::exception &e)
    {
        Toast::Show(this, "Cannot remove user workout!", *wxRED);
        std::cout << "Error, AdminPanelFrame::onRemoveUserWorkTime" << e.what() << endl;
    }
}
