/* Программа-заготовка для домашнего задания
 */

#include "hw/l3_DomainLayer.h"

using namespace std;

Subscriber::Subscriber(const std::string email) : _email(email){};

const string Subscriber::getEmail() const
{
    return _email;
}

bool Subscribtion::invariant() const
{
    return _duration >= MIN_DURATION && _duration <= MAX_DURATION && _difficulty <= MAX_DIFFICULTY && !_name.empty() && _name.size() <= MAX_NAME_LENGTH && !_language.empty() && _language.size() <= MAX_LANGUAGE_LENGTH;
}

Subscribtion::Subscribtion(uint32_t id, const std::string &name, const std::string &language, uint8_t difficulty, uint16_t duration, uint32_t cost, Subscriber sub, bool paid)
    : _id(id), _name(name), _language(language), _difficulty(difficulty), _duration(duration), _cost(cost), _sub(sub), _paid(paid)
{
    assert(invariant());
}

const string &Subscribtion::getName() const
{
    return _name;
}

const string &Subscribtion::getLanguage() const
{
    return _language;
}

uint8_t Subscribtion::getDifficulty() const
{
    return _difficulty;
}

uint16_t Subscribtion::getDuration() const
{
    return _duration;
}

uint32_t Subscribtion::getCost() const
{
    return _cost;
}

uint32_t Subscribtion::getId() const
{
    return _id;
}

Subscriber Subscribtion::getSub() const
{
    return _sub;
}

bool Subscribtion::isPaid() const
{
    return _paid;
}

bool Subscribtion::write(ostream &os)
{
    writeNumber(os, _id);
    writeString(os, _name);
    writeString(os, _language);
    writeNumber(os, _difficulty);
    writeNumber(os, _duration);
    writeNumber(os, _cost);
    writeString(os, _sub.getEmail());
    if (_paid)
    {
        writeString(os, "paid");
    }
    else
    {
        writeString(os, "not paid");
    }

    return os.good();
}

Manager::Manager(std::vector<shared_ptr<Subscribtion>> _list_of_subs) : _list_of_subs(_list_of_subs){};

vector<shared_ptr<Subscribtion>> Manager::getUnpaidSubscribtions()
{
    vector<shared_ptr<Subscribtion>> result;
    for (auto &sub : _list_of_subs)
    {
        if (!sub->isPaid())
        {
            result.push_back(sub);
        }
    }
    return result;
}

shared_ptr<ICollectable> ItemCollector::read(istream &is)
{
    uint32_t id = readNumber<uint32_t>(is);
    string name = readString(is, MAX_NAME_LENGTH);
    string language = readString(is, MAX_LANGUAGE_LENGTH);
    uint8_t difficulty = readNumber<uint8_t>(is);
    uint16_t duration = readNumber<uint16_t>(is);
    uint32_t cost = readNumber<uint32_t>(is);
    string email = readString(is, 50);
    string paid_str = readString(is, 8);
    bool paid = (paid_str == "true");
    Subscriber sub = Subscriber(email);

    return std::make_shared<Subscribtion>(id, name, language, difficulty, duration, cost, sub, paid);
}
