#include "forwardlist.h"
#include <limits>

using IntForwardList = ForwardList<int>;

class ListOperate
{
private:
    IntForwardList list;

    void clearInput() const{
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void pause(const char *str) const{
        std::cout << str << "，按任意键返回。。。";
        this->clearInput();
        std::cin.get();
    }

    void showList(const IntForwardList &fl) const{
        for(auto value : fl){
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    void showList() const { this->showList(this->list); }

    void orderedInsert(int value);

public:
    void menu();
    void create();
    void show() const;
    void sort();
    void add();
    void del();
    void reverse();
    void split() const;
    void clear();
};