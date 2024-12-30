#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>

using namespace std;

struct Flight {
    string from, to, airliner;
    int business_seat, economy_seat, hours, minutes;
};

struct AVLFlight {
    Flight data;
    AVLFlight* pLeft;
    AVLFlight* pRight;
};


AVLFlight* createNode(Flight data) {
    AVLFlight* newNode = new AVLFlight {data, nullptr, nullptr};
    return newNode;
}

int getHeight(AVLFlight* root) {
    if (root == nullptr) {
        return 0;
    }

    return max(getHeight(root->pLeft), getHeight(root->pRight)) + 1;
}

int getBalance(AVLFlight* root) {
    if (root == nullptr) {
        return 0;
    }

    return getHeight(root->pLeft) - getHeight(root->pRight);
}

void leftRotate(AVLFlight* &root) {
    AVLFlight* temp = root->pRight;
    root->pRight = temp->pLeft;
    temp->pLeft = root;
    root = temp;
}

void rightRotate(AVLFlight* &root) {
    AVLFlight* temp = root->pLeft;
    root->pLeft = temp->pRight;
    temp->pRight = root;
    root = temp;
}

void Rebalance(AVLFlight* &root) {
    if (root == nullptr) {
        return;
    }

    int balance = getBalance(root);

    if (balance >= -1 && balance <= 1) {
        return;
    }

    if (balance > 1) {
        if (getBalance(root->pLeft) < 0) {
            leftRotate(root->pLeft);
        }
        rightRotate(root);
    }

    else if (balance < -1) {
        if (getBalance(root->pRight) > 0) {
            rightRotate(root->pRight);
        }
        leftRotate(root);
    }
}

int compare(Flight data1, Flight data2) {
    if (data1.economy_seat * data2.business_seat < data1.business_seat * data2.economy_seat) {
        return -1;
    }
    else if (data1.economy_seat * data2.business_seat > data1.business_seat * data2.economy_seat) {
        return 1;
    }

    else {
        if (data1.hours * 60 + data1.minutes < data2.hours * 60 + data2.minutes) {
            return -1;
        }

        else if (data1.hours * 60 + data1.minutes > data2.hours * 60 + data2.minutes) {
            return 1;
        }

        else {
            return 0;
        }
    }
}

void insert(AVLFlight* &root, Flight data) {
    if (root == nullptr) {
        root = createNode(data);
        return;
    } 

    if (compare(data, root->data) == -1) {
        insert(root->pLeft, data);
    }
    else if (compare(data, root->data) == 1) {
        insert(root->pRight, data);
    }
    else{
        return;
    }

    Rebalance(root);
}

Flight readLine(string line) {
    Flight data;

    if (line.front() == '{') line.erase(0, 1);
    if (line.back() == '}') line.pop_back();

    stringstream ss(line);
    string temp;

    getline(ss, temp, ':');
    temp = temp.substr(1, temp.size() - 2);
    int pos = temp.find(',');
    data.from = temp.substr(0, pos);
    data.to = temp.substr(pos + 1);

    getline(ss, temp, '\"');
    getline(ss, data.airliner, '\"');

    getline(ss, temp, '\"');
    ss >> data.business_seat;

    getline(ss, temp, ',');
    ss >> data.economy_seat;

    getline(ss, temp, '\"');
    getline(ss, temp, '\"');
    ss >> data.hours;
    getline(ss, temp, ',');
    ss >> data.minutes;

    return data;
}

void createAVL(AVLFlight* &root, string data) {
    ifstream f(data);
    if (!f) {
        return;
    }

    string line;
    getline(f, line);

    while (getline(f, line)) {
        insert(root, readLine(line));
    }

    f.close();
}

void levelAVL(AVLFlight* root) {
    if (root == nullptr) {
        return;
    }

    queue<AVLFlight*> q;
    q.push(root);
    int level = 1;

    while (!q.empty()) {
        int size = q.size();
        cout << "Level " << level++ << " :\n";
        for (int i = 0; i < size; i++) {
            AVLFlight* temp = q.front();
            q.pop();
            cout << temp->data.from << "," << temp->data.to << ","
                << temp->data.airliner << "," << temp->data.business_seat << ","
                << temp->data.economy_seat << "," << temp->data.hours << ","
                << temp->data.minutes << "\n";
 
            if (temp->pLeft != nullptr) {
                q.push(temp->pLeft);
            }

            if (temp->pRight != nullptr) {
                q.push(temp->pRight);
            }
        }
        cout << '\n';
    }
}

void RemoveTree(AVLFlight* &pRoot) {
    if (pRoot == nullptr) {
        return;
    }

    RemoveTree(pRoot->pLeft);
    RemoveTree(pRoot->pRight);
    delete pRoot;
    pRoot = nullptr;
}

void removeAVL(AVLFlight* &root, float z) {
    if (root == nullptr) {
        return;
    }

    // Nếu tỉ lệ nhỏ hơn hoặc bằng z, xóa nút này
    if (root->data.economy_seat <= z * root->data.business_seat) {
        if (root->pLeft == nullptr && root->pRight == nullptr) {
            delete root;
            root = nullptr;
        }
        else if (root->pLeft == nullptr || root->pRight == nullptr) {
            AVLFlight* temp = root->pLeft ? root->pLeft : root->pRight;
            delete root;
            root = temp;
        }
        else {
            AVLFlight* minNode = root->pRight, * minParent = nullptr;

            while (minNode->pLeft != nullptr) {
                minParent = minNode;
                minNode = minNode->pLeft;
            }

            root->data = minNode->data;

            if (minParent != nullptr) {
                minParent->pLeft = minNode->pRight;
            }
            else {
                root->pRight = minNode->pRight;
            }

            delete minNode;

            // AVLFlight* minNode = root->pRight;
            // while (minNode->pLeft != nullptr) {
            //     minNode = minNode->pLeft;
            // }

            // root->data = minNode->data;
            // removeAVL(root->pRight, z);
        }
    } 
    else {
        removeAVL(root->pLeft, z);
        removeAVL(root->pRight, z);
    }

    Rebalance(root);
}

struct FGraph {
    vector<string> countries;
    vector<vector<int>> adjList;
};

FGraph createGraph(string data) {
    FGraph G;

    ifstream file(data);

    if (!file.is_open()) {
        cerr << "Cannot open file: " << data << endl;
        return G;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string country1, country2;
        getline(ss, country1, ',');
        getline(ss, country2, ',');

        // Tìm chỉ số của quốc gia trong mảng countries
        auto it1 = find(G.countries.begin(), G.countries.end(), country1);
        auto it2 = find(G.countries.begin(), G.countries.end(), country2);

        int idx1, idx2;
        if (it1 == G.countries.end()) {
            idx1 = G.countries.size();
            G.countries.push_back(country1);
            G.adjList.push_back(vector<int>()); // Thêm danh sách kề cho quốc gia mới
        } else {
            idx1 = distance(G.countries.begin(), it1);
        }

        if (it2 == G.countries.end()) {
            idx2 = G.countries.size();
            G.countries.push_back(country2);
            G.adjList.push_back(vector<int>()); // Thêm danh sách kề cho quốc gia mới
        } else {
            idx2 = distance(G.countries.begin(), it2);
        }

        // Thêm cạnh vào danh sách kề
        G.adjList[idx1].push_back(idx2);
        G.adjList[idx2].push_back(idx1); // Vì đồ thị vô hướng
    }

    file.close();
    return G;
}

FGraph createGraphFromBoeingOrAirbus(string data) {
    FGraph G;

    ifstream file(data);

    if (!file.is_open()) {
        cerr << "Cannot open file: " << data << endl;
        return G;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string airliner, country1, country2;
        getline(ss, airliner, ',');
        getline(ss, country1, ',');
        getline(ss, country2, ',');

        // Kiểm tra hãng máy bay
        if (airliner.find("Boeing") == 0 || airliner.find("Airbus") == 0) {
            auto it1 = find(G.countries.begin(), G.countries.end(), country1);
            auto it2 = find(G.countries.begin(), G.countries.end(), country2);

            int idx1, idx2;
            if (it1 == G.countries.end()) {
                idx1 = G.countries.size();
                G.countries.push_back(country1);
                G.adjList.push_back(vector<int>());
            } else {
                idx1 = distance(G.countries.begin(), it1);
            }

            if (it2 == G.countries.end()) {
                idx2 = G.countries.size();
                G.countries.push_back(country2);
                G.adjList.push_back(vector<int>());
            } else {
                idx2 = distance(G.countries.begin(), it2);
            }

            G.adjList[idx1].push_back(idx2);
            G.adjList[idx2].push_back(idx1);
        }
    }

    file.close();
    return G;
}

void printGraph(FGraph G, string country) {
    auto it = find(G.countries.begin(), G.countries.end(), country);
    if (it == G.countries.end()) {
        cout << "No direct flights to " << country << endl;
        return;
    }

    int idx = distance(G.countries.begin(), it);
    int count = min(10, (int)G.adjList[idx].size());
    for (int i = 0; i < count; i++) {
        cout << G.countries[G.adjList[idx][i]];
        if (i < count - 1) cout << ", ";
    }
    cout << endl;
}

bool isBigraph(FGraph G, vector<int> &subset) {
    vector<int> color(G.countries.size(), -1);

    for (int u : subset) {
        if (color[u] == -1) {
            queue<int> q;
            q.push(u);
            color[u] = 0;

            while (!q.empty()) {
                int v = q.front(); q.pop();
                for (int neighbor : G.adjList[v]) {
                    if (find(subset.begin(), subset.end(), neighbor) == subset.end())
                        continue;

                    if (color[neighbor] == -1) {
                        color[neighbor] = 1 - color[v];
                        q.push(neighbor);
                    } else if (color[neighbor] == color[v]) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool nextPermutation(vector<int>& subset, int start) {
    int n = subset.size();
    int i = n - 2;
    while (i >= start && subset[i] >= subset[i + 1]) i--;
    if (i < start) return false;
    int j = n - 1;
    while (subset[j] <= subset[i]) j--;
    swap(subset[i], subset[j]);
    reverse(subset.begin() + i + 1, subset.end());
    return true;
}

void Bigraph(FGraph G, string A, string B) {
    auto itA = find(G.countries.begin(), G.countries.end(), A);
    auto itB = find(G.countries.begin(), G.countries.end(), B);

    if (itA == G.countries.end() || itB == G.countries.end()) {
        cout << "Countries not found in graph." << endl;
        return;
    }

    int idxA = distance(G.countries.begin(), itA);
    int idxB = distance(G.countries.begin(), itB);

    vector<int> subset;
    subset.push_back(idxA);
    subset.push_back(idxB);

    for (int i = 0; i < G.countries.size(); i++) {
        if (i != idxA && i != idxB) subset.push_back(i);
    }

    do {
        vector<int> currentSubset = {subset[0], subset[1], subset[2], subset[3], subset[4]};
        if (isBigraph(G, currentSubset)) {
            for (int i = 0; i < 5; i++) {
                cout << G.countries[currentSubset[i]];
                if (i < 4) cout << ", ";
            }
            cout << endl;
        }
    } while (nextPermutation(subset, 2));
}

// #include <iostream>
// #include <queue>

// struct NODE {
//     int key;
//     NODE *pLeft;
//     NODE *pRight;
//     int height;
// };

// // ====================== Same Functions BST ======================
// // 1. Create a new NODE with a specific value:
// NODE* CreateNode(int data);
// // 2. Insert a specified value to a given AVL Tree (notify if the value exists):
// void Insert(NODE* &pRoot, int x);
// // 3. Pre-order Traversal:
// void NLR(NODE* pRoot);
// // 4. In-order Traversal:
// void LNR(NODE* pRoot);
// // 5. Post-order Traversal:
// void LRN(NODE* pRoot);
// // 6. Level-order Traversal:
// void LevelOrder(NODE* pRoot);
// // 7. Find the height of a given Binary Tree:
// int Height(NODE* pRoot);
// // 8. Count the number of NODE from a given Binary Tree:
// int CountNode(NODE* pRoot);
// // 9. Find the sum of all NODE values in a Binary Tree:
// int SumNode(NODE* pRoot);
// // 10. Search and return a NODE with specific value in a AVL Tree:
// NODE* Search(NODE* pRoot, int x);
// // 11. Remove a NODE with a specific value from a AVL Tree:
// void Remove(NODE* &pRoot, int x);
// // 12. Create a AVL Tree from an array of values:
// NODE* CreateTree(int a[], int n);
// // 13. Remove a AVL Tree (free the memory occupied by this tree):
// void RemoveTree(NODE* &pRoot);
// // 14. Find the height of a NODE with a specific value (return -1 if not found):
// int HeightNode(NODE* pRoot, int value);
// // 15. Find the level of a specific NODE:
// int Level(NODE* pRoot, NODE* p);
// // 16. Count the number of leaves in a Binary Tree:
// int CountLeaf(NODE* pRoot);
// // 17. Count the number of NODE in a AVL Tree with keys less than a specific value:
// int CountLess(NODE* pRoot, int x);
// // 18. Count the number of NODE in a AVL Tree with keys greater than a specific value:
// int CountGreater(NODE* pRoot, int x);
// // 19. Determine whether a given Binary Tree qualifies as a AVL Tree:
// bool IsBST(NODE* pRoot);
// // 20. Determine whether a given Binary Tree qualifies as a Full AVL Tree:
// bool IsFullBST(NODE* pRoot);


// // ====================== AVL ======================
// void Rebalance(NODE *&pRoot, int data);
// void LeftRotate(NODE *&pRoot);
// void RightRotate(NODE *&pRoot);
// int GetBalance(NODE *pRoot);
// void UpdateHeight(NODE *pRoot);
// // 4. Determine whether a given Binary Tree qualifies as an AVL Tree:
// bool IsAVL(NODE* pRoot);

// // ====================== Functions ======================

// // 1. Create a new NODE with a specific value:
// NODE *CreateNode(int data) {
//     NODE *newNode = new (std::nothrow) NODE {data, nullptr, nullptr, 1};

//     if (newNode == nullptr) {
//         std::cout << "Memory allocation failed!\n";
//         return nullptr;
//     }

//     return newNode;
// }

// // 2. Insert a specified value to a given AVL Tree (notify if the value exists):
// void Insert(NODE *&pRoot, int newData) {
//     if (pRoot == nullptr) {
//         pRoot = CreateNode(newData);
//         return;
//     }

//     if (newData < pRoot->key) {
//         Insert(pRoot->pLeft, newData);
//     }
//     else if (newData > pRoot->key) {
//         Insert(pRoot->pRight, newData);
//     }
//     else {
//         std::cout << "The value exists!\n";
//         return;
//     }

//     Rebalance(pRoot, newData);
// }

// // 3. Pre-order Traversal:
// void NLR(NODE* pRoot) {
//     if (pRoot == nullptr) {
//         return;
//     }

//     std::cout << pRoot->key << ' ';
//     NLR(pRoot->pLeft);
//     NLR(pRoot->pRight);
// }

// // 4. In-order Traversal:
// void LNR(NODE* pRoot) {
//     if (pRoot == nullptr) {
//         return;
//     }

//     LNR(pRoot->pLeft);
//     std::cout << pRoot->key << ' ';
//     LNR(pRoot->pRight);
// }

// // 5. Post-order Traversal:
// void LRN(NODE* pRoot) {
//     if (pRoot == nullptr) {
//         return;
//     }

//     LRN(pRoot->pRight);
//     LRN(pRoot->pRight);
//     std::cout << pRoot->key << ' ';
// }

// // 6. Level-order Traversal:
// void LevelOrder(NODE* pRoot) {
//     if (pRoot == nullptr) {
//         return;
//     }

//     std::queue<NODE*> q;
//     q.push(pRoot);

//     while (!q.empty()) {
//         NODE* temp = q.front();
//         q.pop();
//         std::cout << temp->key << ' ';

//         if (temp->pLeft != nullptr) {
//             q.push(temp->pLeft);
//         }

//         if (temp->pRight != nullptr) {
//             q.push(temp->pRight);
//         }
//     }
// }

// // 7. Find the height of a given Binary Tree:
// int Height(NODE *pRoot) {
//     if (pRoot == nullptr) {
//         return 0;
//     }
//     return pRoot->height;
// }

// // 8. Count the number of NODE from a given Binary Tree:
// int CountNode(NODE* pRoot) {
//     if (pRoot == nullptr) {
//         return 0;
//     }

//     return CountNode(pRoot->pLeft) + CountNode(pRoot->pRight) + 1;
// }

// // 9. Find the sum of all NODE values in a Binary Tree:
// int SumNode(NODE* pRoot) {
//     if (pRoot == nullptr) {
//         return 0;
//     }

//     return pRoot->key + SumNode(pRoot->pLeft) + SumNode(pRoot->pRight);
// }

// // 10. Search and return a NODE with specific value in a Binary Search Tree:
// NODE* Search(NODE* pRoot, int x) {
//     if (pRoot == nullptr) {
//         return nullptr;
//     }

//     if (pRoot->key > x) {
//         return Search(pRoot->pLeft, x);
//     }
//     else if (pRoot->key < x) {
//         return Search(pRoot->pRight, x);
//     }
    
//     return pRoot;
// }

// // 11. Remove a NODE with a specific value from a AVL Tree:
// void Remove(NODE* &pRoot, int x) {
//     if (pRoot == nullptr) {
//         std::cout << "The value does not exist!\n";
//         return;
//     }

//     if (pRoot->key > x) {
//         Remove(pRoot->pLeft, x);
//     }
//     else if (pRoot->key < x) {
//         Remove(pRoot->pRight, x);
//     }
//     else {
//         if (pRoot->pLeft == nullptr) {
//             NODE *temp = pRoot->pRight;
//             delete pRoot;
//             pRoot = temp;
//         }
//         else if (pRoot->pRight == nullptr) {
//             NODE *temp = pRoot->pLeft;
//             delete pRoot;
//             pRoot = temp;
//         }
//         else {
//             NODE *temp = pRoot->pRight;

//             // Node trái nhất của cây con phải
//             while (temp->pLeft != nullptr) {
//                 temp = temp->pLeft;
//             }
//             pRoot->key = temp->key;
//             Remove(pRoot->pRight, temp->key);
//         }
//     }

//     Rebalance(pRoot, x);
// }

// // 12. Create a AVL Tree from an array of values:
// NODE* CreateTree(int a[], int n) {
//     NODE* pRoot = nullptr;
    
//     for (int i = 0; i < n; i++) {
//         Insert(pRoot, a[i]);
//     }

//     return pRoot;
// }

// // 13. Remove a AVL Tree (free the memory occupied by this tree):
// void RemoveTree(NODE* &pRoot) {
//     if (pRoot == nullptr) {
//         return;
//     }

//     RemoveTree(pRoot->pLeft);
//     RemoveTree(pRoot->pRight);
//     delete pRoot;
//     pRoot = nullptr;
// }

// // 14. Find the height of a NODE with a specific value (return -1 if not found):
// int HeightNode(NODE* pRoot, int value) {
//     if (pRoot == nullptr) {
//         return -1;
//     }

//     if (pRoot->key == value) {
//         return pRoot->height;
//     }

//     if (pRoot->key > value) {
//         return HeightNode(pRoot->pLeft, value);
//     }

//     return HeightNode(pRoot->pRight, value);
// }

// // 15. Find the level of a specific NODE:
// int Level(NODE* pRoot, NODE* p) {
//     if (pRoot == nullptr) {
//         return 0;
//     }

//     if (pRoot == p) {
//         return 1;
//     }

//     int left = Level(pRoot->pLeft, p);
//     int right = Level(pRoot->pRight, p);

//     if (left == 0 && right == 0) {
//         return 0;
//     }

//     return left + right;
// }

// // 16. Count the number of leaves in a Binary Tree:
// int CountLeaf(NODE* pRoot) {
//     if (pRoot == nullptr) {
//         return 0;
//     }

//     if (pRoot->pLeft == nullptr && pRoot->pRight == nullptr) {
//         return 1;
//     }

//     return CountLeaf(pRoot->pLeft) + CountLeaf(pRoot->pRight);
// }

// // 17. Count the number of NODE in a AVL Tree with keys less than a specific value:
// int CountLess(NODE* pRoot, int x) {
//     if (pRoot == nullptr) {
//         return 0;
//     }

//     if (pRoot->key < x) {
//         return 1 + CountLess(pRoot->pLeft, x) + CountLess(pRoot->pRight, x);
//     } else {
//         return CountLess(pRoot->pLeft, x);
//     }

// }

// // 18. Count the number of NODE in a AVL Tree with keys greater than a specific value:
// int CountGreater(NODE *pRoot, int x) {
//     if (pRoot == nullptr) {
//         return 0;
//     }
//     if (pRoot->key > x) {
//         return 1 + CountGreater(pRoot->pLeft, x) + CountGreater(pRoot->pRight, x);
//     } else {
//         return CountGreater(pRoot->pRight, x);
//     }
// }

// // 19. Determine whether a given Binary Tree qualifies as a Binary Search Tree:
// bool IsBST(NODE* pRoot) {
//     if (pRoot == nullptr) {
//         return true;
//     }

//     if (pRoot->pLeft != nullptr && pRoot->pLeft->key > pRoot->key) {
//         return false;
//     }

//     if (pRoot->pRight != nullptr && pRoot->pRight->key < pRoot->key) {
//         return false;
//     }

//     return IsBST(pRoot->pLeft) && IsBST(pRoot->pRight);
// }
// // 20. Determine whether a given Binary Tree qualifies as a Full Binary Search Tree:
// bool IsFullBST(NODE* pRoot) {
//     if (pRoot == nullptr) {
//         return true;
//     }

//     if (pRoot->pLeft == nullptr && pRoot->pRight == nullptr) {
//         return true;
//     }

//     if (pRoot->pLeft != nullptr && pRoot->pRight != nullptr) {
//         return IsFullBST(pRoot->pLeft) && IsFullBST(pRoot->pRight);
//     }

//     return false;
// }

// int GetBalance(NODE *pRoot) {
//     if (pRoot == nullptr) {
//         return 0;
//     }
//     return Height(pRoot->pLeft) - Height(pRoot->pRight);
// }

// void UpdateHeight(NODE *pRoot) {
//     if (pRoot == nullptr) {
//         return;
//     }
    
//     pRoot->height = std::max(Height(pRoot->pLeft), Height(pRoot->pRight)) + 1;
// }

// void LeftRotate(NODE *&pRoot) {
//     NODE *pTemp = pRoot->pRight;
//     pRoot->pRight = pTemp->pLeft;
//     pTemp->pLeft = pRoot;
//     UpdateHeight(pRoot);
//     UpdateHeight(pTemp);
//     pRoot = pTemp;
// }

// void RightRotate(NODE *&pRoot) {
//     NODE *pTemp = pRoot->pLeft;
//     pRoot->pLeft = pTemp->pRight;
//     pTemp->pRight = pRoot;
//     UpdateHeight(pRoot);
//     UpdateHeight(pTemp);
//     pRoot = pTemp;
// }

// void Rebalance(NODE *&pRoot, int data) {
//     if (pRoot == nullptr) {
//         return;
//     }

//     UpdateHeight(pRoot);
//     int balance = GetBalance(pRoot);

//     if (balance >= -1 && balance <= 1) {
//         return;
//     }

//     if (balance > 1) {
//         if (GetBalance(pRoot->pLeft) < 0) {
//             LeftRotate(pRoot->pLeft);
//         }
//         RightRotate(pRoot);
//     } 
//     else if (balance < -1) {
//         if (GetBalance(pRoot->pRight) > 0) {
//             RightRotate(pRoot->pRight);
//         }
//         LeftRotate(pRoot);
//     }
// }

// // 4. Determine whether a given Binary Tree qualifies as an AVL Tree:
// bool IsAVL(NODE* pRoot) {
//     if (pRoot == nullptr) {
//         return true;
//     }

//     int balance = GetBalance(pRoot);

//     if (balance < -1 || balance > 1) {
//         return false;
//     }

//     return IsAVL(pRoot->pLeft) && IsAVL(pRoot->pRight);
// }

// ================================GRaph

// std::vector<std::vector<int>> generateBaseUndirectedGraph(std::vector<std::vector<int>> matrix);

// // 1. The “graph1.txt” file contains information about an Adjacency matrix (see the file’s format in
// // Table 1, column 1). Read the file and convert the matrix to a corresponding Adjacency list.
// void readGraphMatrixFromFile(std::vector<std::vector<int>> &matrix, std::string filename) {
//     std::fstream file(filename, std::ios::in);
//     if (!file.is_open()) {
//         std::cout << "Cannot open file " << filename << '\n';
//         return;
//     }

//     int n;
//     file >> n;
//     matrix.resize(n, std::vector<int>(n, 0));

//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             file >> matrix[i][j];
//         }
//     }

//     file.close();
// }

// std::vector<std::vector<int>> convertGraphMatrixToList(std::vector<std::vector<int>> matrix) {
//     std::vector<std::vector<int>> list(matrix.size());

//     for (int i = 0; i < matrix.size(); i++) {
//         for(int j = 0; j < matrix[i].size(); j++) {
//             if (matrix[i][j] == 1) {
//                 list[i].push_back(j);
//             }
//         }
//     }

//     return list;
// }

// void printAdjacencyMatrix(std::vector<std::vector<int>> matrix) {
//     for (std::vector<int> row : matrix) {
//         for (int i : row) {
//             std::cout << i << ' ';
//         }
//         std::cout << '\n';
//     }
// }

// // 2. The “graph2.txt” file contains information about an Adjacency list (see the file’s format in Table 1,
// // column 2). Read the file and convert the list to a corresponding Adjacency matrix.
// void readGraphListFromFile(std::vector<std::vector<int>> &list, std::string filename) {
//     std::fstream file(filename, std::ios::in);
//     if (!file.is_open()) {
//         std::cout << "Cannot open file " << filename << '\n';
//         return;
//     }

//     int n;
//     file >> n;
//     file.ignore();
//     list.resize(n);

//     for (int i = 0; i < n; i++) {
//         std::string line;
//         std::getline(file, line);
//         std::stringstream ss(line);
//         int vertex;
//         while (ss >> vertex) {
//             list[i].push_back(vertex);
//         }
//     }

//     file.close();
// }

// std::vector<std::vector<int>> convertGraphListToMatrix(std::vector<std::vector<int>> list) {
//     std::vector<std::vector<int>> matrix(list.size(), std::vector<int>(list.size(), 0));

//     for (int i = 0; i < list.size(); i++) {
//         for (int j : list[i]) {
//             matrix[i][j] = 1;
//         }
//     }

//     return matrix;
// }

// // 3. Implement functions to provide the following information about a graph:
// // • Graph type (undirected or directed).
// bool isUndirectedGraph (std::vector<std::vector<int>> matrix) {
//     for (int i = 0; i < matrix.size(); i++) {
//         for (int j = 0; j < matrix.size(); j++) {
//             if (matrix[i][j] != matrix[j][i]) return false;
//         }
//     }
//     return true;
// }

// void printGraphType(std::vector<std::vector<int>> matrix) {
//     std::cout << "This is a ";

//     if (isUndirectedGraph(matrix)) {
//         std::cout << "Undirected graph\n";
//     } else {
//         std::cout << "Directed graph\n";
//     }
// }

// // • Number of vertices and edges.
// int countVertices(std::vector<std::vector<int>> matrix) {
//     return matrix.size();
// }

// int countEdges(std::vector<std::vector<int>> matrix) {
//     int count = 0;
//     for (int i = 0; i < matrix.size(); i++) {
//         for (int j = 0; j < matrix.size(); j++) {
//             count += matrix[i][j];
//         }
//     }
//     return count;
// }

// void printVerticesAndEdges(std::vector<std::vector<int>> matrix) {
//     std::cout << "Number of vertices: " << countVertices(matrix) << '\n';
//     std::cout << "Number of edges: " << countEdges(matrix) << '\n';
// }

// // • Vertex degrees (undirected) or in-degrees and out-degrees (directed).
// int vertexDegree(std::vector<std::vector<int>> matrix, int vertex) {
//     int degree = 0;
//     for (int i = 0; i < matrix.size(); i++) {
//         degree += matrix[vertex][i];
//     }
//     return degree;
// }

// int inDegree(std::vector<std::vector<int>> matrix, int vertex) {
//     int degree = 0;
//     for (int i = 0; i < matrix.size(); i++) {
//         degree += matrix[i][vertex];
//     }
//     return degree;
// }

// int outDegree(std::vector<std::vector<int>> matrix, int vertex) {
//     return vertexDegree(matrix, vertex);
// }

// void printVertexDegrees(std::vector<std::vector<int>> matrix, int vertex) {
//     if (isUndirectedGraph(matrix)) {
//         std::cout << "Degree of vertex " << vertex << " : " << vertexDegree(matrix, vertex) << '\n';
//     } else {
//         std::cout << "In-degree of vertex " << vertex << " : " << inDegree(matrix, vertex) << '\n';
//         std::cout << "Out-degree of vertex " << vertex << " : " << outDegree(matrix, vertex) << '\n';
//     }
// }

// // • Identify isolated vertices and leaf vertices.
// std::vector<int> isolatedVertices(std::vector<std::vector<int>> matrix) {
//     std::vector<int> isolated;
//     if (isUndirectedGraph(matrix)) {
//         for (int i = 0; i < matrix.size(); i++) {
//             if (vertexDegree(matrix, i) == 0) {
//                 isolated.push_back(i);
//             }
//         }
//     } else {
//         for (int i = 0; i < matrix.size(); i++) {
//             if (inDegree(matrix, i) == 0 && outDegree(matrix, i) == 0) {
//                 isolated.push_back(i);
//             }
//         }
//     }
//     return isolated;
// }

// std::vector<int> leafVertices(std::vector<std::vector<int>> matrix) {
//     if (isUndirectedGraph(matrix)) {
//         std::vector<int> leaf;
//         for (int i = 0; i < matrix.size(); i++) {
//             if (vertexDegree(matrix, i) == 1) {
//                 leaf.push_back(i);
//             }
//         }
//         return leaf;
//     } else {
//         std::vector<int> leaf;
//         for (int i = 0; i < matrix.size(); i++) {
//             if (outDegree(matrix, i) == 0 && inDegree(matrix, i) == 1) {
//                 leaf.push_back(i);
//             }
//         }
//         return leaf;
//     }
// }

// void printIsolatedVertices(std::vector<std::vector<int>> matrix) {
//     std::vector<int> isolated = isolatedVertices(matrix);
//     if (isolated.size() == 0) {
//         std::cout << "No isolated vertices\n";
//         return;
//     }

//     std::cout << "Isolated vertices: ";
//     for (int i = 0; i < isolated.size(); i++) {
//         std::cout << isolated[i] << ' ';
//     }
//     std::cout << '\n';
// }

// void printLeafVertices(std::vector<std::vector<int>> matrix) {
//     std::vector<int> leaf = leafVertices(matrix);
//     if (leaf.size() == 0) {
//         std::cout << "No leaf vertices\n";
//         return;
//     }

//     std::cout << "Leaf vertices: ";
//     for (int i = 0; i < leaf.size(); i++) {
//         std::cout << leaf[i] << ' ';
//     }
//     std::cout << '\n';
// }

// // • Determine if the graph is complete, circular, bipartite graph, or complete bipartite graph.
// bool isCompleteGraph(std::vector<std::vector<int>> matrix) {
//     for (int i = 0; i < matrix.size(); i++) {
//         for (int j = 0; j < matrix.size(); j++) {
//             if (i != j && matrix[i][j] == 0) return false;
//         }
//     }
//     return true;
// }

// bool isCircularGraph(std::vector<std::vector<int>> matrix) {
//     if (!isUndirectedGraph(matrix)) return false;
//     for (int i = 0; i < matrix.size(); i++) {
//         if (vertexDegree(matrix, i) != 2) return false;
//     }
//     return true;
// }

// bool isBipartiteGraph(std::vector<std::vector<int>> matrix) {
//     std::vector<int> color(matrix.size(), -1);
//     std::queue<int> q;
//     q.push(0);
//     color[0] = 0;
//     while (!q.empty()) {
//         int u = q.front();
//         q.pop();
//         for (int i = 0; i < matrix.size(); i++) {
//             if (matrix[u][i] == 1) {
//                 if (color[i] == -1) {
//                     color[i] = 1 - color[u];
//                     q.push(i);
//                 } else if (color[i] == color[u]) {
//                     return false;
//                 }
//             }
//         }
//     }
//     return true;
// }

// bool isCompleteBipartiteGraph(std::vector<std::vector<int>> matrix) {
//     if (!isBipartiteGraph(matrix)) return false;
//     for (int i = 0; i < matrix.size(); i++) {
//         for (int j = 0; j < matrix.size(); j++) {
//             if (matrix[i][j] == 0 && i != j) return false;
//         }
//     }
//     return true;
// }

// void checkSpecialGraph(std::vector<std::vector<int>> matrix) {
//     std::cout << "This is a ";

//     if (isCompleteGraph(matrix)) {
//         std::cout << "Complete graph\n";
//     } else if (isCircularGraph(matrix)) {
//         std::cout << "Circular graph\n";
//     } else if (isBipartiteGraph(matrix)) {
//         std::cout << "Bipartite graph\n";
//     } else if (isCompleteBipartiteGraph(matrix)) {
//         std::cout << "Complete bipartite graph\n";
//     } else {
//         std::cout << "not a special graph\n";
//     }
// }

// // • Number of connected components. How many of them are trees?
// void dfs(std::vector<std::vector<int>> matrix, std::vector<int> &visited, int vertex) {
//     visited[vertex] = 1;
//     for (int i = 0; i < matrix.size(); i++) {
//         if (matrix[vertex][i] == 1 && !visited[i]) {
//             dfs(matrix, visited, i);
//         }
//     }
// }

// int countConnectedComponents(std::vector<std::vector<int>> matrix) {
//     int count = 0;
//     std::vector<int> visited(matrix.size(), 0);

//     for (int i = 0; i < matrix.size(); i++) {
//         if (!visited[i]) {
//             dfs(matrix, visited, i);
//             count++;
//         }
//     }

//     return count;
// }

// void dfsConnectedComponements(std::vector<std::vector<int>> matrix, std::vector<int>& visited, int &vertexCount, int &edgeCount, int vertex) {
//     visited[vertex] = 1;
//     vertexCount++;
//     for (int i = 0; i < matrix.size(); i++) {
//         if (matrix[vertex][i] == 1) {
//             edgeCount++;
//             if (!visited[i]) {
//                 dfsConnectedComponements(matrix, visited, vertexCount, edgeCount, i);
//             }
//         }
//     }
// }

// int countTreesInConnectedComponents(std::vector<std::vector<int>> matrix) {
//     int count = 0;
//     std::vector<int> visited(matrix.size(), 0);
//     for (int i = 0; i < matrix.size(); i++) {
//         if (!visited[i]) {
//             int vertexCount = 0;
//             int edgeCount = 0;
//             dfsConnectedComponements(matrix, visited, vertexCount, edgeCount, i);
//             if (edgeCount == vertexCount - 1) {
//                 count++;
//             }
//         }
//     }
//     return count;
// }

// void printConnectedComponentsAndTreesInIt(std::vector<std::vector<int>> matrix) {
//     if (!isUndirectedGraph(matrix)) {
//         matrix = generateBaseUndirectedGraph(matrix);
//     }

//     std::cout << "Number of connected components: " << countConnectedComponents(matrix) << '\n';
//     std::cout << "Number of trees in connected components: " << countTreesInConnectedComponents(matrix) << '\n';
// }

// // • Number of cut vertices and bridge edges
// int countCutVertices(std::vector<std::vector<int>> matrix) {
//     int count = 0;
//     for (int i = 0; i < matrix.size(); i++) {
//         std::vector<std::vector<int>> temp = matrix;
//         temp.erase(temp.begin() + i);
//         for (int j = 0; j < temp.size(); j++) {
//             temp[j].erase(temp[j].begin() + i);
//         }
//         if (countConnectedComponents(temp) > countConnectedComponents(matrix)) {
//             count++;
//         }
//     }
//     return count;
// }

// void dfsBridgeEdges(std::vector<std::vector<int>> matrix, std::vector<int> &visited, std::vector<int> &parent, std::vector<int> &low, std::vector<int> &disc, int &time, int vertex) {
//     visited[vertex] = 1;
//     disc[vertex] = low[vertex] = ++time;
//     for (int i = 0; i < matrix.size(); i++) {
//         if (matrix[vertex][i] == 1) {
//             if (!visited[i]) {
//                 parent[i] = vertex;
//                 dfsBridgeEdges(matrix, visited, parent, low, disc, time, i);
//                 low[vertex] = std::min(low[vertex], low[i]);
//                 if (low[i] > disc[vertex]) {
//                     std::cout << vertex << ' ' << i << '\n';
//                 }
//             } else if (i != parent[vertex]) {
//                 low[vertex] = std::min(low[vertex], disc[i]);
//             }
//         }
//     }
// }


// int countBridgeEdges(std::vector<std::vector<int>> matrix) {
//     int count = 0;
//     std::vector<int> visited(matrix.size(), 0);
//     std::vector<int> parent(matrix.size(), -1);
//     std::vector<int> low(matrix.size(), -1);
//     std::vector<int> disc(matrix.size(), -1);
//     int time = 0;
//     for (int i = 0; i < matrix.size(); i++) {
//         if (!visited[i]) {
//             dfsBridgeEdges(matrix, visited, parent, low, disc, time, i);
//         }
//     }
//     return count;
// }

// // 4. Generate a base undirected graph from a given directed graph.
// std::vector<std::vector<int>> generateBaseUndirectedGraph(std::vector<std::vector<int>> matrix) {
//     std::vector<std::vector<int>> base(matrix.size(), std::vector<int>(matrix.size(), 0));

//     for (int i = 0; i < matrix.size(); i++) {
//         for (int j = 0; j < matrix.size(); j++) {
//             if (matrix[i][j] == 1) {
//                 base[i][j] = base[j][i] = 1;
//             }
//         }
//     }

//     return base;
// }

// // 5. Generate a complement (inverse) graph from a given undirected graph, outputting the corresponding adjacency matrix.
// std::vector<std::vector<int>> generateComplementGraph(std::vector<std::vector<int>> matrix) {
//     std::vector<std::vector<int>> complement(matrix.size(), std::vector<int>(matrix.size(), 0));
//     for (int i = 0; i < matrix.size(); i++) {
//         for (int j = 0; j < matrix.size(); j++) {
//             if (i != j && matrix[i][j] == 0) {
//                 complement[i][j] = complement[j][i] = 1;
//             }
//         }
//     }
//     return complement;
// }

// // 6. Generate a converse graph from a given directed graph, outputting the corresponding adjacency matrix
// std::vector<std::vector<int>> generateConverseGraph(std::vector<std::vector<int>> matrix) {
//     std::vector<std::vector<int>> converse(matrix.size(), std::vector<int>(matrix.size(), 0));
//     for (int i = 0; i < matrix.size(); i++) {
//         for (int j = 0; j < matrix.size(); j++) {
//             converse[j][i] = matrix[i][j];
//         }
//     }
//     return converse;
// }

// // 7. Find an Eulerian circuit/path of a graph using Hierholzer’s algorithm.
// void dfsEulerian(std::vector<std::vector<int>> &matrix, std::vector<int> &result, int vertex, bool isDirected) {
//     for (int i = 0; i < matrix.size(); i++) {
//         while (matrix[vertex][i] > 0) { // Ensure all edges are traversed
//             matrix[vertex][i]--;
//             if (!isDirected) matrix[i][vertex]--;
//             dfsEulerian(matrix, result, i, isDirected);
//         }
//     }
//     result.push_back(vertex);
// }

// void printEulerian(std::vector<std::vector<int>> matrix, bool isDirected) {
//     std::vector<int> result;
//     int start = -1, end = -1;

//     std::vector<int> inDegree(matrix.size(), 0), outDegree(matrix.size(), 0);
//     for (int i = 0; i < matrix.size(); i++) {
//         for (int j = 0; j < matrix.size(); j++) {
//             if (matrix[i][j] > 0) {
//                 outDegree[i] += matrix[i][j];
//                 inDegree[j] += matrix[i][j];
//             }
//         }
//     }

//     if (isDirected) {
//         for (int i = 0; i < matrix.size(); i++) {
//             if (outDegree[i] - inDegree[i] == 1) start = i;
//             else if (inDegree[i] - outDegree[i] == 1) end = i;
//         }
//         if (start == -1) {
//             for (int i = 0; i < matrix.size(); i++) {
//                 if (outDegree[i] > 0) {
//                     start = i;
//                     break;
//                 }
//             }
//         }
//     } else {
//         int oddCount = 0;
//         for (int i = 0; i < matrix.size(); i++) {
//             if ((outDegree[i] + inDegree[i]) % 2 == 1) {
//                 oddCount++;
//                 if (start == -1) start = i;
//             }
//         }
//         if (oddCount > 2) {
//             std::cout << "No Eulerian path or circuit.\n";
//             return;
//         }
//         if (start == -1) {
//             for (int i = 0; i < matrix.size(); i++) {
//                 if (outDegree[i] > 0) {
//                     start = i;
//                     break;
//                 }
//             }
//         }
//     }

//     if (start == -1) {
//         std::cout << "Graph is empty or has no Eulerian path/circuit.\n";
//         return;
//     }

//     dfsEulerian(matrix, result, start, isDirected);

//     std::cout << (end == -1 ? "Eulerian Circuit: " : "Eulerian Path: ");
//     for (int i = result.size() - 1; i >= 0; i--) {
//         std::cout << result[i] << ' ';
//     }
//     std::cout << '\n';
// }

// // 8. Find a spanning tree of a graph using:
// // • Depth-first Search (DFS).
// void dfsSpanningTree(const std::vector<std::vector<int>> &matrix, std::vector<int> &visited, std::vector<std::vector<int>> &tree, int vertex) {
//     visited[vertex] = 1;
//     for (int i = 0; i < matrix.size(); i++) {
//         if (matrix[vertex][i] == 1 && !visited[i]) {
//             tree[vertex][i] = tree[i][vertex] = 1;
//             dfsSpanningTree(matrix, visited, tree, i);
//         }
//     }
// }

// // • Breadth-first Search (BFS).
// void bfsSpanningTree(const std::vector<std::vector<int>> &matrix, std::vector<int> &visited, std::vector<std::vector<int>> &tree, int vertex) {
//     std::queue<int> queue;
//     queue.push(vertex);
//     visited[vertex] = 1;
//     while (!queue.empty()) {
//         int u = queue.front();
//         queue.pop();
//         for (int i = 0; i < matrix.size(); i++) {
//             if (matrix[u][i] == 1 && !visited[i]) {
//                 tree[u][i] = tree[i][u] = 1;
//                 visited[i] = 1;
//                 queue.push(i);
//             }
//         }
//     }
// }

// void printAdjacencyMatrix(std::vector<std::vector<int>> matrix) {
//     for (const auto &row : matrix) {
//         for (int cell : row) {
//             std::cout << cell << ' ';
//         }
//         std::cout << '\n';
//     }
// }

// void printSpanningTree(const std::vector<std::vector<int>> &matrix) {
//     std::vector<int> visited(matrix.size(), 0);
//     std::vector<std::vector<int>> tree(matrix.size(), std::vector<int>(matrix.size(), 0));

//     std::cout << "Spanning tree using DFS:\n";
//     dfsSpanningTree(matrix, visited, tree, 0);
//     printAdjacencyMatrix(tree);

//     visited.assign(matrix.size(), 0);
//     tree.assign(matrix.size(), std::vector<int>(matrix.size(), 0));

//     std::cout << "Spanning tree using BFS:\n";
//     bfsSpanningTree(matrix, visited, tree, 0);
//     printAdjacencyMatrix(tree);
// }