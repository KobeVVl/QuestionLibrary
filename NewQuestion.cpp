#include "Arduino.h"
#include "NewQuestion.h"
#include "Regexp.h"

char operators[] = {')', '(', '^', '/', '*', '+', '-'};
uint8_t operatorsLength = sizeof(operators) / sizeof(operators[0]);

const unsigned int easyMax = 10;
const unsigned int mediumMax = 25;
const unsigned int hardMax = 50;
const unsigned int expMax = 3;

// Just default class
NewQuestion::NewQuestion() {}

// Generates a question with given difficulty and question.
NewQuestion::NewQuestion(Difficulty diff, String quest)
{
    // if (diff != EASY || diff != MEDIUM || diff != HARD) {
    //     throw "Difficulty must be EASY, MEDIUM or HARD";
    // }
    _difficulty = diff;
}

// Generates a question with given difficulty and random question.
NewQuestion::NewQuestion(Difficulty diff)
{
    uint8_t depth = 0;
    if (diff == EASY || diff == MEDIUM)
    {
        depth = 1;
    }
    else if (diff == HARD)
    {
        depth = random(1, 3);
    }
    else
    {
        depth = random(2, 4);
    }
    _depth = depth;
    _difficulty = diff;
    uint8_t min_index = 0;
    int index = 6;
    if (diff == EASY)
    {
        index = 5;
    }
    else if ((depth - diff) == 2)
    {
        index = random(max(2, min_index), operatorsLength);
    }
    else if ((depth - diff) < 1)
    {

        index = random(max(4, min_index), operatorsLength);
    }
    else if ((depth - diff) < 2)
    {
        index = random(max(3, min_index), operatorsLength);
    }
    _operator = operators[index];
    int8_t value1;
    int8_t value2;
    if (_operator == '*')
    {
        value1 = random(1, 10);
        value2 = random(1, 10);
    }
    else if (_operator == '/')
    {
        value2 = random(1, 10);
        value1 = value2 * random(1, 10);
        _depth -= 1;
    }
    else
    {
        int max = (diff == EASY) ? easyMax : ((diff == MEDIUM) ? mediumMax : hardMax);
        value1 = random(1, max);
        value2 = random(1, max);
    }
    if (_depth == 1) // Depth of 1 is f.e. 1+1 so both first and second terms are a value
    {

        _firstTerm = new NewQuestion(value1);
        _secondTerm = new NewQuestion(value2);
    }
    else if (_depth == 2)
    {
        _firstTerm = new NewQuestion(value1);
        _secondTerm = new NewQuestion(_difficulty, 1, _operator);
    }
    else if (_depth % 2 == 1)
    {
        _firstTerm = new NewQuestion(_difficulty, (_depth - 1) / 2, _operator);
        _secondTerm = new NewQuestion(_difficulty, (_depth - 2) / 2, _operator);
    }
    else
    {
        _firstTerm = new NewQuestion(_difficulty, _depth / 2, _operator);
        _secondTerm = new NewQuestion(_difficulty, _depth / 2 - 1, _operator);
        if (random(2) == 0)
        {
            NewQuestion *temp = _firstTerm;
            _firstTerm = _secondTerm;
            _secondTerm = temp;
        }
    }
    rearangeNegative();
}

NewQuestion::NewQuestion(Difficulty diff, uint8_t depth, char last_op)
{
    _depth = depth;
    _difficulty = diff;
    uint8_t min_index = 0;
    if (last_op == '+' || last_op == '-')
        min_index = 5;
    else if (last_op == '*')
        min_index = 4;
    int index = 6;
    if ((depth - diff) == 3)
    {
        index = random(max(2, min_index), operatorsLength);
    }
    else if ((depth - diff) < 2)
    {

        index = random(max(4, min_index), operatorsLength);
    }
    else if ((depth - diff) < 3)
    {
        index = random(max(3, min_index), operatorsLength);
    }
    _operator = operators[index];
    int8_t value1;
    int8_t value2;
    if (_operator == '*')
    {
        value1 = random(1, 10);
        value2 = random(1, 10);
    }
    else if (_operator == '/')
    {
        value2 = random(1, 10);
        value1 = value2 * random(1, 10);
        _depth -= 1;
    }
    else
    {
        if (diff == EASY)
        {
            value1 = random(1, 50);
            value2 = random(1, 50);
        }
        else
        {
            value1 = random(1, 100);
            value2 = random(1, 100);
        }
    }
    if (_depth == 1) // Depth of 1 is f.e. 1+1 so both first and second terms are a value
    {

        _firstTerm = new NewQuestion(value1);
        _secondTerm = new NewQuestion(value2);
    }
    else if (_depth == 2)
    {
        _firstTerm = new NewQuestion(value1);
        _secondTerm = new NewQuestion(_difficulty, 1, _operator);
    }
    else if (_depth % 2 == 1)
    {
        _firstTerm = new NewQuestion(_difficulty, (_depth - 1) / 2, _operator);
        _secondTerm = new NewQuestion(_difficulty, (_depth - 2) / 2, _operator);
    }
    else
    {
        _firstTerm = new NewQuestion(_difficulty, _depth / 2, _operator);
        _secondTerm = new NewQuestion(_difficulty, _depth / 2 - 1, _operator);
        if (random(2) == 0)
        {
            NewQuestion *temp = _firstTerm;
            _firstTerm = _secondTerm;
            _secondTerm = temp;
        }
    }
    rearangeNegative();
}

// Generates a value question with the given value
NewQuestion::NewQuestion(int8_t value)
{
    _depth = 0; // Depth of 0 is just value f.e. 5
    _value = value;
}

int NewQuestion::getValue()
{
    return getAnswer();
}

void NewQuestion::rearangeNegative()
{
    if (_operator == '-' && _firstTerm->getValue() < _secondTerm->getValue())
    {
        NewQuestion *tempTerm = _firstTerm;
        _firstTerm = _secondTerm;
        _secondTerm = tempTerm;
    }
}

char NewQuestion::getOperator()
{
    return _operator;
}

int NewQuestion::getDepth()
{
    int depth = _depth;
    return depth;
}

// Returns the question as a string type.
String NewQuestion::getQuestion()
{
    if (_depth == 0)
    {
        return String(_value);
    }
    // return '(' + _firstTerm->getQuestion() + _operator + _secondTerm->getQuestion() + ')';
    return _firstTerm->getQuestion() + _operator + _secondTerm->getQuestion();
}

// Returns the difficulty of this question.
uint8_t NewQuestion::getDifficulty()
{
    return _difficulty;
}

String NewQuestion::createQuestion(uint8_t diff)
{
    String quest = "";
    return quest;
}

int NewQuestion::calculateAnswer(String question)
{
    int index1 = question.indexOf('(');
    if (index1 != -1)
    {
        int index2 = get_second_bracket_index(question, index1);
        if (index2 != -1)
        {
            String first_part = question.substring(0, index1);
            String sub_part = question.substring(index1 + 1, index2);
            String last_part = question.substring(index2 + 1);
            String new_question = first_part + String(calculateAnswer(sub_part)) + last_part;
            return calculateAnswer(new_question);
        }
    }
    else
    {
        int index = question.lastIndexOf("-");
        if (index != -1)
            return (calculateAnswer(question.substring(0, index)) - calculateAnswer(question.substring(index + 1)));
        index = question.lastIndexOf("+");
        if (index != -1)
            return (calculateAnswer(question.substring(0, index)) + calculateAnswer(question.substring(index + 1)));
        index = question.lastIndexOf("*");
        if (index != -1)
            return (calculateAnswer(question.substring(0, index)) * calculateAnswer(question.substring(index + 1)));
        index = question.lastIndexOf("/");
        if (index != -1)
            return (calculateAnswer(question.substring(0, index)) / calculateAnswer(question.substring(index + 1)));
        return question.toInt();
    }
    return 0;
}

// Returns the answer to this question.
int NewQuestion::getAnswer()
{
    return calculateAnswer(getQuestion());
}

// Checks if a given input (answer) is the answer to this Newquestion.
bool NewQuestion::checkAnswer(int answer)
{
    return getAnswer() == answer;
}

String NewQuestion::calculateArray(String *array, int length)
{
    int index;
    String solved;
    // Reken haakjes uit
    if (length == 1)
    {
        return array[0];
    }
    else if (positionString(array, length, "(") != -1)
    {
        index = positionString(array, length, "(");
        int backIndex = positionString(array, length, ")");
        String subArr[backIndex - index - 1];
        subArray(array, index + 1, backIndex, subArr);

        solved = calculateArray(subArr, backIndex - index - 1);

        String newArray[length - (backIndex - index)];
        solvedArray(array, length, index, backIndex, solved, newArray);
        return calculateArray(newArray, length - (backIndex - index));
    }
    else if (positionString(array, length, "^") != -1)
    {
        index = positionString(array, length, "^");
        solved = String(round(pow(array[index - 1].toInt(), array[index + 1].toInt())));

        String newArray[length - 2];
        solvedArray(array, length, index - 1, index + 1, solved, newArray);
        return calculateArray(newArray, length - 2);
    }
    else if (positionString(array, length, "*") != -1)
    {
        index = positionString(array, length, "*");
        solved = String(array[index - 1].toInt() * array[index + 1].toInt());

        String newArray[length - 2];
        solvedArray(array, length, index - 1, index + 1, solved, newArray);
        return calculateArray(newArray, length - 2);
    }
    else if (positionString(array, length, "/") != -1)
    {
        index = positionString(array, length, "/");
        solved = String(round(array[index - 1].toInt() / array[index + 1].toInt()));

        String newArray[length - 2];
        solvedArray(array, length, index - 1, index + 1, solved, newArray);
        return calculateArray(newArray, length - 2);
    }
    else if (positionString(array, length, "+") != -1)
    {
        index = positionString(array, length, "+");
        solved = String(array[index - 1].toInt() + array[index + 1].toInt());

        String newArray[length - 2];
        solvedArray(array, length, index - 1, index + 1, solved, newArray);
        return calculateArray(newArray, length - 2);
    }
    else if (positionString(array, length, "-") != -1)
    {
        index = positionString(array, length, "-");
        solved = String(array[index - 1].toInt() - array[index + 1].toInt());

        String newArray[length - 2];
        solvedArray(array, length, index - 1, index + 1, solved, newArray);
        return calculateArray(newArray, length - 2);
    }
    return "NaN";
}

// Returns the first accurance of a string in an array of strings (if the string is ")" it returns the first starting from the back)
int NewQuestion::positionString(String *array, int length, String string)
{
    if (string == ")")
    {
        for (int index = length; index > 0; index--)
        {
            if (array[index - 1] == string)
                return index - 1;
        }
    }
    else
    {
        for (int index = 0; index < length; index++)
        {
            if (array[index] == string)
                return index;
        }
    }
    return -1;
}

bool charInString(String string, char c)
{
    for (int i = 0; i < string.length(); i++)
    {
        if (string[i] == c)
            return true;
    }
    return false;
}

bool NewQuestion::isOperator(char c)
{
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++)
    {
        if (operators[i] == c)
            return true;
    }
    return false;
}

// Puts a subarray from the parrentArray from minIndex (included) to maxIndex (excluded) in childArray
void NewQuestion::subArray(String *parentArray, int minIndex, int maxIndex, String *childArray)
{
    for (int i = 0; i < maxIndex - minIndex; i++)
    {
        childArray[i] = parentArray[minIndex + i];
    }
}

// Replaces part from minIndex (included) to maxIndex (included) with one String
void NewQuestion::solvedArray(String *parentArray, int length, int minIndex, int maxIndex, String solved, String *childArray)
{
    for (int i = 0; i < minIndex; i++)
    {
        childArray[i] = parentArray[i];
    }
    childArray[minIndex] = solved;
    for (int i = maxIndex + 1; i < length; i++)
    {
        childArray[i - maxIndex + minIndex] = parentArray[i];
    }
}

// Returns random int in between min (included) and max (excluded)
int NewQuestion::randomInt(int min, int max)
{
    int tempInt = millis() % max;
    if (tempInt < min)
    {
        int x = tempInt % (max - min);
        tempInt = min + x;
    }
    return tempInt;
}

int NewQuestion::get_second_bracket_index(String string, int first_bracket_index = 0)
{
    int count = 0;
    for (int i = first_bracket_index; i < string.length(); i++)
    {
        if (string[i] == '(')
            count++;
        else if (string[i] == ')')
            count--;
        if (count == 0)
            return i;
    }
    return -1;
}