#ifndef TOAST_H
#define TOAST_H

#include <wx/wx.h>
#include <wx/timectrl.h>

class Toast
{
public:
    static void Show(wxWindow *parent, const wxString &message, const wxColour &background, int duration = 3000);

private:
    static void OnClose(wxTimerEvent &event, wxWindow *toastWindow);
};

#endif
