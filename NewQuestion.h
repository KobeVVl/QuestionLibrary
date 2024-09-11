#ifndef NewQuestion_h
#define NewQuestion_h

#include "Arduino.h"

// #ifndef HARD
// #define HARD 3
// #endif
// #ifndef MEDIUM
// #define MEDIUM 2
// #endif
// #ifndef EASY
// #define EASY 1
// #endif

enum Difficulty
{
    EASY,
    MEDIUM,
    HARD
};

class NewQuestion
{
private:
    Difficulty _difficulty;

    NewQuestion *_firstTerm;
    NewQuestion *_secondTerm;
    char _operator;
    uint8_t _depth;

    int8_t _value;

    String createQuestion(uint8_t diff);
    int calculateAnswer(String question);
    bool isOperator(char c);
    String calculateArray(String *array, int length);
    int positionString(String *array, int length, String string);
    void subArray(String *parentArray, int minIndex, int maxIndex, String *childArray);
    void solvedArray(String *parentArray, int length, int minIndex, int maxIndex, String solved, String *childArray);
    int randomInt(int min, int max);
    int get_second_bracket_index(String string, int first_bracket_index);
    void rearangeNegative();

public:
    NewQuestion();
    NewQuestion(Difficulty diff, String quest);
    NewQuestion(Difficulty diff);
    NewQuestion(Difficulty diff, uint8_t depth, char last_op);
    NewQuestion(int8_t value);

    int getValue();
    char getOperator();
    int getDepth();

    int getAnswer();
    bool checkAnswer(int answer);

    String getQuestion();
    uint8_t getDifficulty();
};

#endif