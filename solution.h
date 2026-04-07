#ifndef SPEEDCIRCULARLIST_H
#define SPEEDCIRCULARLIST_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

constexpr int s_prime = 31;

inline int log2(int x) {
    int ans = 0;
    while (x > 1) {
        x /= 2;
        ++ans;
    }
    return ans;
}

template<typename T, int b_prime>
class SpeedCircularLinkedList {
public:
    struct Node {
        std::map<std::string, T> kv_map;
        int bound = 0;
        Node* next = nullptr;
        Node** fast_search_list = nullptr;

        Node(int b, int fast_sz) : bound(b), next(nullptr), fast_search_list(nullptr) {
            if (fast_sz > 0) {
                fast_search_list = new Node*[fast_sz];
                for (int i = 0; i < fast_sz; ++i) fast_search_list[i] = nullptr;
            }
        }

        ~Node() {
            delete[] fast_search_list;
            fast_search_list = nullptr;
        }
    };

private:
    Node* head = nullptr;
    int fast_search_list_size = 0;
    int list_size = 0;

    static int GetHashCode(std::string str) {
        long long ans = 0;
        for (auto& ch : str) {
            ans = (ans * s_prime + ch) % b_prime;
        }
        return static_cast<int>((ans + b_prime) % b_prime);
    }

    void BuildFastSearchList() {
        if (!head || list_size <= 0) return;
        fast_search_list_size = log2(std::max(1, list_size));
        std::vector<Node*> nodes;
        nodes.reserve(list_size);
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            nodes.push_back(cur);
            cur = cur->next;
        }
        for (auto* n : nodes) {
            // reallocate fast_search_list to match size if needed
            if (n->fast_search_list) {
                delete[] n->fast_search_list;
                n->fast_search_list = nullptr;
            }
            if (fast_search_list_size > 0) {
                n->fast_search_list = new Node*[fast_search_list_size];
                for (int i = 0; i < fast_search_list_size; ++i) n->fast_search_list[i] = nullptr;
            }
        }
        // level 0 pointers
        for (int i = 0; i < list_size; ++i) {
            if (fast_search_list_size > 0) nodes[i]->fast_search_list[0] = nodes[i]->next;
        }
        // doubling
        for (int k = 1; k < fast_search_list_size; ++k) {
            for (int i = 0; i < list_size; ++i) {
                Node* prev = nodes[i]->fast_search_list[k - 1];
                nodes[i]->fast_search_list[k] = prev ? prev->fast_search_list[k - 1] : nullptr;
            }
        }
    }

public:
    explicit SpeedCircularLinkedList(std::vector<int> node_bounds) {
        list_size = static_cast<int>(node_bounds.size());
        if (list_size == 0) {
            head = nullptr;
            fast_search_list_size = 0;
            return;
        }
        // create nodes and link them
        Node* prev = nullptr;
        for (int i = 0; i < list_size; ++i) {
            Node* node = new Node(node_bounds[i], 0);
            if (!head) head = node;
            if (prev) prev->next = node;
            prev = node;
        }
        // close the ring
        if (prev) prev->next = head;

        BuildFastSearchList();
    }

    ~SpeedCircularLinkedList() {
        if (!head || list_size <= 0) return;
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = nullptr;
        list_size = 0;
        fast_search_list_size = 0;
    }

    void put(std::string str, T value) {
        int code = GetHashCode(str);
        if (!head) return;
        if (code <= head->bound) {
            head->kv_map[str] = value;
            return;
        }
        Node* cur = head->next;
        for (int i = 1; i < list_size; ++i) {
            if (code <= cur->bound) {
                cur->kv_map[str] = value;
                return;
            }
            cur = cur->next;
        }
        // Should not reach here according to problem guarantees
        head->kv_map[str] = value;
    }

    T get(std::string str) {
        int code = GetHashCode(str);
        if (!head) return T();
        Node* target = nullptr;
        if (code <= head->bound) {
            target = head;
        } else {
            Node* cur = head->next;
            for (int i = 1; i < list_size; ++i) {
                if (code <= cur->bound) { target = cur; break; }
                cur = cur->next;
            }
            if (!target) target = head; // fallback
        }
        auto it = target->kv_map.find(str);
        if (it != target->kv_map.end()) return it->second;
        return T();
    }

    void print() {
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            std::cout << "[Node] Bound = " << cur->bound << ", kv_map_size = " << cur->kv_map.size() << '\n';
            cur = cur->next;
        }
    }

    int size() const { return list_size; }
};
#endif //SPEEDCIRCULARLIST_H

