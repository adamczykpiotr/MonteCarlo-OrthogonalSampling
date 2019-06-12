#include "MonteCarlo.h"



unsigned int MonteCarlo::Utils::getEquivalentPrecision(unsigned int partitions, unsigned int samples) {

	return (partitions * partitions * partitions * samples);

}

float MonteCarlo::Utils::randomFloat(float min, float max) {

	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

MonteCarlo::Cuboid*** MonteCarlo::Utils::partitions(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, unsigned int partitions) {

	float xLength = xMax - xMin;
	float yLength = yMax - yMin;
	float zLength = zMax - zMin;


	float avgX = xLength / partitions;
	float avgY = yLength / partitions;
	float avgZ = zLength / partitions;

	// 3D Array
	Cuboid*** arr = new Cuboid * *[partitions];
	for (unsigned int i = 0; i < partitions; i++) {
		arr[i] = new Cuboid * [partitions];
		for (unsigned int j = 0; j < partitions; j++) {
			arr[i][j] = new Cuboid[partitions];
		}
	}

	float xStart = xMin, xEnd = xMin;
	float yStart = yMin, yEnd = yMin;
	float zStart = zMin, zEnd = zMin;

	for (unsigned int i = 0; i < partitions; i++) {
		//slicing x axis

		if (i != partitions - 1) {
			xEnd += avgX;
		}
		else {
			xEnd = xMax;
		}

		yStart = yMin;
		yEnd = yMin;

		for (unsigned int j = 0; j < partitions; j++) {
			//slicing y axis

			if (j != partitions - 1) {
				yEnd += avgY;
			}
			else {
				yEnd = yMax;
			}

			zStart = zMin;
			zEnd = zMin;

			for (unsigned int k = 0; k < partitions; k++) {
				//slicing z axis

				if (k != partitions - 1) {
					zEnd += avgZ;
				}
				else {
					zEnd = zMax;
				}

				arr[i][j][k].xMin = xStart;
				arr[i][j][k].yMin = yStart;
				arr[i][j][k].zMin = zStart;

				arr[i][j][k].xMax = xEnd;
				arr[i][j][k].yMax = yEnd;
				arr[i][j][k].zMax = zEnd;

				zStart += avgZ;
			}

			yStart += avgY;


		}

		xStart += avgX;

	}

	return arr;
}

void MonteCarlo::Utils::printInfo(MonteCarlo::IntegrationInfo info) {

	std::cout << std::setprecision(10);

	std::cout << "Sampling method: " << info.name << "\n";
	std::cout << "Iterations:\t" << info.iterations << "\n";
	std::cout << "Volume:\t" << info.volume << "\n";
	std::cout << "Time:\t" << info.millis << "\n\n";

}


MonteCarlo::IntegrationInfo MonteCarlo::randomSampling(float(*function)(float, float), float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, unsigned int precision) {

	clock_t start = clock();

	IntegrationInfo info;
	info.iterations = precision;
	info.name = "Random";

	if (precision == 0) {
		info.millis = clock() - start;
		info.volume = -INFINITY;
		return info;
	}

	unsigned long long int hits = 0;
	float volume = abs(xMax - xMin) * abs(yMax - yMin) * abs(zMax - zMin);

	for (unsigned int i = 0; i < precision; i++) {

		float x = MonteCarlo::Utils::randomFloat(xMin, xMax);
		float y = MonteCarlo::Utils::randomFloat(yMin, yMax);
		float z = MonteCarlo::Utils::randomFloat(zMin, zMax);

		//std::cout  << " FOR: " << "xmin: " << xMin << " xmax: " << xMax << "ymin: " << yMin << " ymax: " << yMax << "zmin: " << zMin << " zmax: " << zMax << "VALUES: " << " x: " << x << " y: " << y << " z: " << z << std::endl;

		/*Checking whether randomly generated point "hits" desired area under or on the plane*/
		if (function(x, y) >= z && function(x, y) >= zMin) {
			hits++;
		}

	}

	info.millis = clock() - start;
	info.volume = (hits * volume) / precision;
	return info;

}

MonteCarlo::IntegrationInfo MonteCarlo::orthogonalSampling(float(*function)(float, float), float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, unsigned int partitions, unsigned int samples) {

	float mem = (sizeof(Cuboid) + sizeof(int)) * partitions * partitions * partitions / 1024 / 1024;
	if (mem > 256) {
		char answer = 'n';
		std::cout << "Estimated memory usage: " << mem << "MB. Proceed? (y/n): ";
		std::cin >> answer;
		if (answer != 'y' && answer != 'Y') {
			IntegrationInfo info;
			info.iterations = 0;
			info.millis = 0;
			info.name = "None";
			info.volume = 0;
			return info;
		}
	}


	clock_t start = clock();

	IntegrationInfo info;
	info.iterations = Utils::getEquivalentPrecision(partitions, samples);
	info.name = "Orthogonal";

	unsigned long long int hits = 0;
	float volume = abs(xMax - xMin) * abs(yMax - yMin) * abs(zMax - zMin);

	Cuboid*** tab = MonteCarlo::Utils::partitions(xMin, xMax, yMin, yMax, zMin, zMax, partitions);

	for (unsigned int i = 0; i < samples; i++) {
		for (unsigned int i = 0; i < partitions; i++) {
			for (unsigned int j = 0; j < partitions; j++) {
				for (unsigned int k = 0; k < partitions; k++) {

					float x = MonteCarlo::Utils::randomFloat(tab[i][j][k].xMin, tab[i][j][k].xMax);
					float y = MonteCarlo::Utils::randomFloat(tab[i][j][k].yMin, tab[i][j][k].yMax);
					float z = MonteCarlo::Utils::randomFloat(tab[i][j][k].zMin, tab[i][j][k].zMax);

					//std::cout  << " FOR: " << "xmin: " << tab[i][j][k].xMin << " xmax: " << tab[i][j][k].xMax << "ymin: " << tab[i][j][k].yMin << " ymax: " << tab[i][j][k].yMax << "zmin: " << tab[i][j][k].zMin << " zmax: " << tab[i][j][k].zMax << "VALUES: " << " x: " << x << " y: " << y << " z: " << z << std::endl;

					/*Checking whether randomly generated point "hits" desired area under or on the plane*/
					if (function(x, y) >= z && function(x, y) >= zMin) {
						hits++;
					}

				}
			}
		}
	}

	for (unsigned int i = 0; i < partitions; i++) {
		for (unsigned int j = 0; j < partitions; j++) {
			delete[] tab[i][j];
		}
	}


	info.millis = clock() - start;
	info.volume = (hits * volume) / (samples * partitions * partitions * partitions);
	return info;
}

