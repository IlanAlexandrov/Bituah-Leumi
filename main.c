/*
 * main.c
 *
 *  Created on: Jun 8, 2020
 *      Author: Ilan
 *      		
 */
#include"BituachLeumi.h"
//how many requests
int requests=0;
//how many time for clerks do nothing
int times=0;
//how many clerks in total
int clerknum=0;
//total time of waiting to customers

int num=1;
void* mod(void*);
void* workMod();
int wait=0;
St *i_finished;
void* workElse(void* ssn);
sem_t counter;
int count_Finish=0;
int cntPlace;
pClerk clercks;
int temp;
St *s;
St *afterModiin;
int clin;
pCustomer cus;
struct timeval begin;
struct timeval finish;
int tot=0;
int avg;


int main(int argc,char* argv[]){
sem_init(&counter,0,1);
int i=0;
int j=0;

int k=0;
i_finished=(St*)malloc(sizeof(St));
FILE* in;
FILE* out=fopen("Output.txt","w");
if(argc!=2)
{
	printf("Wrong input\n");
	exit(0);
}
in=fopen(argv[1],"r");
if(in==NULL){
	printf("Can't open file");
	exit(1);
}
fscanf(in,"%d",&cntPlace);
//making a queue for who finished modiin
afterModiin=(St*)malloc(sizeof(St)*cntPlace);
afterModiin->q.head=NULL;
afterModiin->q.end=NULL;

s=(St*)malloc(sizeof(St)*cntPlace);
//the initialization for the station
fprintf(out,"Station initializing...\n");
while(i<cntPlace){
	sem_init(&s[i].area,0,1);
	sem_init(&s[i].empty,0,0);
	fscanf(in,"%s",s[i].name);


	s[i].q.head=NULL;
	s[i].q.end=NULL;
	s[i].name[strlen(s[i].name)]='\0';
	fscanf(in,"%d",&s[i].state);
	clerknum+=s[i].state;
	i++;
}
fprintf(out,"Stations are initialized\n");
clercks=(pClerk)malloc(sizeof(Clerk)*clerknum);

//----------------------------------------------------------------
fprintf(out,".\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t\n");
fprintf(out,"Initializing the Clerks...\n");
for(i=0;i<s[0].state;i++)
{
	clercks[i].id=i;
	clercks[i].service=0;
	clercks[i].work_time=0;
	pthread_create(&clercks[i].tid,NULL,workMod,(void*)&clercks[i]);
}

for(i=1;i<cntPlace;i++)
{
	for(k=0;k<s[i].state;k++)
	{
		clercks[num].id=num;
		clercks[num].service=i;
		clercks[num].work_time=0;
		pthread_create(&clercks[num].tid,NULL,workElse,(void*)&clercks[num]);
		num++;
	}
}
fprintf(out,"Clerks initialized\n");
fprintf(out,".\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t\n");
fprintf(out,"Getting custumers info...\n");
fprintf(out,"Clerks start to work\n");
	//check how many customers there are
	fscanf(in,"%d",&clin);
	gettimeofday(&begin,NULL);

	i=0;
	char tmp[20];

	while(i<clin){

		cus=(pCustomer)malloc(sizeof(Customer));
		fscanf(in,"%d %d %d",&cus->id,&cus->wait_time,&cus->sort_time);
		fscanf(in,"%s",tmp);
		tmp[strlen(tmp)]='\0';

		for(j=0;j<cntPlace;j++)
		{
			if(strcmp(s[j].name,tmp)==0)
			{
				cus->service=j;
			}
		}
		fscanf(in,"%d",&cus->service_time);

		gettimeofday(&cus->enter_time,NULL);

		pCustomer temp;
		cus->next=NULL;
		sem_wait(&s[0].area);

		if(s[0].q.end==NULL)
		{
			s[0].q.head=cus;
			s[0].q.end=cus;
		}
		else{
			temp=s[0].q.end;
			temp->next=cus;
			s[0].q.end=cus;
		}
		sem_post(&s[0].area);
		sem_post(&s[0].empty);

		usleep(1000*cus->sort_time);

		i++;

	}
	fprintf(out,".\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t.\t\n");
	fprintf(out,"Last Customer is Done\n");
//-------------------------------------------------------------------------

fprintf(out,"Waiting for threads\n");
while(count_Finish<clin);
fprintf(out,"Last customer thread done\n");
gettimeofday(&finish,NULL);
avg=0;
tot=((finish.tv_sec)-(begin.tv_sec))*1000+(finish.tv_usec-begin.tv_usec)/1000;
fprintf(out,"The total time of the workday time: %d\n",tot);

for(i=0;i<clerknum;i++)
{
	clercks[i].wait_time=tot-clercks[i].work_time;
	avg+=clercks[i].wait_time;
}
int avg2;
k=0;
for(i=0;i<cntPlace;i++)
{
	avg2=0;
	for(j=0;j<s[i].state;j++)
	{
		fprintf(out,"%d %s %d %d\n",clercks[k].id,s[i].name,clercks[k].work_time,clercks[k].wait_time);
		avg2+=clercks[k].work_time;
		k++;
		}
	fprintf(out,"Avg work time in %s = %d\n",s[i].name,avg2/s[i].state);

}
fprintf(out,"Number of costumers that finished = %d \n",count_Finish);
fprintf(out,"Avg wait time for clerk = %d ",avg/clerknum);
fprintf(out,"Avg costumer wait time = %d \n",wait/count_Finish);
fclose(in);
fclose(out);
return 1;


}

void* workMod(void* ssn){

	pClerk cle=(Clerk*)ssn;
	pCustomer cu;
	pCustomer tmp;


	while(1){
		sem_wait(&s[0].empty);
		sem_wait(&s[0].area);
		cu=s[0].q.head;
		s[0].q.head=s[0].q.head->next;
		if(s[0].q.head==NULL)
			s[0].q.end=NULL;
		sem_post(&s[0].area);
		usleep(1000*cu->sort_time);
		cle->work_time+=cu->sort_time;
		sem_wait(&s[cu->service].area);
		cu->next=NULL;

		if(s[cu->service].q.end==NULL)
				{
			s[cu->service].q.head=cu;
			s[cu->service].q.end=cu;
				}
				else{
					tmp=s[cu->service].q.end;
					tmp->next=cu;
					s[cu->service].q.end=cu;
				}

		sem_post(&s[cu->service].area);
		sem_post(&s[cu->service].empty);


	}



}
void* workElse(void* ssn)
{

	pClerk cle=(Clerk*)ssn;
	pCustomer cu;
	int ser=cle->service;
	pCustomer tmp;
	while(1){
	sem_wait(&s[ser].empty);
	sem_wait(&s[ser].area);

	cu=s[ser].q.head;
	s[ser].q.head=cu->next;
	if(s[ser].q.head==NULL)
					s[ser].q.end=NULL;

	if(s[ser].q.end==NULL)
					{
				s[ser].q.head=cu;
				s[ser].q.end=cu;
					}
					else{
						tmp=s[ser].q.end;
						tmp->next=cu;
						s[ser].q.end=cu;
					}
	sem_post(&s[ser].area);
	usleep(1000*cu->service_time);
	gettimeofday(&cu->exit_time,NULL);
	cu->total_time=(cu->exit_time.tv_sec-cu->enter_time.tv_sec)*1000+(cu->exit_time.tv_usec-cu->enter_time.tv_usec)/1000;
	cu->wait_time=cu->total_time-cu->sort_time-cu->service_time;
	cle->work_time+=cu->service_time;
	sem_wait(&counter);
	wait +=cu->wait_time;
	cu->next=NULL;
	if(i_finished->q.end==NULL)
			{
				i_finished->q.head=cu;
				i_finished->q.end=cu;
			}
			else{
				tmp=i_finished->q.end;
				tmp->next=cu;
				i_finished->q.end=cu;
			}

	count_Finish++;
	sem_post(&counter);
	}
}
