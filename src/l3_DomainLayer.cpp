/* Программа-заготовка для домашнего задания
 */

#include "hw/l3_DomainLayer.h"

using namespace std;

bool Course::invariant() const
{
    return _duration >= MIN_DURATION && _duration <= MAX_DURATION && _difficulty <= MAX_DIFFICULTY && !_name.empty() && _name.size() <= MAX_NAME_LENGTH && !_language.empty() && _language.size() <= MAX_LANGUAGE_LENGTH;
}

Course::Course(const std::string &name, const std::string &language, uint8_t difficulty, uint16_t duration, uint32_t cost)
    : _name(name), _language(language), _difficulty(difficulty), _duration(duration), _cost(cost)
{
    assert(invariant());
}

const string &Course::getName() const
{
    return _name;
}

const string &Course::getLanguage() const
{
    return _language;
}

uint8_t Course::getDifficulty() const
{
    return _difficulty;
}

uint16_t Course::getDuration() const
{
    return _duration;
}

uint32_t Course::getCost() const
{
    return _cost;
}

bool Course::write(ostream &os)
{
    writeString(os, _name);
    writeString(os, _language);
    writeNumber(os, _difficulty);
    writeNumber(os, _duration);
    writeNumber(os, _cost);

    return os.good();
}

shared_ptr<ICollectable> ItemCollector::read(istream &is)
{
    string name = readString(is, MAX_NAME_LENGTH);
    string language = readString(is, MAX_LANGUAGE_LENGTH);
    uint8_t difficulty = readNumber<uint8_t>(is);
    uint16_t duration = readNumber<uint16_t>(is);
    uint32_t cost = readNumber<uint32_t>(is);

    return std::make_shared<Course>(name, language, difficulty, duration, cost);
}
