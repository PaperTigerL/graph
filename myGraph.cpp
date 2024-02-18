#include "myGraph.h"
#include <iostream>
#include <sstream>
#include <string>
#include <queue>

using namespace std;

EdgeNode::EdgeNode() : dest(0), link(nullptr) {
    // 使用初始化列表来设置成员变量
}

EdgeNode::EdgeNode(int dest, EdgeNode* link) : dest(dest), link(link) {
    // 使用初始化列表来设置成员变量，并避免命名冲突
}

EdgeNode::~EdgeNode() {
    // 析构函数应该是空的，管理链表中节点的删除应当在链表数据结构的析构函数中处理
    // 因此这里不需要 delete link;
}

int EdgeNode::getDest() {
    return dest;
}

EdgeNode* EdgeNode::getNext() {
    return link;
}

void EdgeNode::setDest(int dest) {
    this->dest = dest;
}

void EdgeNode::setNext(EdgeNode* link) {
    this->link = link;
}

VertexNode::VertexNode() {
    data = '\0';
    firstEdge = nullptr;
}
VertexNode::VertexNode(char data, EdgeNode* firstEdge)
    : data(data), firstEdge(firstEdge) {
    // 使用初始化列表初始化成员变量
}

VertexNode::~VertexNode() {
    // 在析构时删除所有边节点
    EdgeNode* current = firstEdge;
    while (current != nullptr) {
        EdgeNode* toDelete = current;
        current = current->getNext();
        delete toDelete;
    }
}

char VertexNode::getData() {
    return data; // 返回顶点的数据
}

EdgeNode* VertexNode::getFirstEdge() {
    return firstEdge; // 返回指向第一个边节点的指针
}

void VertexNode::setData(char data) {
    this->data = data; // 设置顶点的数据
}

void VertexNode::setFirstEdge(EdgeNode* firstEdge) {
    this->firstEdge = firstEdge; // 设置指向第一个边节点的指针
}

MyGraph::MyGraph() : nodeNum(0), edgeNum(0), VexList(nullptr) {
    // 默认构造函数
}

MyGraph::MyGraph(int nodeNum, int edgeNum, char* nodeList, int* edgeStartList, int* edgeEndList)
    : nodeNum(nodeNum), edgeNum(edgeNum) {
    VexList = new VertexNode[nodeNum];
    for (int i = 0; i < nodeNum; ++i) {
        VexList[i].setData(nodeList[i]);
    }

    // 添加边到无向图中
    for (int i = 0; i < edgeNum; ++i) {
        int start = edgeStartList[i];
        int end = edgeEndList[i];

        // 添加边到起点顶点的邻接表
        EdgeNode* newEdgeTo = new EdgeNode(end, VexList[start].getFirstEdge());
        VexList[start].setFirstEdge(newEdgeTo);

        // 添加边到终点顶点的邻接表（无向图需要同时添加反向边）
        EdgeNode* newEdgeFrom = new EdgeNode(start, VexList[end].getFirstEdge());
        VexList[end].setFirstEdge(newEdgeFrom);
    }
}

MyGraph::~MyGraph() {
    delete[] VexList;
}

int MyGraph::getNodeNum() {
    return nodeNum;
}

int MyGraph::getEdgeNum() {
    return edgeNum;
}

string MyGraph::printGraph() {
    stringstream ss;

    for (int i = 0; i < nodeNum; ++i) {
        ss << "Vertex " << VexList[i].getData() << " -> ";
        EdgeNode* current = VexList[i].getFirstEdge();
        while (current != nullptr) {
            ss << VexList[current->getDest()].getData() << " ";
            current = current->getNext();
        }
        ss << endl;
    }

    return ss.str();
}

int MyGraph::getFirstNeighbor(int v, int* visited) {
    EdgeNode* current = VexList[v].getFirstEdge();
    while (current != nullptr) {
        if (visited[current->getDest()] == 0) {
            return current->getDest();
        }
        current = current->getNext();
    }
    return -1;
}

int MyGraph::getNextNeighbor(int v, int w, int* visited) {
    EdgeNode* current = VexList[v].getFirstEdge();
    while (current != nullptr && current->getDest() != w) {
        current = current->getNext();
    }
    if (current != nullptr) {
        current = current->getNext();
        while (current != nullptr) {
            if (visited[current->getDest()] == 0) {
                return current->getDest();
            }
            current = current->getNext();
        }
    }
    return -1;
}

void MyGraph::DFS(int v, int* visited, string& result) {
    visited[v] = 1;
    result += VexList[v].getData();
    int w = getFirstNeighbor(v, visited);
    while (w != -1) {
        if (visited[w] == 0) {
            DFS(w, visited, result);
        }
        w = getNextNeighbor(v, w, visited);
    }
}

string MyGraph::graph_DFS_Traverse() {
    string result;
    int* visited = new int[nodeNum]();
    for (int i = 0; i < nodeNum; ++i) {
        if (visited[i] == 0) {
            DFS(i, visited, result);
        }
    }
    delete[] visited;
    return result;
}

void MyGraph::BFS(int v, int* visited, string& result) {
    queue<int> q;
    q.push(v);
    visited[v] = 1;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        result += VexList[cur].getData();

        EdgeNode* current = VexList[cur].getFirstEdge();
        while (current != nullptr) {
            int dest = current->getDest();
            if (visited[dest] == 0) {
                q.push(dest);
                visited[dest] = 1;
            }
            current = current->getNext();
        }
    }
}

string MyGraph::graph_BFS_Traverse() {
    string result;
    int* visited = new int[nodeNum]();
    for (int i = 0; i < nodeNum; ++i) {
        if (visited[i] == 0) {
            BFS(i, visited, result);
        }
    }
    delete[] visited;
    return result;
}
