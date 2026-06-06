#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>

/*
Digit DP:
The question was LC 3753: Total Waviness of Numbers in Range II

Problem it solves: Use DP instead of iterating through all numbers within a range

How it works:
- need a memoization table. ex: dp[17][11][11][2][2] 
    - 17 for idx values representing 0 to 16th digit
    - 11 for all values 0-9 (10 is used as a placeholder for unassigned value)
    - 2 for boolean variables
- to solve iterate recursively from range 0 to target and accumulate the solution
- tight variable: keeps track of for a certain index if we can iterate through all digits or are capped at a certain digit
*/

using namespace std;

class Solution {
private:
    struct State {
        long long waviness;
        long long count;
    };

    State dp[17][11][11][2][2];
    string num_str;

    State solve(int idx, int prev1, int prev2, bool is_started, bool tight) {
        if (idx == num_str.size()) {
            return {0, (is_started ? 1LL : 0LL)};
        }

        if (dp[idx][prev1][prev2][is_started][tight].count != -1) {
            return dp[idx][prev1][prev2][is_started][tight];
        }

        long long total_waviness = 0;
        long long total_count = 0;

        int limit = tight ? num_str[idx] - '0' : 9;

        for (int cur = 0; cur <= limit; ++cur) {
            int next_tight = tight && cur == limit;

            if (!is_started && cur == 0) {
                State res = solve(idx + 1, 10, 10, false, next_tight);

                total_waviness += res.waviness;
                total_count += res.count;
            } else {
                State res = solve(idx + 1, cur, prev1, true, next_tight);

                long long cur_waviness_contribution = 0;

                if (is_started && prev1 != 10 && prev2 != 10) {
                    bool is_peak = prev1 > prev2 && prev1 > cur;
                    bool is_valley = prev1 < prev2 && prev1 < cur;

                    if (is_peak || is_valley) {
                        cur_waviness_contribution = res.count;
                    }
                }

                total_waviness +=
                    res.waviness + cur_waviness_contribution;
                total_count += res.count;
            }
        }

        dp[idx][prev1][prev2][is_started][tight] =
            {total_waviness, total_count};

        return dp[idx][prev1][prev2][is_started][tight];
    }

    long long get_waviness(long long n) {
        if (n < 100) return 0;

        num_str = to_string(n);

        for (int i = 0; i < 17; ++i) {
            for (int j = 0; j < 11; ++j) {
                for (int k = 0; k < 11; ++k) {
                    for (int l = 0; l < 2; ++l) {
                        for (int m = 0; m < 2; ++m) {
                            dp[i][j][k][l][m] = {-1, -1};
                        }
                    }
                }
            }
        }

        return solve(0, 10, 10, false, true).waviness;
    }

public:
    long long totalWaviness(long long num1, long long num2) {
        return get_waviness(num2) - get_waviness(num1 - 1);
    }
};
