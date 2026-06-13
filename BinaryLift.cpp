#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

/*
Leetcode 3559: Number of ways to assign edge weights II

Things to Note:
- Needs to find distance betweeen each node in queries and then use ODD = EVEN binomial theorem property and return 2^(dist - 1) for each query
- Needs to use Least Common Ancestor (LCA) to calculate dist quicky: depth[u] + depth[v] - 2 * depth[lca(u, v)]
- Needs Binary Lifting to quickly calcualte LCA
    - uses a 2d up array where up[node][i] = the 2^i-th parent of node

Filling depth and up with dfs:
- set node's immediate parent in up
- fill supsequent entries of up for node using rule that 2^k-th ancestor is the same is the 2^(k-1)-th ancestor of the 2^(k-1)-th ancestor

Calculating LCA:
- get dist between depths of node u and v
- move u up by a jump each time based on the set bits of dist
- move u and v together while their parents are not equal
*/

class Solution {
public:
    static const int MOD = 1e9 + 7;
    static const int LOG = 20;

    vector<int> assignEdgeWeights(vector<vector<int>>& edges, vector<vector<int>>& queries) {
        int n = edges.size() + 1;
        vector<vector<int>> graph(n + 1);
        for (auto& e : edges) {
            int u = e[0], v = e[1];
            graph[u].push_back(v);
            graph[v].push_back(u);
        }

        vector<int> depth(n + 1, 0);
        vector<vector<int>> up(n + 1, vector<int>(LOG, 0));

        auto dfs = [&](auto&& self, int node, int parent) -> void {
            up[node][0] = parent;

            for (int j = 1; j < LOG; ++j) {
                up[node][j] = up[up[node][j - 1]][j - 1];
            }

            for (int nei : graph[node]) {
                if (nei == parent) continue;
                depth[nei] = depth[node] + 1;
                self(self, nei, node);
            }
        };

        dfs(dfs, 1, 1); 

        auto lca = [&](int u, int v) {
            if (depth[u] < depth[v]) swap(u, v);

            int diff = depth[u] - depth[v];

            for (int j = LOG - 1; j >= 0; --j) {
                if (diff & (1 << j)) {
                    u = up[u][j];
                }
            }

            if (u == v) return u;

            for (int j = LOG - 1; j >= 0; --j) {
                if (up[u][j] != up[v][j]) {
                    u = up[u][j];
                    v = up[v][j];
                }
            }

            return up[u][0];
        };

        vector<int> pow2(n + 1, 1);
        for (int i = 1; i < n + 1; ++i) {
            pow2[i] = (pow2[i - 1] * 2) % MOD;
        }

        vector<int> res;

        for (auto& q : queries) {
            int u = q[0], v = q[1];
            
            int dist = depth[u] + depth[v] - 2 * depth[lca(u, v)];
            
            if (dist == 0) res.push_back(0);
            else res.push_back(pow2[dist - 1]);
        }

        return res;
    }
};
