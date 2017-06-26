/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of AC3D.
 *
 * AC3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AC3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with AC3D.  If not, see <http://www.gnu.org/licenses/>.
 */
//Local
#include "DemoMainWindow.hpp"

int32 Main(const String &programName, const LinkedList<String> &args)
{
    DemoMainWindow *mainWindow = new DemoMainWindow;

	mainWindow->Show();

	while(EventQueue::ProcessPendingEvents())
		mainWindow->Update(); //application is idle.. repaint window
	EventQueue::ProcessEvents();

	return EXIT_SUCCESS;
}