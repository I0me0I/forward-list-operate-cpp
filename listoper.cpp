#include "listoper.h"

// 将程序功能做成菜单，形式如下：

// 1、创建一条含正整数结点的无序链表
// 2、链表结点的输出
// 3、链表结点的升序排序
// 4、链表结点插入
// 5、链表结点删除
// 6、链表逆序（升序变降序）
// 7、链表拆分，将链表拆分成由奇数和偶数结点组成的两条链表
// 8、释放链表
// 0、退出
//         通过具体选择实现相应功能。


void ListOperate::orderedInsert(int value)
{
    auto it = this->list.before_begin();

    while(it.next() != this->list.end()){
        if(*it.next() >= value){
            this->list.insert_after(it, value);
            return;
        }

        it++;
    }

    this->list.insert_after(it, value);
}


void ListOperate::menu()
{
    using ListOperFunc = void (ListOperate:: *)();
    static ListOperFunc funcs[] = {
        &this->create,
        reinterpret_cast<ListOperFunc>(&this->show),
        &this->sort,
        &this->add,
        &this->del,
        &this->reverse,
        reinterpret_cast<ListOperFunc>(&this->split),
        &this->clear
    };

    int key = 0;

    while(true){
        std::cout << std::endl
            << " --- 链表操作菜单 --- \n"
            << "1. 创建链表\n"
            << "2. 显示链表元素\n"
            << "3. 按升序排序\n"
            << "4. 添加链表元素\n"
            << "5. 删除链表元素\n"
            << "6. 反转链表\n"
            << "7. 拆分奇数与偶数节点\n"
            << "8. 清空链表\n"
            << "0. 退出\n"
            << "请选择："
        ;

        do{
            std::cin >> key;
        }while(key < 0 || key > 8);

        if(key == 0){
            break;
        }

        (this->*funcs[key-1])();
    };
}


void ListOperate::create()
{
    std::cout << std::endl
        << " --- 创建链表 --- \n"
        << "输入一个正整数序列作为节点元素，以-1结尾\n"
        << "请输入节点元素："
    ;

    this->list.clear();

    while(true){
        int value;
        std::cin >> value;

        if(!std::cin || value <= 0){
            break;
        }

        this->list.push_front(value);
    }

    this->pause("创建完成");
}


void ListOperate::show() const
{
    std::cout
        << " --- 显示链表元素 --- \n"
        << "链表元素："
    ;
    
    this->showList();

    this->pause("已显示所有元素");
}


void ListOperate::sort()
{
    std::cout
        << " --- 按升序排序 --- \n"
        << "链表元素："
    ;

    this->list.sort();
    this->showList();

    this->pause("排序完成");
}


void ListOperate::add()
{
    std::cout << std::endl
        << " --- 添加链表元素 --- \n"
        << "输入一个正整数作为节点元素\n"
        << "请输入："
    ;

    int value;
    std::cin >> value;

    if(std::cin && value > 0){
        this->orderedInsert(value);
    }

    this->pause("添加完成");
}


void ListOperate::del()
{
    std::cout << std::endl
        << " --- 删除链表元素 --- \n"
        << "输入要删除的元素的值\n"
        << "请输入："
    ;

    int value;
    std::cin >> value;

    if(std::cin && value > 0){
        this->list.remove(value);
    }

    this->pause("删除完成");
}


void ListOperate::reverse()
{
    std::cout
        << " --- 反转链表 --- \n"
        << "链表元素："
    ;

    this->list.reverse();
    this->showList();

    this->pause("反转完成");
}


void ListOperate::split() const
{
    std::cout
        << " --- 拆分奇数与偶数节点 --- \n"
        << "（以下拆分结果将不会保存）\n"
    ;

    auto lists = this->list.split([](int x) -> bool { return x % 2; });

    std::cout << "奇数节点元素：";
    this->showList(lists[1]);
    std::cout << "偶数节点元素：";
    this->showList(lists[0]);

    this->pause("拆分完成");
}


void ListOperate::clear()
{
    std::cout
        << " --- 清空链表 --- \n"
    ;

    this->list.clear();

    this->pause("链表已清空");
}
