#pragma once
#include <ctime>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>

class MonteCarlo {
public:

	class Cuboid {
	public:
		float xMin;
		float xMax;
		float yMin;
		float yMax;
		float zMin;
		float zMax;
	};

	class IntegrationInfo {
	public:
		float volume;
		clock_t millis;
		unsigned int iterations;
		std::string name;
	};

	class Function {
	public:
		float(*function)(float, float);
		class Params {
		public:
			float xMin;
			float xMax;
			float yMin;
			float yMax;
			float zMin;
			float zMax;
		};
		Params params;

		Function() {
			function = NULL;
			params = Params();
		}

		Function(float(*f)(float, float), float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
			function = f;
			params.xMin = xMin;
			params.xMax = xMax;
			params.yMin = yMin;
			params.yMax = yMax;
			params.zMin = zMin;
			params.zMax = zMax;

		}
	};

	class Utils {
	public:
		//Calculates number of points designated in otrhogonal sampling method
		inline static unsigned int getEquivalentPrecision(unsigned int partitions, unsigned int samples = 1);
		static float randomFloat(float min, float max);
		static Cuboid*** partitions(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, unsigned int partitions);
		static void printInfo(IntegrationInfo info);

	};

	static IntegrationInfo randomSampling(float(*function)(float, float), float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, unsigned int precision);
	static IntegrationInfo orthogonalSampling(float(*function)(float, float), float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, unsigned int partitions, unsigned int samples = 1);

};

