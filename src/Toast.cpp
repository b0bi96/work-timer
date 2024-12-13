#include "../include/Toast.h"

void Toast::Show(wxWindow *parent, const wxString &message, const wxColour &background, int duration)
{
    wxFrame *frame = new wxFrame(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxFRAME_NO_TASKBAR | wxFRAME_SHAPED | wxBORDER_NONE);
    frame->SetBackgroundColour(background);

    wxStaticText *label = new wxStaticText(frame, wxID_ANY, message, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER | wxST_NO_AUTORESIZE);
    label->SetForegroundColour(*wxWHITE);
    label->Wrap(500);

    frame->SetSizer(new wxBoxSizer(wxHORIZONTAL));
    frame->GetSizer()->Add(label, 1, wxALL | wxEXPAND, 10);
    frame->GetSizer()->Fit(frame);
    frame->GetSizer()->SetSizeHints(frame);

    frame->SetSize(wxSize(parent->GetSize().x, frame->GetSize().y));
    frame->SetPosition(wxPoint(parent->GetPosition().x, parent->GetPosition().y + parent->GetSize().y - frame->GetSize().y));

    frame->Show(true);

    wxTimer *timer = new wxTimer(frame, wxID_ANY);
    timer->Bind(wxEVT_TIMER, [frame](wxTimerEvent &event)
                { frame->Close(); });
    timer->Start(duration, wxTIMER_ONE_SHOT);
}

void Toast::OnClose(wxTimerEvent &event, wxWindow *toastWindow)
{
    toastWindow->Destroy();
}
