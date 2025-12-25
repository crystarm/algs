#include <iostream>

using namespace std;

template <typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;

    Pair() {}
    Pair(T1 f, T2 s) : first(f), second(s) {}
};

template <typename T>
class MyVector {
private:
    T* data;
    size_t _size;
    size_t _capacity;

    void reallocate(size_t new_cap) {
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < _size; ++i) {
            new_data[i] = data[i];
        }
        if (data) delete[] data;
        data = new_data;
        _capacity = new_cap;
    }

public:
    MyVector() : data(nullptr), _size(0), _capacity(0) {}

    MyVector(size_t n, const T& val = T()) : data(nullptr), _size(0), _capacity(0) {
        resize(n, val);
    }

    ~MyVector() {
        if (data) delete[] data;
    }

    MyVector(const MyVector& other) : data(nullptr), _size(0), _capacity(0) {
        if (other._size > 0) {
            reallocate(other._capacity);
            _size = other._size;
            for (size_t i = 0; i < _size; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    MyVector& operator=(const MyVector& other) {
        if (this == &other) return *this;
        if (data) delete[] data;
        data = nullptr;
        _size = 0;
        _capacity = 0;

        if (other._size > 0) {
            reallocate(other._capacity);
            _size = other._size;
            for (size_t i = 0; i < _size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    void push_back(const T& val) {
        if (_size == _capacity) {
            reallocate(_capacity == 0 ? 1 : _capacity * 2);
        }
        data[_size++] = val;
    }

    void resize(size_t n, const T& val = T()) {
        if (n > _capacity) {
            reallocate(n);
        }
        for (size_t i = _size; i < n; ++i) {
            data[i] = val;
        }
        _size = n;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    size_t size() const { return _size; }
    bool empty() const { return _size == 0; }

    T* begin() { return data; }
    T* end() { return data + _size; }
    const T* begin() const { return data; }
    const T* end() const { return data + _size; }
};

template <typename T>
class MyQueue {
    struct Node {
        T val;
        Node* next;
        Node(T v) : val(v), next(nullptr) {}
    };

    Node* head;
    Node* tail;

public:
    MyQueue() : head(nullptr), tail(nullptr) {}

    ~MyQueue() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push(T val) {
        Node* newNode = new Node(val);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void pop() {
        if (!head) return;
        Node* temp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
    }

    T front() {
        return head->val;
    }

    bool empty() {
        return head == nullptr;
    }
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    MyVector<MyVector<Pair<int, int>>> building_to_comps(N + 1);

    MyVector<MyVector<int>> comp_to_buildings;

    for (int i = 0; i < K; ++i) {
        int r;
        cin >> r;

        MyVector<MyVector<int>> adj(N + 1);
        MyVector<int> current_map_buildings;

        for (int j = 0; j < r; ++j) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
            current_map_buildings.push_back(u);
            current_map_buildings.push_back(v);
        }

        MyVector<bool> visited_in_map(N + 1, false);

        for (int b : current_map_buildings) {
            if (!visited_in_map[b]) {
                MyVector<int> component;
                MyQueue<int> q;

                q.push(b);
                visited_in_map[b] = true;

                while (!q.empty()) {
                    int u = q.front();
                    q.pop();
                    component.push_back(u);

                    for (int v : adj[u]) {
                        if (!visited_in_map[v]) {
                            visited_in_map[v] = true;
                            q.push(v);
                        }
                    }
                }

                int comp_idx = (int)comp_to_buildings.size();
                comp_to_buildings.push_back(component);

                for (int u : component) {
                    building_to_comps[u].push_back(Pair<int, int>(i, comp_idx));
                }
            }
        }
    }

    MyVector<int> dist(N + 1, -1);
    MyVector<bool> comp_used(comp_to_buildings.size(), false);
    MyQueue<int> q;

    dist[1] = 0;
    q.push(1);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == N) {
            cout << dist[N] << endl;
            return 0;
        }

        for (auto& p : building_to_comps[u]) {
            int comp_idx = p.second;

            if (!comp_used[comp_idx]) {
                comp_used[comp_idx] = true;

                for (int v : comp_to_buildings[comp_idx]) {
                    if (dist[v] == -1) {
                        dist[v] = dist[u] + 1;
                        q.push(v);
                    }
                }
            }
        }
    }

    cout << dist[N] << endl;

    return 0;
}
