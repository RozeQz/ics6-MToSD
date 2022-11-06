#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <iomanip>

using namespace std;

// Интерфейсы /////////////////////////////////////////////////////////////////

struct Client
{
    string name;
    string email;
    string phone_number;
};

class IOrder
{
public:
    virtual ~IOrder() = default;

    virtual bool isPaid() const = 0;
    virtual unsigned int getNumber() const = 0;
    virtual Client getClient() const = 0;
};

class IContact
{
public:
    virtual ~IContact() = default;

    virtual vector<pair<string, shared_ptr<IOrder>>> getClientsToContact(vector<shared_ptr<IOrder>> &orders) const = 0;
    virtual void contactWithClient(shared_ptr<Client> client) = 0;
};

class IOutput
{
public:
    virtual ~IOutput() = default;

    virtual void displayOrderList(vector<pair<string, shared_ptr<IOrder>>> &orders) = 0;
};

class IReport
{
public:
    virtual ~IReport() = default;

    virtual void doReport(vector<pair<string, shared_ptr<IOrder>>> &orders, IOutput &out) = 0;
};

// Реализация интерфейсов /////////////////////////////////////////////////////

class Order : public IOrder
{
private:
    const unsigned int _full_price, _paid_price, _number;
    const Client _client;

public:
    Order(unsigned int full_price, unsigned int paid_price, unsigned int number, Client client) : _full_price(full_price),
                                                                                                  _paid_price(paid_price),
                                                                                                  _number(number),
                                                                                                  _client(client)
    {
        assert(paid_price <= full_price);
    };

    bool isPaid() const override
    {
        return _paid_price == _full_price;
    };

    unsigned int getNumber() const override
    {
        return _number;
    };

    Client getClient() const override
    {
        return _client;
    };
};

class Manager
{
private:
    vector<shared_ptr<IOrder>> _list_of_orders;

public:
    Manager(vector<shared_ptr<IOrder>> list_of_orders) : _list_of_orders(list_of_orders){};

    vector<shared_ptr<IOrder>> getUnpaidOrders()
    {
        vector<shared_ptr<IOrder>> result;
        for (auto &order : _list_of_orders)
        {
            if (!order->isPaid())
            {
                result.push_back(order);
            }
        }
        return result;
    }
};

class EmailContact : public IContact
{
public:
    static vector<pair<string, shared_ptr<IOrder>>> getClientsToContact(vector<shared_ptr<IOrder>> &orders)
    {
        vector<pair<string, shared_ptr<IOrder>>> result;
        for (auto &order : orders)
        {
            result.push_back(make_pair(order->getClient().email, order));
        }
        return result;
    };

    static void contactWithClient(shared_ptr<Client> client)
    {
        // Связаться с клиентом по email
    }
};

class PhoneContact : public IContact
{
public:
    static vector<pair<string, shared_ptr<IOrder>>> getClientsToContact(vector<shared_ptr<IOrder>> &orders)
    {
        vector<pair<string, shared_ptr<IOrder>>> result;
        for (auto &order : orders)
        {
            result.push_back(make_pair(order->getClient().phone_number, order));
        }
        return result;
    };

    static void contactWithClient(shared_ptr<Client> client)
    {
        // Связаться с клиентом по телефону
    }
};

class Report : public IReport
{
public:
    virtual void doReport(vector<pair<string, shared_ptr<IOrder>>> &orders, IOutput &out) override
    {
        out.displayOrderList(orders);
    };
};

class ConsoleOutput : public IOutput
{
public:
    virtual void displayOrderList(vector<pair<string, shared_ptr<IOrder>>> &orders) override
    {
        cout << endl
             << "\t\t**** Список заказов ****" << endl
             << endl;
        cout << left
             << setw(16) << "Номер заказа"
             << setw(16) << "Статус заказа"
             << setw(22) << "Связь с клиентом"
             << "Имя клиента" << endl;

        for (auto &order : orders)
        {
            cout << left << setw(16) << order.second->getNumber() << setw(16);
            if (!order.second->isPaid())
                cout << "Не оплачен!";
            else
                cout << "Оплачен";
            cout << setw(22) << order.first << order.second->getClient().name;
            cout << endl;
        }
        cout << endl
             << endl;
    }
};

// Использование //////////////////////////////////////////////////////////////

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    vector<Client> clients{
        {
            "Лиза",
            "lizapizza@yandex.ru",
            "+79851234567",
        },
        {
            "Леша",
            "lexus@mail.ru",
            "+79657654321",
        },
        {"Александр",
         "fonparin@bmstu.ru",
         "+76666666666"},
        {"Вова",
         "vv@bmstu.ru",
         "+79853025468"},
        {"Полина",
         "polishgirl@hot.ru",
         "+71234567890"},
    };

    vector<shared_ptr<IOrder>> orders = {};

    for (int i = 0; i < clients.size(); i++)
    {
        unsigned int full_price = rand() % 10000 + 1;
        unsigned int paid_price = full_price * (rand() % 2);
        unsigned int id = rand() % 1000;
        shared_ptr<IOrder> order = make_unique<Order>(full_price, paid_price, id, clients[i]);
        orders.push_back(order);
    };

    vector<pair<string, shared_ptr<IOrder>>> list_of_orders;
    ConsoleOutput console_output;
    Report report;

    list_of_orders = EmailContact::getClientsToContact(orders);
    report.doReport(list_of_orders, console_output);

    Manager manager(orders);
    vector<shared_ptr<IOrder>> unpaid_orders = manager.getUnpaidOrders();

    list_of_orders = EmailContact::getClientsToContact(unpaid_orders);
    report.doReport(list_of_orders, console_output);

    list_of_orders = PhoneContact::getClientsToContact(unpaid_orders);
    report.doReport(list_of_orders, console_output);

    return 0;
}
