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

// 双向Dijkstra算法
int bidirectional_dijkstra(const vector<vector<pii>>& graph, int start, int end) {
    int n = graph.size();
    vector<int> dist_start(n, INT_MAX);
    vector<int> dist_end(n, INT_MAX);
    dist_start[start] = 0;
    dist_end[end] = 0;

    priority_queue<pii, vector<pii>, greater<pii>> pq_start, pq_end;
    pq_start.push({ 0, start });
    pq_end.push({ 0, end });

    int min_dist = INT_MAX;

    while (!pq_start.empty() && !pq_end.empty()) {
        // 从起点方向搜索
        if (!pq_start.empty()) {
            int u = pq_start.top().second;
            int d = pq_start.top().first;
            pq_start.pop();

            if (d > dist_start[u]) continue;

            // 检查是否与终点方向的搜索相遇
            if (dist_end[u] != INT_MAX) {
                min_dist = min(min_dist, dist_start[u] + dist_end[u]);
                break; // 相遇时停止搜索
            }

            for (auto& edge : graph[u]) {
                int v = edge.first;
                int w = edge.second;

                if (dist_start[u] + w < dist_start[v]) {
                    dist_start[v] = dist_start[u] + w;
                    pq_start.push({ dist_start[v], v });
                }
            }
        }

        // 从终点方向搜索
        if (!pq_end.empty()) {
            int u = pq_end.top().second;
            int d = pq_end.top().first;
            pq_end.pop();

            if (d > dist_end[u]) continue;

            // 检查是否与起点方向的搜索相遇
            if (dist_start[u] != INT_MAX) {
                min_dist = min(min_dist, dist_start[u] + dist_end[u]);
                break; // 相遇时停止搜索
            }

            for (auto& edge : graph[u]) {
                int v = edge.first;
                int w = edge.second;

                if (dist_end[u] + w < dist_end[v]) {
                    dist_end[v] = dist_end[u] + w;
                    pq_end.push({ dist_end[v], v });
                }
            }
        }
    }

    return min_dist;
}

int main() {
    int n = 10000; // 中等规模图
    int m = 50000; // 边数
    vector<vector<pii>> graph(n);

    // 构建一个随机图
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

    int test_cases = 10; // 测试次数
    long long total_time_traditional = 0;
    long long total_time_bidirectional = 0;

    for (int i = 0; i < test_cases; ++i) {
        int start = rand() % n;
        int end = rand() % n;

        // 测试传统Dijkstra算法
        auto start_time = high_resolution_clock::now();
        vector<int> dist;
        dijkstra(graph, start, dist);
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end_time - start_time);
        total_time_traditional += duration.count();

        // 测试双向Dijkstra算法
        start_time = high_resolution_clock::now();
        int shortest_dist = bidirectional_dijkstra(graph, start, end);
        end_time = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(end_time - start_time);
        total_time_bidirectional += duration.count();
    }

    cout << "Average Traditional Dijkstra time: " << total_time_traditional / test_cases << " ms" << endl;
    cout << "Average Bidirectional Dijkstra time: " << total_time_bidirectional / test_cases << " ms" << endl;

    return 0;
}