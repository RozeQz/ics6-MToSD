/* Программа-заготовка для домашнего задания
 */

#include "hw/l3_DomainLayer.h"

using namespace std;

bool Subscribtion::invariant() const
{
    return (_id >= 0) && !_name.empty() && !_language.empty() && (_difficulty >= 0) && (_cost >= 0);
}

Subscriber::Subscriber(const std::string email) : _email(email) {}

Subscriber::Subscriber(const Subscriber &s)
{
    _email = s.getEmail();
}

const string Subscriber::getEmail() const
{
    return _email;
}

Subscribtion::Subscribtion(int id, const std::string &name, const std::string &language, int difficulty, int duration, int cost, Subscriber sub, bool paid)
    : _id(id), _name(name), _language(language), _difficulty(difficulty), _duration(duration), _cost(cost), _sub(sub), _paid(paid)
{
    assert(invariant());
}

Subscribtion::Subscribtion(const Subscribtion &s) : _sub(s.getSub())
{
    _id = s.getId();
    _name = s.getName();
    _language = s.getLanguage();
    _difficulty = s.getDifficulty();
    _duration = s.getDuration();
    _cost = s.getCost();
    _paid = s.isPaid();
}

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
        writeString(os, "true");
    }
    else
    {
        writeString(os, "false");
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
    string name = readString(is);
    string language = readString(is);
    int difficulty = readNumber<int>(is);
    int duration = readNumber<int>(is);
    int cost = readNumber<int>(is);
    string email = readString(is);
    string paid_str = readString(is);
    bool paid = (paid_str == "true");
    Subscriber sub = Subscriber(email);

    return std::make_shared<Subscribtion>(id, name, language, difficulty, duration, cost, sub, paid);
}

Subscribtion &ItemCollector::getSubscribtion(size_t index)
{
    Subscribtion *p = static_cast<Subscribtion *>(getItem(index).get());

    assert(p != nullptr);

    return *p;
}
