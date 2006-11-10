//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("Project2.res");
USEFORM("Unit1.cpp", EmergencyHelper);
USEFORM("Unit2.cpp", FormSobre);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "Emergency Helper";
		Application->CreateForm(__classid(TEmergencyHelper), &EmergencyHelper);
		Application->CreateForm(__classid(TFormSobre), &FormSobre);
		Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
