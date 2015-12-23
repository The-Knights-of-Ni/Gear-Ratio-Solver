/* Gear Ratio Solver
Dev Rangarajan, FTC 5206 The Knights of Ni
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
//TODO: Add in part numbers and materials to gear list
//TODO: Multiple options
//TODO: aim high/low
#define fileName "Gears.txt"
#define gearOptionSize sizeof gearOptions/sizeof *gearOptions
#define possibleSolutionSize sizeof possibleSolutions/sizeof *possibleSolutions


int gearOptions[27];
int driveGear = 1;
int drivenGear = 1;
int motorRatio;
int possibleSolutions[20][2];

void populateArray()
{
	FILE *IO;
	char gears[] = fileName;
	if((IO = fopen(gears, "r")) == NULL)
	{
		printf("Fix the code.\n");
		//assert(0);
		return;
	}

	for(int i = 0; i < 25; i++)
	{
		char buffer[4];
		fgets(buffer, 4, IO);
		gearOptions[i] = atoi(buffer);
	}

	fclose(IO);
}

bool closerTo(float val1, float val2, float target)//True is when val1 is closer
{
	if(fabsf(target - val1) < fabsf(target - val2))
		return 1;
	else if(fabsf(target - val1) > fabsf(target - val2))
		return 0;
	else
		return 0;
}

bool checkDuplicate(int val1, int val2, size_t topx)
{
	for(int i = 0; i < topx; i++)
	{
		if((possibleSolutions[i][0] == val1) && (possibleSolutions[i][1] == val2))
			return 0;
	}
	return 1;
}

void solve(float desiredRatio, size_t length)
{
	float currentRatio = 0;
	for(int i = 0; i < length; i++)
	{
		if(gearOptions[i] == 0)
			continue;
		for(int j = 0; j < length; j++)
		{
			if(gearOptions[j] == 0)
				continue;
			currentRatio = (float)gearOptions[j]/gearOptions[i] * motorRatio;
			if(closerTo(currentRatio, (float)drivenGear/driveGear*motorRatio, desiredRatio))
			{
				driveGear = gearOptions[i];
				drivenGear = gearOptions[j];
			}
		}
	}
}

void solveMany(float desiredRatio, size_t lengthGears, size_t topx)
{
	for(int i = 0; i < topx; i++)
	{
		possibleSolutions[i][0] = 1;
		possibleSolutions[i][1] = 1;
	}

	for(int h = 0; h < topx; h++)
	{
		float currentRatio = 0;
		for(int i = 0; i < lengthGears; i++)
		{
			if(gearOptions[i] == 0)
				continue;
			for(int j = 0; j < lengthGears; j++)
			{
				if(gearOptions[j] == 0)
					continue;
				currentRatio = (float)gearOptions[j]/gearOptions[i] * motorRatio;
				if(!checkDuplicate(gearOptions[j], gearOptions[i], topx))
				{
					if(h != 0)
						continue;
				}
				if(closerTo(currentRatio, (float)possibleSolutions[h][0]/possibleSolutions[h][1]*motorRatio, desiredRatio))
				{
					possibleSolutions[h][1] = gearOptions[i];
					possibleSolutions[h][0] = gearOptions[j];
				}
			}
		}
	}
}

float getError(int actualDrivenGear, int actualDriveGear, float target)
{
	return 100*((float)actualDrivenGear/actualDriveGear*motorRatio)/target;
}

void getForMotor(int motorValue, int numberToPrint, float target)
{
	motorRatio = motorValue;
	solveMany(target, gearOptionSize, possibleSolutionSize);
	printf("------NeverRest %i------\n", motorRatio);
	for(int i = 0; i < numberToPrint; i++)
	{
		printf("%3i:%-3i| Accuracy: %3.2f%%\n", possibleSolutions[i][0], possibleSolutions[i][1],
 			getError(possibleSolutions[i][0], possibleSolutions[i][1], target));
	}
}

int main()
{
	populateArray();
	bool continueRunning = 1;
	printf("Welcome to the gear ratio optimizer. Version 1.0\n\n");
	while(continueRunning)
	{
		printf("Please enter the target ratio (including motor): ");
		float target;
		scanf("%f", &target);
		printf("Please enter the number of options you would like to see for each motor: ");
		int options;
		scanf("%i", &options);
		if(options > possibleSolutionSize)
			options = possibleSolutionSize;
		getForMotor(20, options, target);
		getForMotor(40, options, target);
		getForMotor(60, options, target);
		printf("\nWould you like to enter another ratio? (y/n)\n");
		char answer[5];
		scanf("%s", &answer);
		if(answer[0] == 'n' || answer[0] == 'N')
			continueRunning = 0;
	}
}
