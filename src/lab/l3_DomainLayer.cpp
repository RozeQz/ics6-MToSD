/* Программа-заготовка для домашнего задания
 */

#include "hw/l3_DomainLayer.h"

using namespace std;

Subscriber::Subscriber(const std::string email) : _email(email) {}

const string Subscriber::getEmail() const
{
    return _email;
}

Subscribtion::Subscribtion(int id, const std::string &name, const std::string &language, int difficulty, int duration, int cost, Subscriber sub, bool paid)
    : _id(id), _name(name), _language(language), _difficulty(difficulty), _duration(duration), _cost(cost), _sub(sub), _paid(paid) {}

const string &Subscribtion::getName() const
{
    return _name;
}

const string &Subscribtion::getLanguage() const
{
    return _language;
}

int Subscribtion::getDifficulty() const
{
    return _difficulty;
}

int Subscribtion::getDuration() const
{
    return _duration;
}

int Subscribtion::getCost() const
{
    return _cost;
}

int Subscribtion::getId() const
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

Manager::Manager(std::vector<shared_ptr<Subscribtion>> _list_of_subs) : _list_of_subs(_list_of_subs) {}

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
    int id = readNumber<int>(is);
    string name = readString(is, MAX_NAME_LENGTH);
    string language = readString(is, MAX_LANGUAGE_LENGTH);
    int difficulty = readNumber<int>(is);
    int duration = readNumber<int>(is);
    int cost = readNumber<int>(is);
    string email = readString(is, 50);
    string paid_str = readString(is, 8);
    bool paid = (paid_str == "true");
    Subscriber sub = Subscriber(email);

    return std::make_shared<Subscribtion>(id, name, language, difficulty, duration, cost, sub, paid);
}
