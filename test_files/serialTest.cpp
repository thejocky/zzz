#include <zzz.hpp>
#include <Meta.h>

#include <fstream>

struct Price {
    uint32_t price;
    char currency[4];
};

struct Address {
    std::string name;
    std::string address;
    std::string city;
    char* state;
    uint32_t zip;
};

class Purchase {
    std::string name_;
    uint32_t productNum_;
    Price price_;
    Address shipTo_;
    Address billTo_;

    public:

    Purchase() :
        name_(""), productNum_(0), price_(),
        shipTo_(), billTo_()
    {}

    Purchase(std::string name, uint32_t productNum, Price price,
            Address shippingAddress, Address billingAddress) :
        name_(name), productNum_(productNum), price_(price),
        shipTo_(shippingAddress), billTo_(billingAddress)
    {}

    void setName(const std::string& name) {name_ = name;}
    void setProductNum(uint32_t number) {productNum_ = number;}
    void setPrice(Price price) {price_ = price;}
    void setShippingAddress(const Address& address) {shipTo_ = address;}
    void setBillingAddress(const Address& address) {billTo_ = address;}

    const std::string& name() const {return name_;}
    uint32_t productNum() const {return productNum_;}
    Price price() const {return price_;}
    const Address& billingAddress() const {return billTo_;}
    const Address& shippingAddress() const {return shipTo_;}

};

bool operator== (const Price& p1, const Price& p2)
{
    return (p1.price == p2.price &&
            p1.currency[0] == p2.currency[0] &&
            p1.currency[1] == p2.currency[1] &&
            p1.currency[2] == p2.currency[2]);
}

bool operator== (const Address& a1, const Address& a2)
{
    return (a1.name == a2.name &&
            a1.address == a2.address &&
            a1.city == a2.city &&
            !strcmp(a1.state, a2.state) &&
            a1.zip == a2.zip);
}

bool operator== (const Purchase& p1, const Purchase& p2)
{
    return ((p1.name() == p2.name()) &&
            (p1.productNum() == p2.productNum()) &&
            (p1.price() == p2.price()) &&
            (p1.shippingAddress() == p2.shippingAddress()) &&
            (p1.billingAddress() == p2.billingAddress()));
}


namespace meta {

    template <>
    inline auto registerMembers<Address>() {
        return members(
            member("Name", &Address::name),
            member("Address", &Address::address),
            member("City", &Address::city),
            member("State", &Address::state),
            member("Zip", &Address::zip)
        );
    }

    template <>
    inline auto registerMembers<Purchase>() {
        return members(
            member("Name", &Purchase::name, &Purchase::setName),
            member("Product Num", &Purchase::productNum, &Purchase::setProductNum),
            member("Price", &Purchase::price, &Purchase::setPrice),
            member("Ship To", &Purchase::shippingAddress, &Purchase::setShippingAddress),
            member("Bill To", &Purchase::billingAddress, &Purchase::setBillingAddress)
        );
    }
}

namespace zzz {
    void deserialize(Price& object, Node* objRoot) {
        Node* child = objRoot->firstChild();
        object.price = (stof(child->string())*100);
        if (child->firstChild()->string().length() == 3)
            strcpy(object.currency, child->firstChild()->string().c_str());
    }
}



int main() {
    // zzz::printNode(nullptr);
    zzz::Node* root = new zzz::Node("", nullptr, nullptr);
    // zzz::Node* root = new zzz::Node("", nullptr);
    zzz::Parser parser(root);



    std::ifstream stream;
    stream.open("../test_files/serialTest.zzz", std::ifstream::in);

    try {
        parser.parse(stream);
        // zzz::printNode(root);
    } catch (zzz::parser_error& e) {
        std::cerr << e.line() << ":" 
            << e.column() << ": "
            << "\033[1;31m" // Set color output to red and bold
            << e.what() << ": "
            << "\033[0m" // Return to normal text
            << e.info() << ".\n";
        return 1;
    } catch (zzz::assembling_error& e) {
        std::cerr 
            << "\033[1;31m" // Set color output to red and bold
            << e.what() << ": "
            << "\033[0m" // Return to normal text
            << e.info() << ": \n"
            << "String: " << e.nodeString() << "\n"
            << "Depth: " << e.nodeDepth() << "\n";
        return 1;
    }

    zzz::Node* integerNode = root->child("integer");
    assert(integerNode);
    int integer;
    zzz::deserialize<int>(integer, integerNode);
    assert(integer == 1002);

    zzz::Node* floatNode = root->child("float");
    assert(floatNode->firstChild()->string() == std::string("10.5"));
    float floatValue = zzz::deserialize<float>(floatNode);
    assert(floatValue == 10.5);

    zzz::Node* boolNode = root->child("bool");
    assert(boolNode->firstChild()->string() == "true");
    bool boolValue = zzz::deserialize<bool>(boolNode);
    assert(boolValue == true);

    zzz::Node* vectorNode = root->child("int vector");
    assert(vectorNode);
    std::vector<int> intVector = zzz::deserialize<std::vector<int>>(vectorNode);
    std::cout << (intVector == std::vector<int>{42, 100, 1000, -4}) << "\n";


    zzz::Node* purchaseNode = root->child("Purchase");
    assert(purchaseNode);
    Purchase purchase = zzz::deserialize<Purchase>(purchaseNode);
    Purchase targetPurchase{
        "John Smith",
        20223,
        {2395, "USD"},
        {"Jane Smith",
        "123 Maple Street",
        "Pretendville",
        "NY",
        12345},
        {"John Smith",
        "123 Maple Street",
        "Pretendville",
        "NY",
        12345}
    };
    std::cout << purchase.price().currency << "\n";
    assert(purchase == targetPurchase);

    
    return 0;
}