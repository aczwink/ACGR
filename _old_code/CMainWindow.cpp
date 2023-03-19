//Class header
#include "CMainWindow.h"

//Constructor
::CMainWindow::CMainWindow()
{
	CGroupBox *pSettingsGroup;
	CPushButton *pRayTraceButton;
	CCheckBox *pCheckBox;
	
	this->SetTitle("OpenGL Testing");
	this->SetLayout(new CHorizontalLayout);
	
	this->p3DView = new CDisplayWidget(this);

	//settings panel
	pSettingsGroup = new CGroupBox(this);
	pSettingsGroup->SetText("Settings");

	pCheckBox = new CCheckBox(pSettingsGroup);
	pCheckBox->SetText("Toggle wireframe");
	pCheckBox->BindToggled(
		[p3DView = this->p3DView, pCheckBox]()
		{
			p3DView->SetWireFrame(pCheckBox->IsChecked());
		}
	);

	pCheckBox = new CCheckBox(pSettingsGroup);
	pCheckBox->SetText("Toggle debug mode");
	pCheckBox->BindToggled(
		[p3DView = this->p3DView, pCheckBox]()
		{
			p3DView->SetDebugMode(pCheckBox->IsChecked());
		}
	);

	pRayTraceButton = new CPushButton(pSettingsGroup);
	pRayTraceButton->SetText("Raytrace");
	pRayTraceButton->BindPushed(
		[p3DView = this->p3DView, pCheckBox]()
		{
			uint64 t0;
			float64 dt;

			t0 = QueryHighPrecisionClockCounter();
			p3DView->RayTrace();
			dt = (QueryHighPrecisionClockCounter() - t0) / (float64)QueryHighPrecisionClockFrequency();

			stdOut << "Raytracing took " << dt << " seconds." << endl;
		}
	);
}