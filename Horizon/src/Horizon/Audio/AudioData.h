#pragma once

#include "MaterialProperties.h"

namespace Horizon
{
	struct AudioData
	{
	public:
		float frequenciesVolume[8] = { 100.0f };
		

		AudioData operator*=(const MaterialAudioProperties& mat)
		{
			for (int i = 0; i < 7; i++)
			{
				this->frequenciesVolume[i] *= mat.absorbtion[i];
			}
			return *this;
		}
	};
}