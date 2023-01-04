#include "hw/l2_ApplicationLayer.h"

#include <algorithm>
#include <thread>

const int OUTPUT_LIMIT = 1000;

void Application::work()
{
    std::vector<std::string> args = split(_command);
    if (args.empty())
        return;

    // Используется для проверки заполнения очереди.
    // Очередь слишком быстро опустошалась, пытался понять где тормозит.
    // std::this_thread::sleep_for(std::chrono::microseconds(10000));

    // count
    if (args[0] == "c" || args[0] == "count")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды count");
            return;
        }

        _out.Output(std::to_string(_col.getSize()));
        return;
    }

    if (args[0] == "a" || args[0] == "add")
    {
        if (args.size() != 9)
        {
            _out.Output("Некорректное количество аргументов команды add");
            return;
        }

        std::string email = args[7];
        Subscriber sub(email);
        bool paid;
        if (args[8] == "true")
        {
            paid = true;
        }
        else
        {
            paid = false;
        }

        _col.addItem(std::make_shared<Subscribtion>(stoul(args[1]), args[2].c_str(), args[3].c_str(), stoul(args[4]), stoul(args[5]), stoul(args[6]), sub, paid));
        return;
    }

    if (args[0] == "r" || args[0] == "remove")
    {
        if (args.size() != 2)
        {
            _out.Output("Некорректное количество аргументов команды remove");
            return;
        }

        _col.removeItem(stoul(args[1]));
        return;
    }

    if (args[0] == "u" || args[0] == "update")
    {
        if (args.size() != 10)
        {
            _out.Output("Некорректное количество аргументов команды update");
            return;
        }

        std::string email = args[8];
        Subscriber sub(email);
        bool paid;
        if (args[9] == "true")
        {
            paid = true;
        }
        else
        {
            paid = false;
        }

        _col.updateItem(stoul(args[1]), std::make_shared<Subscribtion>(stoul(args[2]), args[3].c_str(), args[4].c_str(), stoul(args[5]), stoul(args[6]), stoul(args[7]), sub, paid));
        return;
    }

    if (args[0] == "v" || args[0] == "view")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды view");
            return;
        }

        size_t count = 0;
        for (size_t i = 0; i < _col.getSize(); ++i)
        {
            const Subscribtion &item = static_cast<Subscribtion &>(*_col.getItem(i));
            std::string paid;

            if (item.isPaid())
            {
                paid = "true";
            }
            else
            {
                paid = "false";
            }

            if (!_col.isRemoved(i))
            {
                _out.Output("[" + std::to_string(i) + "] " + std::to_string(item.getId()) + " " + item.getName() + " " + item.getLanguage() + " " + std::to_string(item.getDifficulty()) + " " + std::to_string(item.getDuration()) + " " + std::to_string(item.getCost()) + " " + item.getSub().getEmail() + " " + paid);
                count++;
            }
        }

        _out.Output("Количество элементов в коллекции: " + std::to_string(count));
        return;
    }

    if (args[0] == "report")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды report");
            return;
        }

        std::vector<std::shared_ptr<Subscribtion>> subs;

        for (size_t i = 0; i < _col.getSize(); ++i)
        {
            const Subscribtion &item = static_cast<Subscribtion &>(*_col.getItem(i));

            if (!_col.isRemoved(i))
            {
                subs.push_back(std::make_shared<Subscribtion>(item));
            }
        }

        Manager man(subs);

        for (const auto &unpaid_sub : man.getUnpaidSubscribtions())
        {
            _out.Output("ID Подписки: " + std::to_string(unpaid_sub->getId()));
            _out.Output("\tОтправляем электронное письмо по адресу... \t" + unpaid_sub->getSub().getEmail());
        }

        return;
    }

    _out.Output("Недопустимая команда '" + args[0] + "'");
    return;
}
