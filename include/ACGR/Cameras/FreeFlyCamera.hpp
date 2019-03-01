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
//Local
#include "ACGR/Camera.hpp"

namespace ACGR
{
	class FreeFlyCamera : public Camera
	{
	public:
		//Constructor
		inline FreeFlyCamera()
		{
			this->freeLook = false;
		}

		//Inline
		inline void EnableFreeLook(bool enable)
		{
			this->freeLook = enable;
		}

		inline bool IsFreeLookEnabled() const
		{
			return this->freeLook;
		}

	private:
		//Members
		bool freeLook;

		//Methods
		void UpdateByMouse(float32 dt, const StdXX::Math::Point<uint16>& mousePos, bool isLeftMouseButtonDown);
		void UpdateByKeyboard(float32 dt, bool isShiftDown, bool w, bool s, bool a, bool d);
	};
}