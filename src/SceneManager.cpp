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
#include <ACGR/SceneManager.hpp>
//Local
#include "Internal/Parse.hpp"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACGR;

//Constructor
SceneManager::SceneManager()
{
	this->pSceneGraphRoot = new SceneNode;
}

//Destructor
SceneManager::~SceneManager()
{
	delete this->pSceneGraphRoot;
}

//Public methods
void SceneManager::Load(InputStream &refInput)
{
	XML::CDocument *pDocument;

	pDocument = XML::CDocument::Parse(refInput);

	XML::CElement &refScene = pDocument->GetRootElement();

	if(refScene.HasAttribute("ambientLight"))
		this->ambientLight = ParseVec3(refScene.GetAttribute("ambientLight"));

	if(refScene.HasAttribute("skybox"))
		this->SetSkyBox(To8BitString(refScene.GetAttribute("skybox")));

	this->pSceneGraphRoot->Deserialize(refScene);

	delete pDocument;
}