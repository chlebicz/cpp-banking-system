#include "model/Deposit.h"
#include "model/Bank.h"

Deposit::Deposit(const Amount &amount, const gr::date &beginTime)
    : amount(amount), beginTime(beginTime)
{
    Bank::getInstance().increaseBalance(amount);
}

Amount Deposit::getAmount() const {
    return amount;
}

gr::date Deposit::getBeginTime() const {
    return beginTime;
}

Amount Deposit::endDeposit() {
    gr::date endTime = gr::day_clock::local_day();
    int months = (endTime.year() - beginTime.year()) * 12
        + (endTime.month() - beginTime.month());
    for (int i = 0; i < months; i++) {
        Amount benefit(amount.getZloty()/50);
        amount = amount + benefit;
    }
    Bank::getInstance().decreaseBalance(amount);
    return amount;
}

std::string Deposit::toString() const {
    return "Amount: " + amount.toString()
        + " Start date: " + gr::to_simple_string(beginTime);
}

json Deposit::toJSON() const {
    return {
        {"beginTime", {
            {"year", static_cast<unsigned short>(beginTime.year())},
            {"month", static_cast<unsigned short>(beginTime.month())}
        }},
        {"amount", amount}
    };
}

std::shared_ptr<Deposit> Deposit::fromJSON(const json& j) {
    return std::make_shared<Deposit>(
        Amount::fromJSON(j["amount"]),
        gr::date(
            j["beginTime"]["year"].get<unsigned short>(),
            j["beginTime"]["month"].get<unsigned short>(),
            0
        )
    );
}
