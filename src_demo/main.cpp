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
#include "CDemoMainWindow.h"

//Global Variables
CDemoMainWindow *g_pMainWindow;

int32 Main(const CString &refProgramName, const CLinkedList<CString> &refArgs)
{
    int32 exitCode;

    g_pMainWindow = new CDemoMainWindow;

    g_pMainWindow->Show();

    while(CEventQueue::ProcessPendingEvents(exitCode))
    {
        g_pMainWindow->Update(); //application is idle.. repaint window
    }

    return exitCode;
}