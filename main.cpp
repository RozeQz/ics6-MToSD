#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <iomanip>
#include <memory>

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
    virtual vector<pair<string, shared_ptr<IOrder>>> getClientsToContact(vector<shared_ptr<IOrder>> &orders) const override
    {
        vector<pair<string, shared_ptr<IOrder>>> result;
        for (auto &order : orders)
        {
            result.push_back(make_pair(order->getClient().email, order));
        }
        return result;
    };

    // void contactWithClient(shared_ptr<Client> client)
    // {
    //     // Связаться с клиентом по email
    // }
};

class PhoneContact : public IContact
{
public:
    virtual vector<pair<string, shared_ptr<IOrder>>> getClientsToContact(vector<shared_ptr<IOrder>> &orders) const override
    {
        vector<pair<string, shared_ptr<IOrder>>> result;
        for (auto &order : orders)
        {
            result.push_back(make_pair(order->getClient().phone_number, order));
        }
        return result;
    };

    // void contactWithClient(shared_ptr<Client> client)
    // {
    //     // Связаться с клиентом по телефону
    // }
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
        cout << "Номер заказа"
             << "\tСтатус заказа"
             << "\tСвязь с клиентом"
             << "\tИмя клиента" << endl;

        for (auto &order : orders)
        {
            cout << order.second->getNumber() << "\t\t";
            if (!order.second->isPaid())
                cout << "Не оплачен";
            else
                cout << "Оплачен\t";
            cout << "\t" << order.first;
            if (order.first.length() < 14)
                cout << "\t\t";
            else
                cout << "\t";
            cout << order.second->getClient().name;
            cout << endl;
        }
        cout << endl
             << endl;
    }
};

// Использование //////////////////////////////////////////////////////////////

int main()
{
    // srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    vector<Client> clients{
        {
            "Лиза",
            "lizapizza@yandex.ru",
            "+79851234567",
        },
        {
            "Леша",
            "lexusrx300@mail.ru",
            "+79657654321",
        },
        {"Александр",
         "fonparin@bmstu.ru",
         "+76666666666"},
        {"Вова",
         "vvputin@bmstu.ru",
         "+79853025468"},
        {"Полина",
         "polishgirl@hot.ru",
         "+71234567890"},
    };

    vector<shared_ptr<IOrder>> orders = {};

    for (int i = 0; i < clients.size(); i++)
    {
        unsigned int full_price = (i + 1) * 10000;
        unsigned int paid_price = full_price * ((i + 1) % 2);
        unsigned int id = ((i + 1) * 100) + 47 * (i % 2) + i - 69;
        shared_ptr<IOrder> order = make_unique<Order>(full_price, paid_price, id, clients[i]);
        orders.push_back(order);
    };

    vector<pair<string, shared_ptr<IOrder>>> list_of_orders;
    ConsoleOutput console_output;
    EmailContact email_contact;
    PhoneContact phone_contact;
    Report report;

    list_of_orders = email_contact.getClientsToContact(orders);
    report.doReport(list_of_orders, console_output);

    list_of_orders = phone_contact.getClientsToContact(orders);
    report.doReport(list_of_orders, console_output);

    Manager manager(orders);
    vector<shared_ptr<IOrder>> unpaid_orders = manager.getUnpaidOrders();

    list_of_orders = email_contact.getClientsToContact(unpaid_orders);
    report.doReport(list_of_orders, console_output);

    list_of_orders = phone_contact.getClientsToContact(unpaid_orders);
    report.doReport(list_of_orders, console_output);

    return 0;
}
