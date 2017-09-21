/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
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
DemoMainWindow::DemoMainWindow()
{
    this->SetTitle("AC3D Demo");

	this->SetupChildren();
}

//Private methods
void DemoMainWindow::SetupChildren()
{
	this->SetLayout(new HorizontalLayout);
	this->displayWidget = new DisplayWidget(this);

	//settings panel
	GroupBox *pSettingsGroup = new GroupBox(this);
	pSettingsGroup->SetText("Settings");

	PushButton *rayTraceButton = new PushButton(pSettingsGroup);
	rayTraceButton->SetText("Raytrace");
	rayTraceButton->onActivatedHandler = [p3DView = this->displayWidget]()
	{
		Clock c;
		c.Start();
		p3DView->RayTrace();
		stdOut << "Raytracing took " << c.GetElapsedMicroseconds() / 1000000.0 << " seconds." << endl;
	};

	CheckBox *checkBox = new CheckBox(pSettingsGroup);
	checkBox->SetText("Toggle wireframe");
	checkBox->onToggledHandler = [p3DView = this->displayWidget, checkBox]()
	{
		p3DView->SetWireFrame(checkBox->IsChecked());
	};

	checkBox = new CheckBox(pSettingsGroup);
	checkBox->SetText("Toggle debug mode");
	checkBox->onToggledHandler = [p3DView = this->displayWidget, checkBox]()
	{
		p3DView->SetDebugMode(checkBox->IsChecked());
	};
}