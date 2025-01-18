#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <limits.h>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

typedef pair<int, int> pii;

// 传统Dijkstra算法
void dijkstra(const vector<vector<pii>>& graph, int start, vector<int>& dist) {
    int n = graph.size();
    dist.assign(n, INT_MAX);
    dist[start] = 0;

    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({ 0, start });

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto& edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({ dist[v], v });
            }
        }
    }
}

// 动态Dijkstra算法（简化版，仅更新受影响节点）
void dynamicDijkstra(vector<vector<pii>>& graph, int start, vector<int>& dist, int u, int v, int new_w) {
    // 更新边权重
    for (auto& edge : graph[u]) {
        if (edge.first == v) {
            edge.second = new_w;
            break;
        }
    }
    for (auto& edge : graph[v]) {
        if (edge.first == u) {
            edge.second = new_w;
            break;
        }
    }

    // 重新计算受影响节点的最短路径
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({ dist[u], u });
    pq.push({ dist[v], v });

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto& edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({ dist[v], v });
            }
        }
    }
}

int main() {
    int n = 1000; // 节点数
    int m = 5000; // 边数
    int k = 100;  // 修改次数

    // 随机生成图
    vector<vector<pii>> graph(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < m; ++i) {
        int u = rand() % n;
        int v = rand() % n;
        int w = dis(gen);
        graph[u].push_back({ v, w });
        graph[v].push_back({ u, w });
    }

    int start = 0; // 起点
    vector<int> dist;

    // 测试传统Dijkstra算法
    auto start_time = high_resolution_clock::now();
    dijkstra(graph, start, dist);
    for (int i = 0; i < k; ++i) {
        int u = rand() % n;
        int v = rand() % n;
        int new_w = dis(gen);
        // 更新边权重
        for (auto& edge : graph[u]) {
            if (edge.first == v) {
                edge.second = new_w;
                break;
            }
        }
        for (auto& edge : graph[v]) {
            if (edge.first == u) {
                edge.second = new_w;
                break;
            }
        }
        // 重新计算最短路径
        dijkstra(graph, start, dist);
    }
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Traditional Dijkstra total time: " << duration.count() << " ms" << endl;

    // 测试动态Dijkstra算法
    start_time = high_resolution_clock::now();
    dijkstra(graph, start, dist);
    for (int i = 0; i < k; ++i) {
        int u = rand() % n;
        int v = rand() % n;
        int new_w = dis(gen);
        // 动态更新最短路径
        dynamicDijkstra(graph, start, dist, u, v, new_w);
    }
    end_time = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Dynamic Dijkstra total time: " << duration.count() << " ms" << endl;

    return 0;
}