/*
MIT License

Copyright (c) 2019 МГТУ им. Н.Э. Баумана, кафедра ИУ-6, Михаил Фетисов,

Программа-заготовка для домашнего задания
*/

#ifndef HW_L3_DOMAIN_LAYER_H
#define HW_L3_DOMAIN_LAYER_H

#include "hw/l4_InfrastructureLayer.h"

#include <string>
#include <vector>

class Subscriber
{
    std::string _email;

public:
    Subscriber() = delete;
    Subscriber(const Subscriber &s);

    Subscriber &operator=(const Subscriber &s) = delete;

    Subscriber(const std::string email);

    const std::string getEmail() const;
};

class Subscribtion : public ICollectable
{
    int _id;
    std::string _name;
    std::string _language;
    int _difficulty;
    int _duration;
    int _cost;
    Subscriber _sub;
    bool _paid;

protected:
    bool invariant() const;

public:
    Subscribtion() = delete;

    Subscribtion &operator=(const Subscribtion &s) = delete;

    Subscribtion(const Subscribtion &s);

    Subscribtion(int _id, const std::string &_name, const std::string &_language, int _difficulty, int _duration, int _cost, Subscriber _sub, bool paid);

    int getId() const;
    const std::string &getName() const;
    const std::string &getLanguage() const;
    int getDifficulty() const;
    int getDuration() const;
    int getCost() const;
    Subscriber getSub() const;
    bool isPaid() const;

    virtual bool write(std::ostream &os) override;
};

class ItemCollector : public ACollector
{
public:
    virtual std::shared_ptr<ICollectable> read(std::istream &is) override;

    Subscribtion &getSubscribtion(size_t index);
};

class Manager : public ItemCollector
{
private:
    std::vector<std::shared_ptr<Subscribtion>> _list_of_subs;

public:
    Manager(std::vector<std::shared_ptr<Subscribtion>> _list_of_subs);

    std::vector<std::shared_ptr<Subscribtion>> getUnpaidSubscribtions();
};

#endif // HW_L3_DOMAIN_LAYER_H
