#include "model/Loan.h"

#include "model/Bank.h"

Loan::Loan(
    int months, Amount amount, gr::date beginTime,
    std::shared_ptr<Account> operationalAccount
)
    : months(months), amount(amount), beginTime(beginTime),
      operationalAccount(std::move(operationalAccount))
{}

Loan::Loan(
    int months, Amount amount, gr::date beginTime,
    std::string operationalAccount
)
    : months(months), amount(amount), beginTime(beginTime),
      operationalAccountNumber(std::move(operationalAccountNumber))
{}

Loan::Loan(
    int months, Amount amount, gr::date beginTime,
    std::string operationalAccountNumber, Amount monthlyPayment
)
    : months(months), amount(amount), beginTime(beginTime),
      operationalAccountNumber(std::move(operationalAccountNumber)),
      monthlyPayment(monthlyPayment)
{}

void Loan::create() {
    Amount addition(
        amount.getZloty() * 0.1 * (months/12.0),
        amount.getGrosz() * 0.1 * (months/12.0)
    );
    Amount fullPayment = amount + addition;
    Amount payment(
        fullPayment.getZloty()*(1.0/months),
        fullPayment.getGrosz()*(1.0/months)
    );
    monthlyPayment = payment;
    Bank::getInstance().decreaseBalance(amount);

    auto operationalAccount = getOperationalAccount();
    operationalAccount->setBalance(operationalAccount->getBalance() + amount);
}

int Loan::getMonths() const {
    return months;
}

Amount Loan::getAmount() const {
    return amount;
}

Amount Loan::getMontlyPayment() const {
    return monthlyPayment;
}

int Loan::getRemainingMonths() const {
    gr::date now = gr::day_clock::local_day();
    int passedMonths = (now.year() - beginTime.year()) * 12
        + (now.month() - beginTime.month());
    return months - passedMonths;
}

gr::date Loan::getBeginTime() const {
    return beginTime;
}

std::shared_ptr<Account> Loan::getOperationalAccount() const {
    if (operationalAccount)
        return operationalAccount;

    auto fetched = Bank::getInstance().getAccountManager()
        .getAccount(operationalAccountNumber);
    return fetched;
}

bool Loan::takeMoney() {
    gr::date now = gr::day_clock::local_day();
    int passedMonths = (now.year() - beginTime.year()) * 12
        + (now.month() - beginTime.month());
    if (passedMonths > months)
        passedMonths = months;

    int monthsToPay = passedMonths - paidMonths;

    auto operationalAccount = getOperationalAccount();

    for (int i = 0; i < monthsToPay; i++) {
        try {
            operationalAccount->setBalance(
                operationalAccount->getBalance() - monthlyPayment
            );
        } catch (const InvalidAmountError&) {
            return false;
        }

        Bank::getInstance().increaseBalance(monthlyPayment);
        paidMonths++;
    }
    return true;
}

std::string Loan::toString() const {
    return "Loan taken for: " + amount.toString() + ", for "
        + std::to_string(months) + " months, monthly payment: "
        + monthlyPayment.toString() + ", paid months: "
        + std::to_string(paidMonths) + " months remaining to pay: "
        + std::to_string(getRemainingMonths());
}

json Loan::toJSON() const {
    std::string number = operationalAccountNumber;
    if (operationalAccount)
        number = operationalAccount->getId();

    return {
        {"months", months},
        {"amount", amount},
        {"beginTime", {
            {"year", static_cast<int>(beginTime.year())},
            {"month", static_cast<int>(beginTime.month())},
            {"day", static_cast<int>(beginTime.day())}
        }},
        {"operationalAccount", number},
        {"monthlyPayment", monthlyPayment}
    };
}

std::shared_ptr<Loan> Loan::fromJSON(const json& source) {
    auto beginTime = source["beginTime"];
    int year = beginTime["year"];
    int month = beginTime["month"];
    int day = beginTime["day"];

    return std::make_shared<Loan>(
        source["months"],
        Amount::fromJSON(source["amount"]),
        gr::date(year, month, day),
        source["operationalAccount"],
        Amount::fromJSON(source["monthlyPayment"])
    );
}
