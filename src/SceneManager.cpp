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
#include <ACGR/SceneManager.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;
using namespace ACGR;

//Constructor
SceneManager::SceneManager()
{
	this->sceneGraphRoot = new SceneNode;
}

//Destructor
SceneManager::~SceneManager()
{
	delete this->sceneGraphRoot;
}

//Public methods
void SceneManager::SetSkyBox(const String &refResource)
{
	NOT_IMPLEMENTED_ERROR;

	/*
	this->skyBox.pBack = refResourceManager.GetTexture(refResource + "_back");
	this->skyBox.pBottom = refResourceManager.GetTexture(refResource + "_bottom");
	this->skyBox.pFront = refResourceManager.GetTexture(refResource + "_front");
	this->skyBox.pLeft = refResourceManager.GetTexture(refResource + "_left");
	this->skyBox.pRight = refResourceManager.GetTexture(refResource + "_right");
	this->skyBox.pTop = refResourceManager.GetTexture(refResource + "_top");
	*/
}