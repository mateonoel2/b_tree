#include <vector>
#include <iostream>
using namespace std;

int ORDER;

template <typename T>
class BPlusTree{

private:
    enum state_t {OVER, UNDER, B_OK};

    struct node {
        std::vector<T> data;
        std::vector<node*> children;
        size_t count{0};

        node(): data(ORDER+1), children(ORDER+2, nullptr) {}

        void insert_into(size_t index, const T& value) {
            size_t j = this->count;
            while (j > index) {
                children[j+1] = children[j];
                data[j] = data[j-1];
                j--;
            }
            children[j+1] = children[j];
            data[j] = value;
            this->count++;
        }

        void push_back(const T& value) {
            insert_into(this->count, value);
        }

        state_t insert(const T& value) {
            // binary_search
            size_t index = 0;
            while (this->data[index] < value  && index < this->count) {
                index += 1;
            }
            if (this->children[index] == nullptr) {
                // this is a leaf node
                this->insert_into(index, value);
            } else {
                auto state = this->children[index]->insert(value);
                if (state == state_t::OVER) {
                    // split
                    this->split(index);
                }
            }
            return this->count > ORDER ? OVER : B_OK;
        }

        void split(size_t position) {
            // leaf nodes / index nodes
            node* parent = this;
            node* ptr = this->children[position];

            // TODO: reuse ptr buffer
            node* child1 = new node();
            node* child2 = new node();
            size_t i = 0;
            for (; i < ptr->count / 2; i++) {
                child1->children[i] = ptr->children[i];
                child1->data[i] = ptr->data[i];
                child1->count++;
            }
            child1->children[i] = ptr->children[i];
            auto mid = i;
            if (ptr->children[0] != nullptr) {
                i += 1;
            }
            //i += 1;
            size_t j = 0;
            for (; i < ptr->count; i++) {
                child2->children[j] = ptr->children[i];
                child2->data[j] = ptr->data[i];
                child2->count++;
                j++;
            }
            child2->children[j] = ptr->children[i];

            parent->insert_into(position, ptr->data[mid]);
            parent->children[position] = child1;
            parent->children[position + 1] = child2;
        }

    };

public:

    BPlusTree() = default;

    void insert(const T& value) {

        auto state = root.insert(value);
        if (state == state_t::OVER) {
            // split root node
            split_root();
        }

    }

    void print() {

        print(&root, 0);
    }

    void print(node *ptr, int level) {

        if (ptr) {
            int i;
            for (i = ptr->count - 1; i >= 0; i--) {
                print(ptr->children[i + 1], level + 1);

                for (int k = 0; k < level; k++) {
                    std::cout << "    ";
                }
                std::cout << ptr->data[i] << "\n";
            }
            print(ptr->children[i + 1], level + 1);
        }

    }

private:

    void split_root() {

        node* ptr = &root;
        node* child1 = new node();
        node* child2 = new node();
        size_t i = 0;
        for (; i < ptr->count / 2; i++) {
            child1->children[i] = ptr->children[i];
            child1->data[i] = ptr->data[i];
            child1->count++;
        }
        child1->children[i] = ptr->children[i];
        auto mid = i;
        if (ptr->children[0] != nullptr) {
            i  += 1;
        }
        //i += 1;
        size_t j = 0;
        for (; i < ptr->count; i++) {
            child2->children[j] = ptr->children[i];
            child2->data[j] = ptr->data[i];
            child2->count++;
            j++;
        }
        child2->children[j] = ptr->children[i];

        ptr->data[0] = ptr->data[mid];
        ptr->children[0] = child1;
        ptr->children[1] = child2;
        ptr->count = 1;

    }

private:
    node root;
};

int main() {

    //BPlusTree<int, 4> bt;

    /*for (size_t i = 0; i <= 7; i++) {
        bt.insert(i);
    }*/

    /*bt.insert(1);
    bt.insert(1);
    bt.insert(2);
    bt.insert(3);
    bt.insert(5);*/

    vector <int> elements;
    int n, order, temp;
    cin >> n >> order;
    for (int i = 0; i < n; i++) {
        cin >> temp;
        elements.push_back(temp);
    }

    ORDER = order;
    BPlusTree<int> bt;

    for (size_t i = 0; i < n; i++) {
        bt.insert(elements[i]);
    }

    bt.print();

    return 0;

}

