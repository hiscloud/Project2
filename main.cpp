// Ref: https://stackoverflow.com/questions/6119664/how-to-use-pthread-mutex-and-its-functions-within-a-class
#include <iostream>
#include <time.h>
#include "windows.h"  // for sleep - windows
//#include <unistd.h> // for sleep - unix/linux/mac
using namespace std;
#include <pthread.h>
#include <stdio.h>
#include <cstdlib> // exit :: may be needed on CodeBlocks;
#include<random>
pthread_mutex_t toon_signal_mutex;
struct MUTEX_SHARED{
int COUNTING;
int AGENT;
};


class piece{
	public:
	piece(){};
	~piece(){};
	bool carrot; //is a carrot
	bool mt;
	bool mv;
	bool carry; // carry a carrot
	int col;
	int row;
	string name;
	int ID;
	
	void init(string n){
		srand( (unsigned)time( NULL ) );
		col=rand()%5;
		row=rand()%5;
		name=n;
		carry =false;
	}
	
	
	
};

class table{
	public:
	table(){};
	~table(){};	
	string matrix[5][5];
	//class piece *pieceOBJ=NULL;
	piece arr[7];//lowest number is 7
	int arrIND=0;
	void init(){
		srand( (unsigned)time( NULL ) );
		for (int i=0;i<5;i++){
			for (int j=0;j<5;j++){
				matrix[i][j]="-   ";
			}	
		}
		
		piece marvin,bunny,tweety,devil,c1,c2,mount;
		bunny.init("B   ");
		tweety.init("T   ");
		c1.init("C   ");
		c1.carrot=true;
		c2.init("C   ");
		c2.carrot=true;
		devil.init("D   ");
		marvin.init("M   ");
		marvin.mv=true;
		mount.init("F   ");
		mount.mt=true;
		addPiece(devil);	
		addPiece(bunny);
		addPiece(tweety);
		addPiece(marvin);
		
		addPiece(mount);
		addPiece(c1);
		addPiece(c2);
		
			
	}

	void addPiece(piece p){  
		while(checkOn(p.col,p.row)){
			p.col=rand()%5;
			p.row=rand()%5;	
				
		}
		matrix[p.col][p.row]=p.name;
		
		arr[arrIND]=p;
		p.ID=arrIND;
	
		arrIND++;
		//p.carry=false;
		
	}	
	
	void printTable(){
		for (int i=0;i<5;i++){
			for(int j=0;j<5;j++){
			cout<<matrix[i][j];
		} 
		cout<<endl;
		}
	}	
	
	
	
	
	//random a point which is around the piece
	void move(int ind){
		piece p=arr[ind];
		
		//cout<<p.name<<p.col<<p.row<<endl;/////////////////////////////////
		matrix[p.col][p.row]="-   ";
		int x=rand()%5,y=rand()%5;//(x,y) next place to step
		while((checkOn(x,y)&&!getPiece(x,y).carrot)||(p.col-x>1||p.col-x<-1)||(p.row-y>1||p.row-y<-1)){//(then it should change)
			x=rand()%5;
			y=rand()%5;	
		}
		p.col=x;
		p.row=y;
		//cout<<x<<y<<endl;/////////////////////////////
		if(getPiece(x,y).carrot){//carrot condition
			if (p.ID==0)
				p.name="D(C)";	
			if(p.ID==1)
				p.name="B(C)";
			if(p.ID ==2)
				p.name="T(C)";
			if(p.ID==3)
				p.name="M(C)";
			p.carry=true;
		}
		if(getPiece(x,y).mt&&p.carrot){ //winning condition
			cout<< p.name<<" wins"<<endl;	
		}
		matrix[p.col][p.row]=p.name;
		arr[ind].col=p.col;
		arr[ind].row=p.row;
		//setPiece();
	}
	
	//check if the place has a piece
	bool checkOn(int x1, int y1){
		
		for(int i=0;i<arrIND;i++){
			if(x1==arr[i].col &&y1==arr[i].row){
				return true;
			}
		}
		return false;
	}
	
	void setPiece(){/////////////////////////////////////////////////////////////////
		for(int i=0;i<arrIND;i++){
			matrix[arr[i].col][arr[i].row]=arr[i].name;
		}
		
	}
	
	piece getPiece(int x, int y){
		piece pNull;	
			for(int i=0;i<arrIND;i++){
			if(x==arr[i].col &&y==arr[i].row)
				return arr[arrIND];
		}
		return pNull;
		
	}
};

class CParallel
{      public:
		CParallel(){};
		~CParallel(){};
		table *t;
		
		int thread_id_obj;
		pthread_t thread_id;
		pthread_mutex_t *toon_signal_mutex = NULL;
		MUTEX_SHARED *SHARED=NULL;
		int stop;

		void init(int value, MUTEX_SHARED *temp_SHARED,
            pthread_mutex_t *temp_toon_signal_mutex,table *T){
		    thread_id_obj = value;
		    SHARED = (MUTEX_SHARED*)temp_SHARED;
		    toon_signal_mutex = (pthread_mutex_t*)temp_toon_signal_mutex;
		    stop = 0;
			t=T;
			
			
			
		}

		static void *parallel_API(void *context)
		{	((CParallel *)context)->run_process();
		}
		
		

    private:
        void *run_process(void){
            while(!stop){
                toon_signal();
                cout << "thread_id_obj = " << thread_id_obj << endl;
                
            }
            pthread_exit(NULL);
			return 0;
		}
		

		
        void toon_signal(void)
		{   pthread_mutex_lock(toon_signal_mutex);
                if(SHARED->COUNTING<2 && thread_id_obj == 0){
                    // RUN 1st THREAD
                    SHARED->AGENT = thread_id_obj;
                    cout << "SHARED->COUNTING = " << SHARED->COUNTING
                            << " SHARED->AGENT = " << SHARED->AGENT << endl;
                    SHARED->COUNTING++;
					t->move(0);
                    if(SHARED->COUNTING==2){
                        stop=1;
                    }
                }else if(SHARED->COUNTING<4 && thread_id_obj == 1){
                    // RUN 2nd THREAD
                    SHARED->AGENT = thread_id_obj;
                    cout << "SHARED->COUNTING = " << SHARED->COUNTING
                            << " SHARED->AGENT = " << SHARED->AGENT << endl;
                    SHARED->COUNTING++;
					t->move(1);
                    if(SHARED->COUNTING==4){
                        stop=1;
                    }
				}else if(SHARED->COUNTING<6 && thread_id_obj == 2){
					
					 // RUN 3rd THREAD
                    SHARED->AGENT = thread_id_obj;
                    cout << "SHARED->COUNTING = " << SHARED->COUNTING
                            << " SHARED->AGENT = " << SHARED->AGENT << endl;
                    SHARED->COUNTING++;
					t->move(2);
                    if(SHARED->COUNTING==6){
                        stop=1;
                    }
				}else if(SHARED->COUNTING<8 && thread_id_obj == 3){
					
					 // RUN 4th THREAD
                    SHARED->AGENT = thread_id_obj;
					t->move(3);
                    cout << "SHARED->COUNTING = " << SHARED->COUNTING
                            << " SHARED->AGENT = " << SHARED->AGENT << endl;
                    SHARED->COUNTING++;
                    if(SHARED->COUNTING==8){
                        stop=1;
                    }
				}else{
					
					 // RUN 5th THREAD
                    SHARED->AGENT = thread_id_obj;
                    cout << "SHARED->COUNTING = " << SHARED->COUNTING
                            << " SHARED->AGENT = " << SHARED->AGENT << endl;
                    SHARED->COUNTING++;
                    if(SHARED->COUNTING==10){
                        stop=1;
                    }
				}
            pthread_mutex_unlock(toon_signal_mutex);
        }
		
		
};

int main (int argc, char *argv[])
{  

  	
int total_threads = 5;
    CParallel obj[5];
    MUTEX_SHARED SHARED;
    SHARED.COUNTING=0;

/////////////////////////////////////////////////////////////////////////////////////////

	table *myTable=new table();
	myTable->init();	
	myTable->printTable();
	myTable->move(0);
	myTable->move(1);// duplicated
	myTable->move(2);
	myTable->move(3);
	
	
	
	
		
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//threads
    pthread_mutex_init(&toon_signal_mutex, NULL);

    for(int i=0;i<total_threads;i++){
        obj[i].init(i,&SHARED,&toon_signal_mutex,myTable);
        pthread_create(&obj[i].thread_id, NULL,
                       &CParallel::parallel_API, &obj[i]);
        
        Sleep(1); // for windows
        pthread_join(obj[i].thread_id, NULL);
		myTable->printTable();
    }

    pthread_mutex_destroy(&toon_signal_mutex);
	
    
    cout << "program finished!"<< endl;
myTable->printTable();
    return 0;
}
