#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <regex>
#include <algorithm>
#include <numeric>

#define OUT

void loadFile(const std::string& filename, OUT std::vector<std::string>& log)
{
    std::ifstream inputFile;
    inputFile.open(filename);
    if (inputFile.is_open())
    {
        log.clear();
        std::string line;
        while (std::getline(inputFile, line))
        {
            log.push_back(line);
        }
        std::sort(log.begin(), log.end());
    }
}

int getMinute(const std::string& timestamp)
{
    const std::regex minuteRegex(R"(00:([0-5][0-9]))");

    std::smatch match;
    if (std::regex_search(timestamp, match, minuteRegex))
    {
        return std::stoi(match[1]);
    }
    
    return -1;
}

void fillMap(const std::vector<std::string>& log, OUT std::unordered_map<int, std::vector<int>>& sleepMap)
{
    // Parsing constants
    const std::regex logEntryRegex(R"((\[.+\]) (.+))");
    const std::regex guardRegex(R"(Guard #(\d+) begins shift)");
    const std::string fallsAsleep = "falls asleep";
    const std::string wakesUp = "wakes up";

    int currentGuard = -1;
    int beginMinute;
    for (const std::string& entry : log)
    {
        std::smatch match;
        if (std::regex_search(entry, match, logEntryRegex))
        {
            std::string timestamp = match[1];
            std::string action = match[2];
            std::smatch submatch;

            if (action == fallsAsleep)
            {
                beginMinute = getMinute(timestamp);
            }
            else if (action == wakesUp)
            {
                int endMinute = getMinute(timestamp);
                std::vector<int>& guardVector = sleepMap[currentGuard];
                std::for_each(guardVector.begin() + beginMinute, guardVector.begin() + endMinute,
                    [](int& count) { count++; }
                );
            }
            else if (std::regex_search(action, submatch, guardRegex))
            {
                currentGuard = std::stoi(submatch[1]);
                if (!sleepMap.count(currentGuard))
                {
                    sleepMap[currentGuard] = std::vector<int>(60, 0);
                }
            }
        }
    }
}

int findSleepyHead(const std::unordered_map<int, std::vector<int>>& sleepMap)
{
    int bestCount = 0;
    int sleepyHead = -1;
    for (const auto& entry : sleepMap)
    {
        const std::vector<int>& timeTable = entry.second;
        int count = std::accumulate(timeTable.begin(), timeTable.end(), 0);
        if (count > bestCount)
        {
            bestCount = count;
            sleepyHead = entry.first;
        }
    }
    return sleepyHead;
}

void findSleepyHead2(const std::unordered_map<int, std::vector<int>>& sleepMap, OUT int& sleepyGuard, OUT int& sleepyMinute)
{
    int bestCount = 0;
    for (const auto& entry : sleepMap)
    {
        std::vector<int>::const_iterator thisSleepyMinutePos = std::max_element(entry.second.begin(), entry.second.end());
        int thisSleepyMinute = thisSleepyMinutePos - entry.second.begin();
        int thisSleepyMinuteCount = *thisSleepyMinutePos;

        if (thisSleepyMinuteCount > bestCount)
        {
            bestCount = thisSleepyMinuteCount;
            sleepyMinute = thisSleepyMinute;
            sleepyGuard = entry.first;
        }
    }
}

int main(int argc, char** argv)
{
    std::vector<std::string> log;
    loadFile("input.in", log);

    // Map, for each guard, each minute the times he was found asleep
    std::unordered_map<int, std::vector<int>> sleepMap;
    fillMap(log, sleepMap);

    int bestGuard = findSleepyHead(sleepMap);
    std::vector<int>& times = sleepMap[bestGuard];
    int sleepyMinute = std::max_element(times.begin(), times.end()) - times.begin();

    std::cout << (bestGuard * sleepyMinute) << std::endl;

    findSleepyHead2(sleepMap, bestGuard, sleepyMinute);
    std::cout << (bestGuard * sleepyMinute) << std::endl;

    std::cin.get();
}