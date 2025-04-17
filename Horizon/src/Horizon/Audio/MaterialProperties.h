#pragma once

// MaterialProperties
// Contains an absorbtion coefficient for each frequency range
// - Equation for coefficient: r = Input strength / Output strength
// So to find the output from the input: Output strength = r * Input strength
// There are 8 bands of frequencies
// 125hz
// 250hz
// 500hz
// 1000hz
// 2000hz
// 4000hz
// 8000hz
// 16000hz

namespace Horizon
{
	struct MaterialAudioProperties
	{
		float absorbtion[8] = {1.0f};
		float scattering = 1.0f;
		float transmission = 0.0f;
	};
}

