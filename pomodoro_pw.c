/*
author: plapacz6@gmail.com 
data: 2022-04-14 
license: GNU General Public License v3.0

description: elementary cli pomodoro timer
*/

#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

typedef enum type_of_period_tt {
    TP_WORK,
    TP_SHORT_PAUSE,
    TP_LONG_PAUSE,
    TP_END_OF_CYCLE,
} type_of_period_t;

typedef struct time_period_table_my_tt {
  type_of_period_t curr_period_type;
  unsigned curr_period_time;
  unsigned work_nr;
} time_period_table_my_t;

typedef struct time_period_table_cfg_tt {
  unsigned work_time;
  unsigned work_nr;
  unsigned short_pause_time;
  unsigned long_pause_time;
} time_period_table_cfg_t;

#define DEFAULT_WORK_TIME (25)
#define DEFAULT_SHORT_TIME (5)
#define DEFAULT_LONG_TIME (15)
#define DEFAULT_WORK_NR (4)

int main(int argc, char **argv){  

  time_period_table_my_t period_table_my;
  time_period_table_cfg_t period_table_cfg;
  bool end_of_pomodoro = false;
  char odp = 't';
  unsigned long arg = 0;
  char *cli_help_switch = "--help";
  //time_t time_stamp1;
  
  if(argc > 1){
    if( ! strncmp(argv[1], cli_help_switch, strlen(cli_help_switch))){
      printf( 
        "\n"
        "usage: %s   work_time   short_break_time  long_break_time  number"
        "\n\n", 
        argv[0]);
      return 0;
    }
    arg = atoi(argv[1]);
    if(!( arg  > 0 && arg < 120 ))
      arg = DEFAULT_WORK_TIME;          
    period_table_cfg.work_time = (unsigned)arg; 
  }
  else
    period_table_cfg.work_time = DEFAULT_WORK_TIME; 

  if(argc > 2){
    arg = atoi(argv[2]);
    if(!( arg > 0 && arg < 120 ))
      arg = DEFAULT_SHORT_TIME;
    period_table_cfg.short_pause_time = (unsigned)arg; 
  }
  else
    period_table_cfg.short_pause_time = DEFAULT_SHORT_TIME; 



  if(argc > 3){
    arg = atoi(argv[3]);
    if(!( arg > 0 && arg < 120 ))      
      arg = DEFAULT_LONG_TIME;
    period_table_cfg.long_pause_time = (unsigned)arg;
  }
  else
    period_table_cfg.long_pause_time = DEFAULT_LONG_TIME;


  if(argc > 4){
    arg = atoi(argv[4]);
    if(!( arg > 0 && arg < 10 ))
      arg = DEFAULT_WORK_NR;    
    period_table_cfg.work_nr = (unsigned)arg;
  }
  else
    period_table_cfg.work_nr = DEFAULT_WORK_NR;


  printf(
  "work time:        %u min\n"
  "short pause time: %u min\n"
  "long pause time:  %u min\n"
  "cycles number:    %u times"
  "\n\n",
    period_table_cfg.work_time,
    period_table_cfg.short_pause_time,
    period_table_cfg.long_pause_time,
    period_table_cfg.work_nr
  );

  do { //main loop
  
    period_table_my.work_nr = 0;
    period_table_my.curr_period_type = TP_SHORT_PAUSE;

    while(period_table_my.curr_period_type != TP_END_OF_CYCLE){

      /*
      if(        
        period_table_my.curr_period_time < 
        (unsigned)( difftime(time(NULL), time_stamp1) / 60 )
      ){
      */

        switch(period_table_my.curr_period_type){
          case TP_WORK:
            if(period_table_my.work_nr >= period_table_cfg.work_nr){
              period_table_my.curr_period_time = 
                period_table_cfg.long_pause_time;
              period_table_my.curr_period_type = TP_LONG_PAUSE;
              printf("%s\n",
                " ____ Take a LONG BREAK ____ \a\a\a");
              sleep(period_table_cfg.long_pause_time * 60);
            }
            else{
              period_table_my.curr_period_time = 
                period_table_cfg.short_pause_time;
              period_table_my.curr_period_type = TP_SHORT_PAUSE;
              printf("%s\n",
                " ---- Take a short break ----\a");
              sleep(period_table_cfg.short_pause_time * 60);
            }
            break;
            
          case TP_SHORT_PAUSE:
            period_table_my.curr_period_time = 
              period_table_cfg.work_time;
            period_table_my.curr_period_type = TP_WORK;
            period_table_my.work_nr++;
            printf("%s%d\n",
              " ****  TIME TO WORK  ***** ", 
              period_table_my.work_nr);
            sleep(period_table_cfg.work_time * 60);
            break;

          case TP_LONG_PAUSE:
            period_table_my.curr_period_type = TP_END_OF_CYCLE;;
            break;
          case TP_END_OF_CYCLE:
            printf("%s\n", "program logic error");
            break;
        }//switch
    
      /*
        time_stamp1 = time(NULL);
      }//if 
      */
    
    }//while  
    printf("%s\n","Do you want to continue ? (t/n) [ENTER]");
    
    do{      
      odp = getc(stdin);
    } while( odp != 't' && odp != 'T' && odp != 'n' && odp != 'N');
    
    if(odp == 'n' || odp == 'N'){ 
      end_of_pomodoro = true;
    }

  } while(!end_of_pomodoro); //main loop

  return 0;
}
