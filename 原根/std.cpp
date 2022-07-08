#include <bits/stdc++.h>
using namespace std;
const int MAXN=1000010;
int t,p,cnt,tot,ctans,fc[MAXN],ans[MAXN],pri[MAXN],rt[MAXN],q[MAXN],phi[MAXN];
void init () {
	phi[1]=1;
	for (int i=2;i<=MAXN-10;i++) {
		if (!q[i]) {pri[++tot]=i,phi[i]=i-1;}
		for (int j=1;j<=tot&&pri[j]*i<=MAXN-10;j++) {
			q[i*pri[j]]=1;
			if (i%pri[j]==0) {
				phi[i*pri[j]]=phi[i]*pri[j];
				break;
			}
			phi[i*pri[j]]=phi[i]*(pri[j]-1);
		}
	}
	rt[2]=rt[4]=1;
	for (int i=2;i<=tot;i++) {
		for (int j=1;(1ll*j*pri[i])<=MAXN-10;j*=pri[i]) {rt[j*pri[i]]=1;}
		for (int j=2;(1ll*j*pri[i])<=MAXN-10;j*=pri[i]) {rt[j*pri[i]]=1;}
	}
	return;
}
int gcd (int a,int b) {return (b==0?a:gcd(b,a%b));}
int qpow (int a,int b,int p) {
	int res=1;
	while (b) {
		if (b&1) {res=(1ll*res*a)%p;}
		a=(1ll*a*a)%p;
		b>>=1;
	}
	return res;
}
void proc (int p) {
	for (int i=2;i*i<=p;i++) {
		if (p%i==0) {
			fc[++cnt]=i;
			while (p%i==0) {p/=i;}
		}
	}
	if (p>1) {fc[++cnt]=p;}
	return;
}
bool chk (int x,int p) {
	if (qpow(x,phi[p],p)!=1) {return 0;}
	for (int i=1;i<=cnt;i++) {
		if (qpow(x,phi[p]/fc[i],p)==1) {return 0;}
	}
	return 1;
}
int findrt (int p) {
	for (int i=1;i<p;i++) {
		if (chk(i,p)) {return i;}
	}
	return 0;
}
void getrt (int p,int x) {
	int prod=1;
	for (int i=1;i<=phi[p];i++) {
		prod=(1ll*prod*x)%p;
		if (gcd(i,phi[p])==1) {
			ans[++ctans]=prod;
		}
	}
	return;
}

int main () {
  freopen ("in.txt", "r", stdin);
  freopen("out1.txt", "w", stdout);
	init();
	scanf("%d",&t);
	for (int ii=1;ii<=t;ii++) {
		int wtf;
		scanf("%d%d",&p,&wtf);
		if (rt[p]) {
			ctans=cnt=0;
			proc(phi[p]);
			int mn=findrt(p);
			getrt(p,mn);
			sort(ans+1,ans+ctans+1);
			printf("%d\n",ctans); 
      for (int j = 1; j <= 10; j++) cout << ans[j] <<  " ";
			printf("\n");
		} else {
			printf("0\n\n");
		}
	}
	return 0;
}
