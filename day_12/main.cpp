#include <iostream>

#include <map>

#include "helpers.h"

const char* INPUT = "input_example.txt";

struct DataRow
{
public:
    std::string m_data;
    std::vector<int> m_check;
};

std::vector<DataRow> readInput()
{
    std::ifstream file(INPUT);

    std::vector<DataRow> inputData;

    if(file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::vector<std::string> lineParts = split(line, ' ');
            std::vector<int> numbers = parseNumbers<int>(lineParts[1], ',');
            inputData.push_back({lineParts[0], numbers});
        }
    }

    return inputData;
}

bool isValidRow(const std::string& row, const DataRow& dataRow)
{
    std::vector<int> checksum;

    int i = 0;
    for (char ch : row)
    {
        if (ch == '.' && i != 0)
        {
            checksum.push_back(i);
            i = 0;
        }

        if (ch == '#')
        {
            i++;
        }
    }

    if (i != 0)
    {
        checksum.push_back(i);
    }

    return checksum == dataRow.m_check;
}

void generate(int size, const std::string& str, std::vector<std::string>& variants)
{
    if (size == str.size())
    {
        variants.push_back(str);
    }
    else
    {
        std::vector<char> chars;
        chars.push_back('.');
        chars.push_back('#');
        for(char ch : chars)
        {
            generate(size, str + ch, variants);
        }
    }
}

void generateTemplate(const std::string& strTemplate, int current, const std::map<int, std::vector<std::string>>& variants, const DataRow& row, std::vector<std::string>& result)
{
    int questions = std::count(strTemplate.begin(), strTemplate.end(), '?');
    if (isValidRow(strTemplate, row) && questions == 0)
    {
        result.push_back(strTemplate);
        return;
    }

    if(questions > 0)
    {
        std::vector<std::string> vars = variants.at(current);
        for (const std::string& variant : vars)
        {
            auto itNext = variants.find(current);
            itNext++;

            std::string nextTemplate = strTemplate;
            nextTemplate.replace(current, variant.size(), variant);
            generateTemplate(nextTemplate, itNext->first, variants, row, result);
        }
    }

}

std::vector<std::string> bruteforceVariants(const DataRow& row)
{
    std::vector<std::string> variants;
    std::string rowTemplate = row.m_data;

    std::vector<int> check = row.m_check;

    std::map<int, std::string> unknown;

    int index = -1;
    std::string question;
    for (int i = 0; i < rowTemplate.size(); ++i)
    {
        char ch = rowTemplate[i];
        if (ch == '?')
        {
            if (index < 0)
            {
                index = i;
            }
            question += ch;
        }
        else if (!question.empty())
        {
            unknown[index] = question;
            question.clear();
            index = -1;
        }
    }

    if (!question.empty())
    {
        unknown[index] = question;
        question.clear();
        index = -1;
    }

    std::map<int, std::vector<std::string>> unknownVariants;
    for (const auto& pair : unknown)
    {
        std::vector<std::string> vars;
        generate(pair.second.size(), "", vars);

        unknownVariants[pair.first] = vars;
    }

    generateTemplate(rowTemplate, unknownVariants.begin()->first, unknownVariants, row, variants);
    return variants;
}



long countVariants(std::vector<DataRow> rows)
{
    long result = 0;
    int i = 0;
    for (const DataRow& row : rows)
    {
        std::vector<std::string> variants = bruteforceVariants(row);
        result += variants.size();

        std::cout << "Progress: " << ++i << "/" << rows.size() << std::endl;
    }

    return result;
}

std::vector<DataRow> unfold(std::vector<DataRow> rows)
{
    for (int i = 0; i < rows.size(); ++i)
    {
        std::string newData = rows[i].m_data;
        for (int j = 0; j < 4; ++j)
        {
            newData += "?" + rows[i].m_data;
        }

        std::vector<int> newCheck = rows[i].m_check;
        for (int j = 0; j < 4; ++j)
        {
            newCheck.insert(newCheck.begin(), rows[i].m_check.begin(),  rows[i].m_check.end());
        }

        rows[i] = {newData, newCheck};
    }

    return rows;
}

int main()
{
    START_TIMER(point_1)

    std::vector<DataRow> input = readInput();

    bruteforceVariants(input[1]);
    std::cout << "Answer part 1: " << countVariants(input) << std::endl;


    std::vector<DataRow> input2 = unfold(input);
    std::cout << "Answer part 2: " << countVariants(input2) << std::endl;

    STOP_TIMER(point_1)
    return 0;
}
