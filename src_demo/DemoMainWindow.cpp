/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACGR.
 *
 * ACGR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACGR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACGR.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include "DemoMainWindow.hpp"

//Constructor
DemoMainWindow::DemoMainWindow(EventQueue& eventQueue) : MainAppWindow(eventQueue)
{
    this->SetTitle("AC3D Demo");

	this->SetupChildren();
}

//Private methods
void DemoMainWindow::SetupChildren()
{
	CompositeWidget* container = new CompositeWidget;
	container->SetLayout(new HorizontalLayout);
	this->SetContentContainer(container);

	this->displayWidget = new DisplayWidget();
	container->AddChild(this->displayWidget);

	//settings panel
	GroupBox *pSettingsGroup = new GroupBox();
	pSettingsGroup->SetTitle(u8"Settings");
	container->AddChild(pSettingsGroup);

	PushButton *rayTraceButton = new PushButton();
	rayTraceButton->SetText("Raytrace");
	rayTraceButton->onActivatedHandler = [p3DView = this->displayWidget]()
	{
		Clock c;
		c.Start();
		p3DView->RayTrace();
		stdOut << "Raytracing took " << c.GetElapsedMicroseconds() / 1000000.0 << " seconds." << endl;
	};
	pSettingsGroup->AddContentChild(rayTraceButton);

	CheckBox *checkBox = new CheckBox();
	checkBox->SetText("Toggle wireframe");
	checkBox->onToggledHandler = [p3DView = this->displayWidget, checkBox]()
	{
		p3DView->SetWireFrame(checkBox->IsChecked());
	};
	pSettingsGroup->AddContentChild(checkBox);

	checkBox = new CheckBox();
	checkBox->SetText("Toggle debug mode");
	checkBox->onToggledHandler = [p3DView = this->displayWidget, checkBox]()
	{
		p3DView->SetDebugMode(checkBox->IsChecked());
	};
	pSettingsGroup->AddContentChild(checkBox);
}