#include "stdio.h"
#include <stdlib.h>
#include <time.h>
#define getpch(type) (type*)malloc(sizeof(type))
typedef struct pcb PCB;
struct pcb {
    int id;
    char name[10];
    int time_start;
    int time_need;
    int time_left;
    int time_used;
    char state;
};


/** system functions **/
void _sleep( int n){
    clock_t goal;
    goal = (clock_t)n * CLOCKS_PER_SEC + clock();
    while( goal > clock() ){
        //pass
    };
}

char _keygo( ){
    char c;
    printf("Press any key to continue...\n");
    scanf( "%c", &c );
    return c;
}




/** user functions **/
//----- Data settings -----
int time_unit = 2;
#define MAXNUM 10
int num = 5;
PCB pcbdata[MAXNUM] = {
    { 1000, "A", 0, 4, 4, 0, 'R' },
    { 1001, "B", 1, 3, 3, 0, 'R' },
    { 1002, "C", 2, 5, 5, 0, 'R' },
    { 1003, "D", 3, 2, 2, 0, 'R' },
    { 1004, "E", 4, 4, 4, 0, 'R' },
};

int ready[MAXNUM];
int order[MAXNUM];

//void input(){} //input data manualy

//debuging functions
void display_ready(  int runned ){
    int i;
    for( i = 0; i < runned; i++ ){
        printf( "%s ", pcbdata[ready[i]].name );
    }
    printf( "\n" );
}

void FCFS(){
    int i, j, temp;
    double k;
    PCB currentProcess;
    for( i = 0; i < num; i++){
        order[i] = pcbdata[i].time_start;
        ready[i] = i;
    }

    for( i = 0; i < num; i++){
        for( j = i + 1; j< num; j++ ){
            if( order[i] > order[j] ){
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;

                temp = ready[i];
                ready[i] = ready[j];
                ready[j] = temp;
            }
        }
    }

    printf("-----FCFS algorithm: No cut in, no time slice.-----\n");
    temp = pcbdata[ready[0]].time_start;
    //pritnf( "%9s%4s%10%4s" );
    printf( "|-----------------------------------------------------------------\n" );
    printf( "| Num |Name | Arv.T | Srv.T | Run  | Fin  | Fin.T | Zz.T | Zz.wT |\n" );
    printf( "|----------------------------------------------------------------|\n" );
    for( i = 0; i < num; i++){
        currentProcess = pcbdata[ready[i]];
        printf( "| %3d |  %s  |   %d   |   %d   | tick |", i+1, currentProcess.name, currentProcess.time_start, currentProcess.time_need );
        //printf( "%dth process -- %s\n", i+1, currentProcess.name );
        //printf( "Arrived at: %d, served: %d secs. \n", currentProcess.time_start, currentProcess.time_need );
        //printf( "Current process is running........" );
        _sleep( 1 );
        printf( " tick |" );
        //printf( "Finish running.\n" );

        temp += currentProcess.time_need;
        j = temp - currentProcess.time_start;
        k = (float)j/currentProcess.time_need;
        printf( "  %3d  |  %2d  |  %.1f  |\n", temp, j, k );
        //printf( "Finish time: %d, zhouzhuan time: %d, zz time with weight: %.1f\n", temp, j, k );
    }
    printf( "|----------------------------------------------------------------|\n" );
    printf( "----- All process has executed -----\n" );
}

void SJF(){
    int i, j, temp;
    double k;
    int timePassed = 0, running = 0;
    PCB currentProcess;
    int readyProcsNum = 0, runnedProcessNum = 0;

    printf( "-----SJF algorithm: No cut in, no time slice.-----\n" );
    printf( "|----------------------------------------------------|\n" );
    printf( "| Num |Name | Arv.T | Srv.T | Fin.T | Cyc.T | Cyc.wT |\n" );
    printf( "|----------------------------------------------------|\n" );

    while( num-runnedProcessNum ){
        //Find the process start at timePassed
        if( readyProcsNum < num ){
            for( i = runnedProcessNum + running ; i < num; i++ ){
                if( pcbdata[i].time_start == timePassed ){
                    ready[ readyProcsNum ] = i;
                    readyProcsNum++;
                }
            }
        }

        //sort those process which are in ready queue, by the time they need to use processor
        for( i = 0; i < readyProcsNum; i++ ){
            for( j = i + 1; j < readyProcsNum; j++ ){
                if( pcbdata[ ready[ i ] ].time_need > pcbdata[ ready[ j ] ].time_need  ){
                    temp = ready[i];
                    ready[i] = ready[j];
                    ready[j] = temp;
                }
            }
        }

        //display_ready( readyProcsNum );

        if( running == 1 && currentProcess.time_used == currentProcess.time_need ){
            runnedProcessNum++;
            running = 0;
            printf(
                    "| %3d |  %s  |   %d   |   %d   |",
                    runnedProcessNum,
                    currentProcess.name,
                    currentProcess.time_start,
                    currentProcess.time_need
                    );

            //_sleep( 1 );

            j = timePassed - currentProcess.time_start;
            k = (float)j/currentProcess.time_need;
            printf( "  %3d  |   %2d  |   %.1f  |\n", timePassed, j, k );
        }

        // If there's no process running,
        // pick first one from the ready queue
        if( running == 0 && num-runnedProcessNum != 0){
            currentProcess = pcbdata[ ready[ 0 ] ];

            readyProcsNum--;
            for( i = 0; i < readyProcsNum; i++ ){
                ready[ i ] = ready[ i+1 ];
            }

            running = 1;
        }

        //run
        timePassed++;
        currentProcess.time_used++;
    }


    printf( "|----------------------------------------------------|\n" );
}

void HRF(){    int i, j, temp;
    double k;
    int timePassed = 0, running = 0;
    PCB currentProcess;
    int readyProcsNum = 0, runnedProcessNum = 0;
    double reflect[num];

    printf( "-----HRF algorithm: No cut in, no time slice.-----\n" );
    printf( "|----------------------------------------------------|\n" );
    printf( "| Num |Name | Arv.T | Srv.T | Fin.T | Cyc.T | Cyc.wT |\n" );
    printf( "|----------------------------------------------------|\n" );

    while( num-runnedProcessNum ){
        //Find the process start at timePassed
        if( readyProcsNum < num ){
            for( i = runnedProcessNum + running ; i < num; i++ ){
                if( pcbdata[i].time_start == timePassed ){
                    ready[ readyProcsNum ] = i;
                    readyProcsNum++;
                }
            }
        }

        //sort those process which are in ready queue, by the time they need to use processor
        for( i = 0; i < readyProcsNum; i++ ){
            reflect[ i ] = ((timePassed - pcbdata[ready[i]].time_start )*1.0/ pcbdata[ready[i]].time_need )+1;
            for( j = i + 1; j < readyProcsNum; j++ ){
                reflect[ j ] = ((timePassed - pcbdata[ready[j]].time_start )*1.0/ pcbdata[ready[j]].time_need )+1;
                if( reflect[ i ] < reflect[ j ]  ){
                    temp = ready[i];
                    ready[i] = ready[j];
                    ready[j] = temp;
                }
            }
        }

        //display_ready( readyProcsNum );

        if( running == 1 && currentProcess.time_used == currentProcess.time_need ){
            runnedProcessNum++;
            running = 0;
            printf(
                    "| %3d |  %s  |   %d   |   %d   |",
                    runnedProcessNum,
                    currentProcess.name,
                    currentProcess.time_start,
                    currentProcess.time_need
                    );

            //_sleep( 1 );

            j = timePassed - currentProcess.time_start;
            k = (float)j/currentProcess.time_need;
            printf( "  %3d  |   %2d  |   %.1f  |\n", timePassed, j, k );
        }

        // If there's no process running,
        // pick first one from the ready queue
        if( running == 0 && num-runnedProcessNum != 0){
            currentProcess = pcbdata[ ready[ 0 ] ];

            readyProcsNum--;
            for( i = 0; i < readyProcsNum; i++ ){
                ready[ i ] = ready[ i+1 ];
            }

            running = 1;
        }

        //run
        timePassed++;
        currentProcess.time_used++;
    }


    printf( "|----------------------------------------------------|\n" );
    printf( "Hight Reflection First Algorithm, unfinished." );
}

void Timeslice(){
    printf( "Time slice Algorithm, unfinished." );
}

void MRLA(){
    printf( "Multi Reflection xxxx queue Algorithm, unfinished." );
}


int main(){
    int sch = 99;
    //input();
    while( sch != 0 ){
        printf( "\nPlease select an algorithm:\n" );
        printf( "(1)FCFS: First come first serve\n" );
        printf( "(2)SJF : Short job first serve\n" );
        printf( "(3)HRF : High reflection first serve\n" );
        printf( "(4)Timeslice: Time slice serve\n" );
        printf( "(5)MRLA: Multi relation xxxx queue serve\n" );
        printf( "(0)Quit\n" );
        scanf( "%d", &sch );

        switch( sch ){
            case 1:
                FCFS();
                break;
            case 2:
                SJF();
                break;
            case 3:
                HRF();
                break;
            case 4:
                Timeslice();
                break;
            case 5:
                MRLA();
                break;
            case 0:
                printf( "Quited program\n" );
                break;
        }
    }
    _keygo();
    return 0;
}






