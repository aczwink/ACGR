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
//Local
#include "SceneNode.hpp"
#include "SkyBox.hpp"

namespace ACGR
{
	class SceneManager
	{
	public:
		//Members
		ACStdLib::Math::Vector3 ambientLight;

		//Constructor
		SceneManager();

		//Destructor
		~SceneManager();

		//Methods
		void SetSkyBox(const ACStdLib::ByteString &refResource);

		//Inline
		inline ACStdLib::Map<Light *, SceneNode *> GetLights() const
		{
			return this->sceneGraphRoot->GetLights();
		}

		inline SceneNode *GetRootNode()
		{
			return this->sceneGraphRoot;
		}

		inline const SceneNode *GetRootNode() const
		{
			return this->sceneGraphRoot;
		}

		inline const SkyBox &GetSkyBox() const
		{
			return this->skyBox;
		}

		inline void SetRootNode(SceneNode *node)
		{
			if(this->sceneGraphRoot)
				delete this->sceneGraphRoot;

			this->sceneGraphRoot = node;
		}

	private:
		//Members
		SceneNode *sceneGraphRoot;
		SkyBox skyBox;
	};
}