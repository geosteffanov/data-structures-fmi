#include <iostream>
#include <iomanip>
#include <cstring>
#include <stack>
#include <assert.h>
using namespace std;
template <class T>
struct Node {
    T data;
    Node<T>* left;
    Node<T>* right;

    Node() {
        this->data = T();
        right = NULL;
        left = NULL;
    }

    Node(const T data) {
        this->data = data;
        right = NULL;
        left = NULL;
    }

    Node(T d, Node<T>* l, Node<T>* r) :
         data(d), left(l), right(r) {}
};

template <class T>
class BTree  {
public:
    class TreeIterator
	{

	public:
	    TreeIterator() : current(nullptr) {}

	    TreeIterator(const TreeIterator& other) : current(other.current) {}

	    TreeIterator& operator= (const TreeIterator& other) {
            if (this != &other) {
                current = other.current;
            }
            return *this;
	    }

		TreeIterator (Node<T> * const &root) : current(root) {}

		T get() const {
            assert(current != nullptr);
            return current->data;
		}

		T& operator * () {
            if (current == nullptr)
                current = new Node<T> (T(),nullptr,nullptr);
            return current->data;
		}

		TreeIterator goLeft () {
            assert(current!= nullptr);
            return TreeIterator(current->left);
		}
		TreeIterator goRight () {
            assert(current != nullptr);
            return TreeIterator(current->right);
		}
		bool empty() const {
            return current == nullptr;
		}

	private:
		Node<T> * const &current;
	};
private:
    Node<T>* root;

    void add (const T& x, const char *trace, Node<T>* &subTreeRoot) {
        if (subTreeRoot == nullptr)
        {
            assert (strlen(trace) == 0);
            subTreeRoot = new Node<T>(x,nullptr,nullptr);
            return;
        }

        assert (strlen(trace)>0);

        if (trace[0]=='L') {
            add (x,trace+1,subTreeRoot->left);
            return;
        }

        assert (trace[0]=='R');
        add (x,trace+1,subTreeRoot->right);
    }

    Node<T>* copyTree(Node<T>* other) {
        if (other == nullptr)
            return nullptr;
        return new Node<T>(other->data, copyTree(other->left), copyTree(other->right));
    }

    void deleteTree(Node<T>*& subTreeRoot) {
        if (subTreeRoot == NULL)
            return;

        deleteTree(subTreeRoot->left);
        deleteTree(subTreeRoot->right);
        Node<T>* temp = subTreeRoot;
        delete temp;

        subTreeRoot = NULL;
    }

public:
    BTree() {
        root = NULL;
    }

    BTree(const BTree<T>& other) {
        root = copyTree(other.root);
    }

    BTree<T>& operator=(const BTree<T>& other) {
        if (this != &other) {
            deleteTree(root);
            root = copyTree(other.root);
        }
    }

    ~BTree() {
        deleteTree(root);
    }

    TreeIterator iterator() const {
        return TreeIterator(root);
    }

    BTree<T>& add(const T& x, const char *trace) {
            add (x,trace,root);
            return *this;
    }
};

template <class T>
using iter = typename BTree<T>::TreeIterator;

int sumNodes(const BTree<int>& tree) {
    struct QueueFrame {
        const iter<int> parent;
        const iter<int> current;

        QueueFrame(const iter<int>& _parent, const iter<int>& _current) : parent(_parent), current(_current) {
        }
    };
    stack<QueueFrame> helper;

    int result = 0;

    iter<int> root = tree.iterator();
    if (root.empty())
        return result;

    iter<int> firstLeft = root.goLeft();
    iter<int> firstRight = root.goRight();

    if (!firstLeft.empty()) {
        helper.push(QueueFrame(root, firstLeft));
    }
    if (!firstRight.empty()) {
        helper.push(QueueFrame(root, firstRight));
    }

    while (!helper.empty()) {
            QueueFrame top = helper.top();
            helper.pop();

            iter<int> parent = top.parent;
            iter<int> current = top.current;
            iter<int> currentRight = current.goRight();
            iter<int> currentLeft = current.goLeft();

            if (!(currentRight.empty() && currentLeft.empty())) {
                    int parentNode = parent.get();
                    int currentNode = current.get();

                    bool toAdd = false;
                    if (currentRight.empty()) {
                        helper.push(QueueFrame(current, currentLeft));
                        if (currentNode < parentNode && currentNode > currentLeft.get())
                            toAdd = true;
                    }
                    else if (currentLeft.empty()) {
                        helper.push(QueueFrame(current, currentRight));
                        if (currentNode < parentNode && currentNode > currentRight.get())
                        toAdd = true;
                    }
                    else {
                        helper.push(QueueFrame(current, currentLeft));
                        helper.push(QueueFrame(current, currentRight));
                        if (currentNode < parentNode && currentNode > currentRight.get() && currentNode > currentLeft.get())
                            toAdd = true;
                    }
                    if (toAdd)
                        result += currentNode;

            }
    }
    return result;
}
