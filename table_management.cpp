#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;
//to build the structure table
struct table{
  int tablenum;
  int tsize;
  int numppl;
  bool occupied;
  string otime;
  int omin;
  string overtime;
};

//to change the position of variable in array for futher program running
void move_list(int list[],int j){
  for(int i=j;i<999;i++){
    list[i]=list[i+1];
  }
  list[999]=0;
}

/*to store the Sit-in time of the table when there are customers sit in.
the input data is the table number and the struct of table.
*/
//All the time of the Sit-in time is according to the clock on the local computer
void timechange(int select, struct table tabledis[])
{
    string s_startime,stime;
    auto startime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    s_startime=ctime(&startime);
    stime=s_startime.substr(11,5);
    tabledis[select].otime=stime;
}

/*To calculate and compare the difference between the Sit-in time
and the real time (in minutes).
  In real situation, if the difference in minutes is higher than 60mins,
the group of customeers is defined to be overtime.
  It will notice the staff in the deli and inform the customers.
  This can prevent somer customers seat in the deli for a long time and the
customers behind them will not have a table at a long time.
  For experiment condition, to make the result of overtime more easy to be
discovered, the highest minutes of customers to sit in the table
will be 3 mins as shown bellow "if(difference<=3)". */
void timecompare(string a, struct table tabledis[], int select){
  string stime = a.substr(0,2)+a.substr(3,2);
  int time_i=stoi(stime);
  auto nowtime = chrono::system_clock::to_time_t(chrono::system_clock::now());
  string s_nowtime=ctime(&nowtime);
  string str_nowtime=s_nowtime.substr(11,2)+s_nowtime.substr(14,2);
  int nowtime_i=stoi(str_nowtime);
  int difference=nowtime_i-time_i;
  if(difference<=3){
    tabledis[select].overtime="No";
  }
  else{
    tabledis[select].overtime="Yes";
  }
  tabledis[select].omin=difference;
}

/*This function is reading the total number of table in order to set the
number of different types of table.
  Then an initial template of all of the condition of table will be set. */
void readtable(int number_of_table,struct table tabledis[]){
  string line;
  int counter=0,size,number,sum=0;
  while(number_of_table>counter){
    cout<<"Please input the size of table and its amount:";
    cin>>size>>number;
    sum+=number;
    if(sum>number_of_table){
      cout<<"ERROR: The input number of table exceeds the original total number of table"<<endl;
      sum-=number;
      continue;
    }
    for(int i=0;i<number;i++){
      tabledis[counter]={counter,size,0,false,"00:00",0,"No"};
      counter+=1;
    }
  }
}

void pending(int size_of_group,int pending_list[]){
  int i=0;
  while(pending_list[i]!=0){
    ++i;
  }
  pending_list[i]=size_of_group;
}

// this function is initiating the template of waiting list of customers
void pending_checking(int pending_list[],struct table tabledis[],int totaltable,int max,int max2){
  int i=0,count=0,min,difference,selected,count2,count3;
  min=max;
  //to see are there any customer size among the first 5 pending customers larger than then second largest table size
  for(int z=0;z<5;z++){
    if(pending_list[z]>max2){
      count=1;
    }
  }
  //there are customer size larger than then second largest table size
  if(count==1){
    while(pending_list[i]!=0){
      count2=0;
      count3=0;
      for(int j=0;j<totaltable;j++){
          if(tabledis[j].occupied){
            continue;
          }
          else{
            count2+=1;
            if(tabledis[j].tsize>=pending_list[i]){
              difference=tabledis[j].tsize-pending_list[i];
              count3+=1;
              if(difference<min){
                selected=j;
              }
            }
          }
      }
      // all tables are full
      if(count2==0){
        min=max;
        for(int j=0;j<totaltable;j++){
	/*if there are customer size among the first 5 pending customers larger than then second largest table size, the largest table will not allow different groups of customers to seat togerther*/
          if(tabledis[j].tsize==max&pending_list[1]!=0){
            if(tabledis[j].occupied){
              continue;
            }
          }
          /*to see are there any table that have seats to allow another group of customers to seat*/
          if((tabledis[j].tsize-tabledis[j].numppl)>=pending_list[i]){
            difference=(tabledis[j].tsize-tabledis[j].numppl)-pending_list[i];
            cout<<difference<<" "<<j<<endl;
            count3+=1;
            if(difference<min){
              selected=j;
            }
          }
        }
      }
      // no table can fit in
      if(count3==0){
        i++;
        continue;
      }
//there are table that can let customers to sit in
      if(count3!=0){
        tabledis[selected].occupied=true;
        tabledis[selected].numppl+=pending_list[i];
        move_list(pending_list,i);
        cout<<"\n"<<"Recommend table: Table "<<tabledis[selected].tablenum<<"people in"<<pending_list[i]<<endl;
        timechange(selected,tabledis);
        //timecompare(tabledis[selected].otime,tabledis,selected);
      }
    }
  }
 //there aren’t customer size larger than then second largest table size
  else{
    while(pending_list[i]!=0){
      count2=0;
      count3=0;
      for(int j=0;j<totaltable;j++){
        if(tabledis[j].occupied){
          continue;
        }
        else{
          count2+=1;
          if(tabledis[j].tsize>=pending_list[i]){
            difference=tabledis[j].tsize-pending_list[i];
            count3+=1;
            if(difference<min){
              selected=j;
            }
          }
        }
      }
//all table are full
      if(count2==0){
        cout<<"\n"<<"all full"<<endl;
        min=max;
        for(int j=0;j<totaltable;j++){
          if((tabledis[j].tsize-tabledis[j].numppl)>=pending_list[i]){
            difference=(tabledis[j].tsize-tabledis[j].numppl)-pending_list[i];
            count3+=1;
            if(difference<min){
              selected=j;
            }
          }
        }
      }
// no table can fit in
      if(count3==0){
        i++;
        continue;
      }
//there are table that can let customers to sit in
      if(count3!=0){
        tabledis[selected].occupied=true;
        tabledis[selected].numppl+=pending_list[i];
        move_list(pending_list,i);
        cout<<endl;
        cout<<"Recommend table: Table "<<tabledis[selected].tablenum<<endl;
        timechange(selected,tabledis);
      }
    }
  }
}

/*To find the biggest and the second biggest size of table*/
void finding_max(int totaltable,struct table tabledis[],int &max,int &max2){
  for(int j=0;j<totaltable;j++){
    if(tabledis[j].tsize>max){
      max2=max;
      max=tabledis[j].tsize;
    }
    else if(tabledis[j].tsize>max2&tabledis[j].tsize<max){
      max2=tabledis[j].tsize;
    }
  }
}

/* to  arrange cusotmers to the table which is most suitable for them. (i.e. the difference
Between the table size and the number of customer in the group is the smallest.
  Also to when the table is fulled, this function will move the group of customers to a waiting list
By connecting to the function pending() and pending_checking()*/
void sitin(int n,int totaltable, struct table tabledis[],int pendinglist[],int max,int max2){
  int i;
  int min=100,selected,temp,count=0,value;
  value=pendinglist[0];
  if(value==0){
    for(i=0;i<totaltable;i++){
      if(tabledis[i].occupied){
        continue;
      }
      else{
        if(tabledis[i].tsize>=n){
          temp=tabledis[i].tsize-n;
          if(temp<min){
            min=temp;

            selected=i;
          }
        }
        else{
          ++count;
          continue;
        }
        ++count;
      }
    }
/*here is the case when all tables are occupied or the size of remaining table is
 Not big enough for the customers*/
    if(count==totaltable){
      for(i=0;i<totaltable;i++){
        if(tabledis[i].tsize-tabledis[i].numppl>=n){
          temp=tabledis[i].tsize-tabledis[i].numppl-n;
          if(temp<min){
            min=temp;
            selected=i;
            cout<<endl;
            cout<<"Recommend table: Table "<<tabledis[selected].tablenum<<endl;
            timechange(selected,tabledis);
          }
        }
      }
    }
/*the table will become occupied if there is customers sit in. Also the number of people
 On the table will be stored*/
    if(min!=100){
      tabledis[selected].occupied=true;
      tabledis[selected].numppl=n;
      cout<<endl;
      cout<<"Recommend table: Table "<<tabledis[selected].tablenum<<endl;
      timechange(selected,tabledis);
    }
//putting the customers to pending list
    if(min==100){
      pending(n,pendinglist);
      pending_checking(pendinglist,tabledis,totaltable,max,max2);
      cout<<endl;
      cout<<"Top 10 waiting list:";
      for(int i=0;i<10;i++){
        cout<<pendinglist[i]<<" ";
      }
      cout<<endl;
    }
  }
  else{
    pending(n,pendinglist);
    pending_checking(pendinglist,tabledis,totaltable,max,max2);
  }
  cout<<endl;
  cout<<setw(4)<<"No."<<setw(5)<<"Size"<<setw(7)<<"Numppl"<<setw(9)<<"Occupied"
   <<setw(13)<<"Sit-in time"<<setw(9)<<" Meal time(min)"<<setw(10)<<"Overtime"<<endl;
  for(int i=0;i<totaltable;i++){
    cout<<setw(4)<<tabledis[i].tablenum<<setw(5)<<tabledis[i].tsize<<setw(7)<<tabledis[i].numppl<<
     setw(9)<<tabledis[i].occupied<<setw(13)<<tabledis[i].otime<<setw(9)<<tabledis[i].omin<<setw(10)<<tabledis[i].overtime<<endl;
  }
  cout<<"Top 10 waiting list:";
  for(int i=0;i<10;i++){
    cout<<pendinglist[i]<<" ";
  }
  cout<<endl;
}

int main(){
  int totaltable,n,i,j, pendingnum=0,max=0,max2=0;
  int pendinglist[1000];
  for(i=0;i<1000;i++){
    pendinglist[i]=0;
  }
  //input the total number of table
  cout<<"Please input the total number of tables:";
  cin>>totaltable;
  //creating dynamic array
  table* array = new table [totaltable];
  readtable(totaltable,array);
  cout<<endl;
  //show the distribution
  cout<<setw(4)<<"No."<<setw(5)<<"Size"<<setw(7)<<"Numppl"<<setw(9)<<"Occupied"<<setw(13)<<
   "Sit-in time"<<setw(9)<<" Meal time(min)"<<setw(10)<<"Overtime"<<endl;
  for(int i=0;i<totaltable;i++){
    cout<<setw(4)<<array[i].tablenum<<setw(5)<<array[i].tsize<<setw(7)<<array[i].numppl<<
     setw(9)<<array[i].occupied<<setw(13)<<array[i].otime<<setw(9)<<array[i].omin<<setw(10)<<array[i].overtime<<endl;
  }
  //finding the maximum and the second maximun size of table
  finding_max(totaltable,array,max,max2);
  cout<<endl;
  cout<<"Please insert the number of people: "<<"\n"<<"(People leave their table --- insert -1 then insert table number)"
   <<"\n"<<"(Exit the programm "<<setw(20)<<" --- insert 0)"<<"\n"<<"(Change the ditribution of table "<<setw(10)<<" --- insert -2)"<<endl;
  cin>>n;
  while(n!=0){
    //customers moving out
    if(n==-1){
      //input the moving out table number
      cout<<"Please input a table number:";
      cin>>j;
      //error if no such table
      if(j>(totaltable-1)){
        cout<<"ERROR: No such table!"<<endl;
      }
      else{
        //reseting the data of that table
        array[j].otime="00:00";
        array[j].omin=0;
        array[j].occupied=false;
        array[j].numppl=0;
        array[j].overtime="No";
        cout<<"Customers moved out"<<"\n"<<endl;
        cout<<setw(4)<<"No."<<setw(5)<<"Size"<<setw(7)<<"Numppl"<<setw(9)<<"Occupied"
         <<setw(13)<<"Sit-in time"<<setw(9)<<" Meal time(min)"<<setw(10)<<"Overtime"<<endl;
        for(int i=0;i<totaltable;i++){
          cout<<setw(4)<<array[i].tablenum<<setw(5)<<array[i].tsize<<setw(7)<<array[i].numppl
           <<setw(9)<<array[i].occupied<<setw(13)<<array[i].otime<<setw(9)<<array[i].omin<<setw(10)<<array[i].overtime<<endl;
        }
        cout<<endl;
        //showing the waiting customers
        cout<<"Top 10 waiting list:";
        for(int i=0;i<10;i++){
          cout<<pendinglist[i]<<" ";
        }
        cout<<endl;
        //let the pending customers to move in
        pending_checking(pendinglist,array,totaltable,max,max2);
        cout<<endl;
        cout<<setw(4)<<"No."<<setw(5)<<"Size"<<setw(7)<<"Numppl"<<setw(9)<<"Occupied"
         <<setw(13)<<"Sit-in time"<<setw(9)<<" Meal time(min)"<<setw(10)<<"Overtime"<<endl;

        for(int i=0;i<totaltable;i++){
          cout<<setw(4)<<array[i].tablenum<<setw(5)<<array[i].tsize<<setw(7)<<array[i].numppl
           <<setw(9)<<array[i].occupied<<setw(13)<<array[i].otime<<setw(9)<<array[i].omin<<setw(10)<<array[i].overtime<<endl;
        }
        //showing the new waiting customers
        cout<<"Top 10 waiting list:";
        for(int i=0;i<10;i++){
          cout<<pendinglist[i]<<" ";
        }
        cout<<endl;
      }
    }
    //reseting table
    else if(n==-2){
      cout<<"Please input the total number of tables:";
      //input the number of table again
      cin>>totaltable;
      table* array = new table [totaltable];
      //read the distribution of table
      readtable(totaltable,array);
      cout<<endl;
      cout<<setw(4)<<"No."<<setw(5)<<"Size"<<setw(7)<<"Numppl"<<setw(9)<<"Occupied"
       <<setw(13)<<"Sit-in time"<<setw(9)<<" Meal time(min)"<<setw(10)<<"Overtime"<<endl;
      for(int i=0;i<totaltable;i++){
        cout<<setw(4)<<array[i].tablenum<<setw(5)<<array[i].tsize<<setw(7)<<array[i].numppl<<
         setw(9)<<array[i].occupied<<setw(13)<<array[i].otime<<setw(9)<<array[i].omin<<setw(10)<<array[i].overtime<<endl;
      }
    }
    //input the number of customer moving in
    else{
      for(int i=0;i<totaltable;i++){
        if(array[i].otime!="00:00")
          timecompare(array[i].otime,array,i);
      }
      if(n>max){
        //error if the size of customers exceeds the maximum suze of table
        cout<<"ERROR: lnvalid zize of customers!"<<endl;
      }
      else{
        //allow customers to pending or sit in
        sitin(n, totaltable,array,pendinglist,max,max2);
      }
    }
    cout<<endl;
    cout<<"Please insert the number of people: "<<"\n"<<"(People leave their table --- insert -1 then insert table number)"
     <<"\n"<<"(Exit the programm "<<setw(20)<<" --- insert 0)"<<"\n"<<"(Change the ditribution of table "<<setw(10)<<" --- insert -2)"<<endl;
    cin>>n;
  }
  //output the results
  ofstream fout;
  fout.open("output.txt");
  if(fout.fail()){
    exit(1);
  }
  fout<<setw(4)<<"No."<<setw(5)<<"Size"<<setw(7)<<"Numppl"<<setw(9)<<"Occupied"
   <<setw(13)<<"Sit-in time"<<setw(9)<<" Meal time(min)"<<setw(10)<<"Overtime"<<endl;

  for(int i=0;i<totaltable;i++){
    fout<<setw(4)<<array[i].tablenum<<setw(5)<<array[i].tsize<<setw(7)<<array[i].numppl
     <<setw(9)<<array[i].occupied<<setw(13)<<array[i].otime<<setw(9)<<array[i].omin<<setw(10)<<array[i].overtime<<endl;
  }
  fout<<"Top 10 waiting list:";
  for(int i=0;i<10;i++){
    fout<<pendinglist[i]<<" ";
  }
  fout<<endl;
  fout.close();
}
