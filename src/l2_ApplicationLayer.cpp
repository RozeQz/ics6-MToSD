/* Программа-заготовка для домашнего задания
 */

#include "hw/l2_ApplicationLayer.h"

using namespace std;

inline const string DATA_DEFAULT_NAME = "lab.data";

bool Application::performCommand(const vector<string> &args)
{
    if (args.empty())
        return false;

    if (args[0] == "l" || args[0] == "load")
    {
        string filename = (args.size() == 1) ? DATA_DEFAULT_NAME : args[1];

        if (!_col.loadCollection(filename))
        {
            _out.Output("Ошибка при загрузке файла '" + filename + "'");
            return false;
        }

        return true;
    }

    if (args[0] == "s" || args[0] == "save")
    {
        string filename = (args.size() == 1) ? DATA_DEFAULT_NAME : args[1];

        if (!_col.saveCollection(filename))
        {
            _out.Output("Ошибка при сохранении файла '" + filename + "'");
            return false;
        }

        return true;
    }

    if (args[0] == "c" || args[0] == "clean")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды clean");
            return false;
        }

        _col.clean();

        return true;
    }

    if (args[0] == "a" || args[0] == "add")
    {
        if (args.size() != 9)
        {
            _out.Output("Некорректное количество аргументов команды add");
            return false;
        }

        string email = args[7];
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

        _col.addItem(make_shared<Subscribtion>(stoul(args[1]), args[2].c_str(), args[3].c_str(), stoul(args[4]), stoul(args[5]), stoul(args[6]), sub, paid));
        return true;
    }

    if (args[0] == "r" || args[0] == "remove")
    {
        if (args.size() != 2)
        {
            _out.Output("Некорректное количество аргументов команды remove");
            return false;
        }

        _col.removeItem(stoul(args[1]));
        return true;
    }

    if (args[0] == "u" || args[0] == "update")
    {
        if (args.size() != 10)
        {
            _out.Output("Некорректное количество аргументов команды update");
            return false;
        }

        string email = args[8];
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

        _col.updateItem(stoul(args[1]), make_shared<Subscribtion>(stoul(args[2]), args[3].c_str(), args[4].c_str(), stoul(args[5]), stoul(args[6]), stoul(args[7]), sub, paid));
        return true;
    }

    if (args[0] == "v" || args[0] == "view")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды view");
            return false;
        }

        size_t count = 0;
        for (size_t i = 0; i < _col.getSize(); ++i)
        {
            const Subscribtion &item = static_cast<Subscribtion &>(*_col.getItem(i));
            string paid;

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
                _out.Output("[" + to_string(i) + "] " + to_string(item.getId()) + " " + item.getName() + " " + item.getLanguage() + " " + to_string(item.getDifficulty()) + " " + to_string(item.getDuration()) + " " + to_string(item.getCost()) + " " + item.getSub().getEmail() + " " + paid);
                count++;
            }
        }

        _out.Output("Количество элементов в коллекции: " + to_string(count));
        return true;
    }

    if (args[0] == "report")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды report");
            return false;
        }

        vector<shared_ptr<Subscribtion>> subs;

        for (size_t i = 0; i < _col.getSize(); ++i)
        {
            const Subscribtion &item = static_cast<Subscribtion &>(*_col.getItem(i));

            if (!_col.isRemoved(i))
            {
                subs.push_back(make_shared<Subscribtion>(item));
            }
        }

        Manager man(subs);

        for (const auto &unpaid_sub : man.getUnpaidSubscribtions())
        {
            _out.Output("ID Подписки: " + to_string(unpaid_sub->getId()));
            _out.Output("\tОтправляем электронное письмо по адресу... \t" + unpaid_sub->getSub().getEmail());
        }

        return true;
    }

    _out.Output("Недопустимая команда '" + args[0] + "'");
    return false;
}
