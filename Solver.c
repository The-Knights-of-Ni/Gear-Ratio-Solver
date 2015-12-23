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
//TODO: Give range (for sizes)
#define fileName "Gears.txt"
int gearOptions[25];
int driveGear = 1;
int drivenGear = 1;
int motorRatio;
int numberSolutions; //Uninmplemented. It should find the most compact solution right now.

bool closerTo(float val1, float val2, float target)//True is when val1 is closer
{
	if(fabsf(target - val1) < fabsf(target - val2))
		return 1;
	else if(fabsf(target - val1) > fabsf(target - val2))
		return 0;
	else
		return 0;
}

void solve(float desiredRatio, size_t length)
{
	float currentRatio = 0;
	float previousRatio;
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

int main()
{
	populateArray();
	bool continueRunning = 1;
	printf("Welcome to the gear ratio optimizer. Version 1.0\n\n");
	while(continueRunning)
	{
		printf("Please enter the gearbox ratio on the motor: ");
		scanf("%i", &motorRatio);
		printf("Please enter the target ratio (not including motor): ");
		float target;
		scanf("%f", &target);
		solve(target*motorRatio, sizeof gearOptions/sizeof *gearOptions);
		printf("These are the closest gears we could find (at the smallest size): \n", motorRatio, target);
		printf("Drive gear: %i\n", driveGear);
		printf("Driven gear: %i\n", drivenGear);
		printf("\nWould you like to enter another ratio? (y/n)\n");
		char answer[5];
		scanf("%s", &answer);
		if(answer[0] == 'n' || answer[0] == 'N')
			continueRunning = 0;
	}
}
