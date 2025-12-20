#include <bits/stdc++.h>

using namespace std;

uint64_t compute_num_paths(const string& a, const string& b,
                           map<string, vector<string>>& m) {
  map<string, uint64_t> memo;
  map<string, bool> visiting;

  function<uint64_t(const string&)> dfs = [&](const string& u) -> uint64_t {
    if (u == b) {
      return 1ULL;
    }

    // cycle detected → infinite paths
    if (visiting[u]) {
      return UINT64_MAX;
    }

    if (memo.count(u)) {
      return memo[u];
    }

    visiting[u] = true;
    uint64_t total = 0ULL;

    for (const auto& v : m[u]) {
      uint64_t paths = dfs(v);

      if (paths == UINT64_MAX) {
        visiting[u] = false;
        return UINT64_MAX;
      }

      total += paths;
    }

    visiting[u] = false;
    memo[u] = total;
    return total;
  };

  return dfs(a);
}
