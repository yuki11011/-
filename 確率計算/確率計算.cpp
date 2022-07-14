#include <stdio.h>
#include <iostream>
#include <random>
#include <fstream>
#include <windows.h>

#define RED_BALL 3
#define WHITE_BALL 4
#define BALLS_QUANTITY RED_BALL + WHITE_BALL
#define FETCH_TIMES 2
#define TRY_TIMES 100000


void initBoolArray(bool* settingArray, int arraySize) {
    if (settingArray == nullptr){
        return 1;
    }
    for (int arrayElement = 0; arrayElement < arraySize; ++arrayElement) {
        settingArray[arrayElement] = false;
    }
}

int main() {
    std::ofstream outputFile("probability_data.json", std::ios::app);
    outputFile << "[";

    bool* ballsArray;
    double* whiteBallProbability;

    ballsArray = new bool[BALLS_QUANTITY];
    whiteBallProbability = new double[TRY_TIMES];
    bool shouldWriteNum = false;
    double loopedWhiteBallQuantity = 0;

    std::random_device seedGen;
    std::default_random_engine engine(seedGen());
    std::uniform_int_distribution<> dist(0, BALLS_QUANTITY - 1);

    HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode = 0;
    GetConsoleMode(stdOut, &consoleMode);
    consoleMode = consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(stdOut, consoleMode);

    for (int tryTimes = 0; tryTimes < TRY_TIMES; ++tryTimes) {
        initBoolArray(ballsArray, BALLS_QUANTITY);

        for (int fetchTimes = 0; fetchTimes < FETCH_TIMES; ++fetchTimes) {
            int randBall = dist(engine);

            if (ballsArray[randBall] != true) {
                ballsArray[randBall] = true;
                if (randBall >= RED_BALL && fetchTimes == FETCH_TIMES - 1) {
                    loopedWhiteBallQuantity++;
                }
            } else {
                --tryTimes;
                shouldWriteNum = true;
                break;
            }
        }
        if (!shouldWriteNum) {
           whiteBallProbability[tryTimes] =
                loopedWhiteBallQuantity / ((tryTimes + 1)* FETCH_TIMES);
        }
        shouldWriteNum = false;
    }

    for (int i = 0; i < TRY_TIMES; ++i) {
       std::cout << whiteBallProbability[i] << ", " << i + 1 << std::endl;
        outputFile << whiteBallProbability[i] << ",";
    }

    std::cout << "\x1B[33;1m**********************" << std::endl;
    std::cout << "calculation has ended" << std::endl;
    std::cout << "**********************\x1B[37;m" << std::endl;
    outputFile << "]";

    outputFile.close();
    delete[] ballsArray;
    delete[] whiteBallProbability;
}
