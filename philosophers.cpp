# OS_qs
#include<cstdio>
#include<cstdlib>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<bits/stdc++.h>
using namespace std;

#define N 5

enum state {THINKING,HUNGRY,EATING};

state philstate[N] ={THINKING}; 
sem_t mutex;
sem_t self[N];

void test(int i){
if(philstate[i]==HUNGRY && philstate[(i+1)%N]!=EATING && philstate[(i+4)%N]!=EATING)
{
	philstate[i]=EATING;
	cout<<"phil"<<i<<"is eating\n";
	sem_post(&self[i]);
}
}

void pickup(int i){
sem_wait(&mutex);
philstate[i]= HUNGRY;
cout<<"phil"<<i<<"is in hungry state\n";
test(i);
sem_post(&mutex);
if(philstate[i]!=EATING)
sem_wait(&self[i]);
sleep(1);
}

void putdown(int i){
sem_wait(&mutex);
philstate[i]=THINKING;
cout<<"phil"<<i<<"is in thinking state\n";
test((i+4)%N);
test((i+1)%N);
sem_post(&mutex);
sleep(1);
}

void * phil(void *arg){
int *i = (int*)arg;
while(1){
pickup(*i);
sleep(2);
putdown(*i);
}
}
 
int main(){
pthread_t tid[N];
int pid[]={0,1,2,3,4};
sem_init(&mutex,0,1);

for(int i=0;i<N;i++)
sem_init(&self[i],0,0);

for(int i=0;i<N;i++)
pthread_create(&tid[i],NULL,&phil,(void *)&pid[i]);

for(int i=0;i<N;i++)
pthread_join(tid[i],NULL);
return 0;
}
