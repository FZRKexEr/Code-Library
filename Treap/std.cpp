#include <cstdio>
#include <cstdlib>
#include <ctime>
#define N 500005
using namespace std;
int inline read()
{
    int x=0,f=1;char ch=getchar();
    while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
    while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
    return x*f;
}

int ch[N][2],val[N],pri[N],siz[N],sz;


void update(int x){siz[x]=1+siz[ch[x][0]]+siz[ch[x][1]];}
int new_node(int v)
{
    siz[++sz]=1;
    val[sz]=v;
    pri[sz]=rand();
    return sz;
}
int merge(int x,int y)
{
    if (!x || !y) return x+y;
    if (pri[x]<pri[y])
    {
        ch[x][1]=merge(ch[x][1],y);
        update(x);
        return x;
    }
    else
    {
        ch[y][0]=merge(x,ch[y][0]);
        update(y);
        return y;
    }
}
void split(int now,int k,int &x,int &y)
{
    if (!now) x=y=0;
    else
    {
        if (val[now]<=k)
            x=now,split(ch[now][1],k,ch[now][1],y);
        else
            y=now,split(ch[now][0],k,x,ch[now][0]);
        update(now);
    }
}
int kth(int now,int k)
{
    while(1)
    {
        if (k<=siz[ch[now][0]])
            now=ch[now][0];
        else
        if (k==siz[ch[now][0]]+1)
            return now;
        else
            k-=siz[ch[now][0]]+1,now=ch[now][1];
    }
}
main()
{
    srand((unsigned)time(NULL));
    int T,com,x,y,z,a,b,root=0;
    scanf("%d",&T);
    while(T--)
    {
        com=read(),a=read();
        if (com==1)
        {
            split(root,a,x,y);
            root=merge(merge(x,new_node(a)),y);
        }
        else
        if (com==2)
        {
            split(root,a,x,z);
            split(x,a-1,x,y);
            y=merge(ch[y][0],ch[y][1]);
            root=merge(merge(x,y),z);
        }
        else
        if (com==3)
        {
            split(root,a-1,x,y);
            printf("%d\n",siz[x]+1);
            root=merge(x,y);
        }
        else
        if (com==4)
            printf("%d\n",val[kth(root,a)]);
        else
        if (com==5)
        {
            split(root,a-1,x,y);
            printf("%d\n",val[kth(x,siz[x])]);
            root=merge(x,y);
        }
        else
        {
            split(root,a,x,y);
            printf("%d\n",val[kth(y,1)]);
            root=merge(x,y);
        }
    }
}
