#include "forwardlist.h"
#include<vector>

int main()
{
    ForwardList<int> fl;

    fl.push_back(9);
    fl.push_back(8);
    fl.insert(fl.begin(), 7);

    for(auto val : fl){
        std::cout << val << std::endl;
    }

    fl.clear();
    std::cout << fl.size() << std::endl;

    std::cin.get();
}