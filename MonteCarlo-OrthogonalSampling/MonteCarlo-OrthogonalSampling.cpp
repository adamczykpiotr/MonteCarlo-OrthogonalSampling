#include <iostream>
#include "MonteCarlo.h"
#include <vector>
#include <fstream>

float matyas(float x, float y) {
	return 0.26f * (x * x + y * y) - 0.48f * x * y;
}

float mcCormick(float x, float y) {
	return sin(x + y) + (x - y) * (x - y) - 1.5f * x + 2.5f * y + 1.f;
}

float zEquals4(float x, float y) {
	return 4;
}

int main() {

	MonteCarlo::Function matyas(matyas, -10.f, 10.f, -10.f, 10.f, 0.f, 100.f);
	MonteCarlo::Function mcCormick(mcCormick, -2.f, 4.f, -2.f, 4.f, 0.f, 40.f);
	MonteCarlo::Function zEquals4(zEquals4, 0.f, 10.f, 0.f, 10.f, 0.f, 100.f);

	srand(static_cast<unsigned int>(time(NULL)));

	//	<config>
	unsigned int parts[6] = { 2,5,10,20,50,100 };
	unsigned int samples[7] = { 1,2,5,10,20,50,100 };

	int k = 4;
	int j = 3;
	//	</config>

	MonteCarlo::Function f = zEquals4;

	//Monte Carlo (Random Sampling) 
	MonteCarlo::IntegrationInfo a = MonteCarlo::randomSampling(f.function, f.params.xMin, f.params.xMax, f.params.yMin, f.params.yMax, f.params.zMin, f.params.zMax, MonteCarlo::Utils::getEquivalentPrecision(parts[j], samples[k]));
	MonteCarlo::Utils::printInfo(a);


	//Monte Carlo (Random Sampling) with 10 times the iterations
	MonteCarlo::IntegrationInfo b = MonteCarlo::randomSampling(f.function, f.params.xMin, f.params.xMax, f.params.yMin, f.params.yMax, f.params.zMin, f.params.zMax, 10 * MonteCarlo::Utils::getEquivalentPrecision(parts[j], samples[k]));
	MonteCarlo::Utils::printInfo(b);

	//Monte Carlo (Orthogonal Sampling)
	MonteCarlo::IntegrationInfo c = MonteCarlo::orthogonalSampling(f.function, f.params.xMin, f.params.xMax, f.params.yMin, f.params.yMax, f.params.zMin, f.params.zMax, parts[j], samples[k]);
	MonteCarlo::Utils::printInfo(c);

	return 0;
}

