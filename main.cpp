#include "forwardlist.h"
#include<vector>

int main()
{
    ForwardList<int> fl;

    fl.push_front(1);
    fl.push_front(6);
    fl.push_front(8);
    fl.push_front(7);
    fl.insert_after(fl.begin(), 2);

    auto printList = [] <typename ListLike> (const ListLike &list) {
        for(auto val : list){
            std::cout << val << ' ';
        }
        std::cout << std::endl;
    };

    printList(fl);
    fl.sort();
    printList(fl);
    fl.reverse();
    printList(fl);

    std::cin.get();
}