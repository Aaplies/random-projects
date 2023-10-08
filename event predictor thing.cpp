#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

void printVector(std::vector<float> input, std::string title)
{
	std::cout << title << ":\n";
	for (auto i : input)
		std::cout << i << '\n';
	std::cout << '\n';
}

void parseUserInput(const std::string& input, std::vector<std::string>& output, bool& isRT)
{
	std::string tempString;
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] != ' ' || input[i] == ':')
		{
			tempString += input[i];
			if (input[i] == ':')
				isRT = true;
		}
		else
		{
			output.push_back(tempString);
			tempString = "";
		}
	}
	output.push_back(tempString);
}

float HOURS_parseInputString(const std::string& input)
{
	std::string S_numbers;
	char identifier = '\0';
	for (int i = 0; i < input.size(); i++)
	{
		if (isdigit(input[i]) || input[i] == '.')
			S_numbers += input[i];
		else
			identifier = input[i];
	}
	float returnFloat = std::stof(S_numbers);
	switch (identifier)
	{
	case 'm': // MONTHS! Minutes is the default. (keep in mind hours is what's returned)
		return returnFloat * float(730.5); // hours in a year / 12
	case 'h':
		return returnFloat;
	case 'd':
		return returnFloat * 24;
	case 'y':
		return returnFloat * 8766;
	default:
		return returnFloat / 60;
	}
	return -1;
}

float HOURS_parseTime(const std::string& now, const std::string& input)
{
	std::string hours, minutes, hours2, minutes2;
	float totalTime;
	bool addToHours = true, addToHours2 = true, nextDay = false, nextday1 = false;

	for (auto& i : input)
	{
		if (i == 'n')
			nextday1 = true;
		else if (i != ':' && addToHours)
			hours += i;
		else if (i != ':')
			minutes += i;
		else
			addToHours = false;
	}
	for (auto& i : now)
	{
		if (i == 'n')
			nextDay = true;
		else if (i != ':' && addToHours2)
			hours2 += i;
		else if (i != ':')
			minutes2 += i;
		else
			addToHours2 = false;
	}
	totalTime = (60 * std::stof(hours)) + std::stof(minutes);
	float hours2Float = std::stof(hours2);
	if (nextDay)
		hours2Float += 24; 
	float hours1Float = std::stof(hours);
	if (nextday1)
		totalTime += 1440;
	return ((60 * hours2Float) + std::stof(minutes2) - totalTime) / 60;
}

void printTime(float input)
{
	int seconds = int(round(input * 3600));
	std::cout << int(floor(seconds / 86400)) % 365 << " day(s) ";
	std::cout << int(floor(seconds / 3600)) % 24 << " hours ";
	std::cout << int(floor(seconds / 60)) % 60 << " minutes.";
}

float findAverageDeviation(const std::vector<float>& inputVector, const float& average)
{
	float sumOfDeviations = 0.0;
	for (auto& i : inputVector)
		sumOfDeviations += abs(average - i);
	return sumOfDeviations / inputVector.size();
}

float findStandardDeviation(const std::vector<float>& inputVector, const float& average)
{
	float sumOfDeviations = 0.0;
	for (auto& i : inputVector)
		sumOfDeviations += float(pow((average - i), 2));
	sumOfDeviations /= inputVector.size();
	return sqrt(sumOfDeviations);
}

void help()
{
	std::cout << "Input all of the times of previous events, separated by spaces.\n";
	std::cout << "Choose either relative time or absolute time (10:05 vs. 2h)\n";
	std::cout << "There is no idiot proofing. Switching back and forth will produce unexpected results with no warning.\n";
	std::cout << "Prefixes are: h, d, m, y. M IS MONTHS, NO PREFIX IS MINUTES\n";
	std::cout << "When using absolute time (10:14), use 24h time. The first time you input will be treated as the current time.\n";
	exit(0);
	// append 'n' to an absolute time to add 24 hours automatically
}

void outputHelp()
{
	std::cout << "Absolute next: when an event should/should've occurred.\n";
	std::cout << "Realistic next predicted event: when an event realistically should occur, always in the future (or 0)\n";
	std::cout << "Average difference: sum of differences between times (chronologically) divided by number of differences.\n";
	std::cout << "AD (Average Deviation): The average deviation from the mean (per time difference)\n";
	std::cout << "SD (Standard Deviation): The expected standard deviation from the mean (almost same as AD, just slightly different)\n";
	std::cout << "Mean: exactly what you think, same as average difference but this one is in decimal hours.\n";
	std::cout << "STC (subject-to-change): how much the average would change if an event occurred right now.\n";
	std::cout << "Irregular: if something occurred right now, would it be within 1 standard deviation from the mean?\n";
	exit(0);
}

int main()
{
	bool isRT = false;
	std::vector<std::string> inputList;
	std::vector<float> hoursList, hours_differenceList;
	float sumOfDifferences = 0.0, averageDifferenceHours = 0.0, tempAvgDiff = 0.0, averageMean = 0.0, standardDeviation = 0.0, prevAvgDiff;
	std::string userFullInput, timeNow;

	getline(std::cin, userFullInput);
	if (userFullInput == "help")
		help(); // will not return, exits program
	else if (userFullInput == "outputhelp")
		outputHelp(); // will not return, exits program
	parseUserInput(userFullInput, inputList, isRT);

	if (!isRT)
	{
		for (auto& i : inputList)
			hoursList.push_back(HOURS_parseInputString(i));
	}
	else
	{
		timeNow = inputList[0];
		inputList.erase(inputList.begin());
		for (auto& i : inputList)
			hoursList.push_back(HOURS_parseTime(timeNow, i));
	}
	if (hoursList.size() <= 1)
	{
		std::cout << "Error: only 1 time listed.\n";
		exit(-1);
	}
	// printVector(hoursList, "hours list");
	std::sort(hoursList.begin(), hoursList.end());
	for (int i = 0; i < hoursList.size() - 1; i++)
		hours_differenceList.push_back(hoursList.at(i+1) - hoursList[i]); // hopefully should be overflow protected

	for (auto& i : hours_differenceList)
		sumOfDifferences += i;

	averageDifferenceHours = sumOfDifferences / hours_differenceList.size();
	averageMean = findAverageDeviation(hours_differenceList, averageDifferenceHours);
	standardDeviation = findStandardDeviation(hours_differenceList, averageDifferenceHours);

	std::cout << "\nAbsolute next predicted event:  ";
	printTime(averageDifferenceHours - hoursList[0]);
	float absNextTime = abs(averageDifferenceHours - hoursList[0]);

	tempAvgDiff = averageDifferenceHours - hoursList[0];
	if (tempAvgDiff < 0 && averageDifferenceHours != 0)
	{
		while (tempAvgDiff < 0)
			tempAvgDiff += averageDifferenceHours;
	}
	std::cout << "\nRealistic next predicted event: ";
	if (averageDifferenceHours == 0)
		std::cout << "undefined.";
	else
		printTime(tempAvgDiff);

	std::cout << "\nAverage difference:             ";
	printTime(averageDifferenceHours);
	std::cout << "\n\nAD = " << averageMean << " hours\n";
	std::cout << "SD = " << standardDeviation << " hours.\n";
	std::cout << "Mean = " << averageDifferenceHours << " hours.\n";
	prevAvgDiff = averageDifferenceHours;
	averageDifferenceHours *= hoursList.size();
	averageDifferenceHours += hoursList[0];
	averageDifferenceHours /= hoursList.size() + 1;
	std::cout << "STC: " << averageDifferenceHours - prevAvgDiff << " hours. (" << averageDifferenceHours << "h)\n";
	std::cout << "Irregular: ";
	if (absNextTime > standardDeviation)
		std::cout << "YES.\n";
	else
		std::cout << "NO.\n";
	return 0;
}
