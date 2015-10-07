# OS_qs
#include<cstdio>
#include<cstdlib>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<bits/stdc++.h>
using namespace std;

#define N 10
int nActive=0;
int nPendingreaders=0;
int nPendingwriters=0;
pthread_mutex_t m;
pthread_cond_t canread,canwrite;


void lockshared(int p){
pthread_mutex_lock(&m);
nPendingreaders++;
if(nPendingwriters>0)
pthread_cond_wait(&canread,&m);

while(nActive<0)
pthread_cond_wait(&canread,&m);

nActive++;
nPendingreaders--;
pthread_t pid = pthread_self();
cout<<"reader "<<(int)pid<<" is reading"<<p<<"\n";
pthread_mutex_unlock(&m);
sleep(1);
}

void unlockshared(int p){
pthread_mutex_lock(&m);
nActive--;
pthread_t pid = pthread_self();
cout<<"the reader "<<(int)pid<<" is done reading "<<p<<"\n";
if(nActive==0)
pthread_cond_signal(&canwrite);
pthread_mutex_unlock(&m);
sleep(1);
}

void lockexclusive(int p){
pthread_mutex_lock(&m);
nPendingwriters++;
if(nActive!=0)
pthread_cond_wait(&canwrite,&m);

nActive=-1;
nPendingwriters--;
pthread_t pid = pthread_self();
cout<<"the writer "<<(int)pid<<" is writing in "<<p<<"\n";
pthread_mutex_unlock(&m);
sleep(1);
}

void unlockexclusive(int p){
pthread_mutex_lock(&m);
nActive = 0;
pthread_t pid = pthread_self();
cout<<"the writer "<<(int)pid<<" is done writing in "<<p<<"\n";
if(nPendingreaders>0)
pthread_cond_broadcast(&canread);
else
pthread_cond_signal(&canwrite);
pthread_mutex_unlock(&m);
sleep(1);
}

void * reading(void *args){
int *i = (int *) args;
lockshared(*i);
sleep(2);
unlockshared(*i);
}

void * writing(void *args){
int *i = (int *) args;
lockexclusive(*i);
sleep(2);
unlockexclusive(*i);
}

int main(){

pthread_mutex_init(&m,NULL);
pthread_cond_init(&canwrite,NULL);
pthread_cond_init(&canread,NULL);

int a[5]={0,1,2,3,4};

pthread_t reader[10],writer[10];

for(int i=0;i<10;i++){
pthread_create(&reader[i],NULL,&reading,(void *)&a[i]);
pthread_create(&writer[i],NULL,&writing,(void *)&a[i]);
}

for(int i=0;i<10;i++)
pthread_join(reader[i],NULL);

for(int i=0;i<10;i++)
pthread_join(writer[i],NULL);

}

