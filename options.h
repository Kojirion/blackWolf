#ifndef OPTIONS_H
#define OPTIONS_H

enum class optionType
{
    check,
    spin,
    combo,
    button,
    string
};

template <optionType T>
class option
{
public:
    std::string getName();
    optionType getType();

private:
    std::string name;
    optionType type;
};

class optionCheck : public option<optionType::check>
{
public:
    bool getDefault()
    {
        return defaultValue;
    }

private:
    bool defaultValue;
};

class optionSpin : public option<optionType::spin>
{
public:
    int getDefault()
    {
        return defaultValue;
    }

    int min()
    {
        return minValue;
    }

    int max()
    {
        return maxValue;
    }

private:
    int defaultValue;
    int minValue;
    int maxValue;
};

class optionString : public option<optionType::string>
{
public:
    std::string getDefault()
    {
        return defaultValue;
    }

private:
    std::string defaultValue;
};

class optionCombo : public option<optionType::combo>
{
public:
    std::string getDefault()
    {
        return defaultValue;
    }

private:
    std::string defaultValue;
    std::vector<std::string> values;
};

class optionButton : public option<optionType::button>
{

};

#endif // OPTIONS_H
