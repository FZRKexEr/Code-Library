#include <stdio.h>

typedef long long ll;

const int N = 1e6, M = 3, K = 1e3, P = 5e3 + 7, mod = 998244353;
// N 值域范围, M = 3 固定, K 等于 sqrt(N), P 不管, mod 模数 
// 代码来自 洛谷 Leasier https://www.luogu.com.cn/blog/Leasier/Quick-GCD-Based-on-Range-Preprocessing
// 功能: O(n) 预处理值域后 O(1) gcd

int cnt = 0;
int prime[N + 7], decompand[N + 7][M + 1], pre_gcd[K + 7][K + 7], a[P], b[P];
bool p[N + 7];

inline void init(){
	int cnt = 0;
	p[0] = p[1] = true;
	decompand[1][1] = decompand[1][2] = decompand[1][3] = 1;
	for (int i = 2; i <= N; i++){
		if (!p[i]){
			prime[++cnt] = i;
			decompand[i][1] = decompand[i][2] = 1;
			decompand[i][3] = i;
		}
		for (int j = 1; j <= cnt && i * prime[j] <= N; j++){
			int t1 = i * prime[j], t2 = decompand[i][1] * prime[j];
			p[t1] = true;
            if (t2 < decompand[i][2]){
                decompand[t1][1] = t2;
                decompand[t1][2] = decompand[i][2];
                decompand[t1][3] = decompand[i][3];
            } else if (t2 < decompand[i][3]){
                decompand[t1][1] = decompand[i][2];
                decompand[t1][2] = t2;
                decompand[t1][3] = decompand[i][3];
            } else {
                decompand[t1][1] = decompand[i][2];
                decompand[t1][2] = decompand[i][3];
                decompand[t1][3] = t2;
            }
			if (i % prime[j] == 0) break;
		}
	}
	for (int i = 1; i <= K; i++){
		pre_gcd[0][i] = pre_gcd[i][0] = pre_gcd[i][i] = i;
		for (int j = 1; j < i; j++){
			pre_gcd[i][j] = pre_gcd[j][i] = pre_gcd[j][i % j];
		}
	}
}

inline int quick_gcd(int a, int b){
	if (a <= K && b <= K) return pre_gcd[a][b];
	int ans = 1;
	for (int i = 1; i <= M; i++){
		int d;
		if (decompand[a][i] <= K){
			d = pre_gcd[decompand[a][i]][b % decompand[a][i]];
		} else if (b % decompand[a][i] == 0){
			d = decompand[a][i];
		} else {
			d = 1;
		}
		b /= d;
		ans *= d;
	}
	return ans;
}

int main(){
	int n;
	scanf("%d", &n);
	init();
	for (int i = 1; i <= n; i++){
		scanf("%d", &a[i]);
	}
	for (int i = 1; i <= n; i++){
		scanf("%d", &b[i]);
	}
	for (int i = 1; i <= n; i++){
		ll j = i, cur_ans = 0;
		for (int k = 1; k <= n; j = j * i % mod, k++){
			cur_ans = (cur_ans + j * quick_gcd(a[i], b[k]) % mod) % mod;
		}
		printf("%lld\n", cur_ans);
	}
	return 0;
}
