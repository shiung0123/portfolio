#include <iostream>
#include <deque>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

#define TOP dataset.front()

class Data {
public:
  int ID;
  int CPU_burst;
  int arrival;
  int priority;
  int start;
  int CPU_actual;
  int CPU_last;
  int CPU_burst_o;
  Data(int id, int brust, int arr, int p) {
    ID = id;
    CPU_burst = brust;
    arrival = arr;
    priority = p;
    start = arr;
    CPU_actual = 0;
    CPU_last = -1;
    CPU_burst_o = brust;
  } // Data()
};

bool ID( Data a, Data b ) {
  if ( a.ID < b.ID ) return true;
  else return false;
} // ID()

bool FCFS( Data a, Data b ) {
  if ( a.arrival < b.arrival ) return true;
  else if ( a.arrival > b.arrival ) return false;
  else if ( a.ID < b.ID ) return true;
  else return false;
} // FCFS()

bool RR( Data a, Data b ) {
  if ( a.arrival < b.arrival ) return true;
  else if ( a.arrival > b.arrival ) return false;
  else if ( a.CPU_last == -1 && b.CPU_last != -1 ) return true;
  else if ( b.CPU_last == -1 && a.CPU_last != -1 ) return false;
  else if ( a.ID < b.ID ) return true;
  else return false;
} // RR()

bool PSJF_init( Data a, Data b ) {
  if ( a.arrival < b.arrival ) return true;
  else if ( a.arrival > b.arrival ) return false;
  else if ( a.CPU_burst < b.CPU_burst ) return true;
  else if ( a.CPU_burst > b.CPU_burst ) return false;
  else if ( a.ID < b.ID ) return true;
  else return false;
} // PSJF_init()

bool PSJF( Data a, Data b ) {
  if ( a.CPU_burst < b.CPU_burst ) return true;
  else if ( a.CPU_burst > b.CPU_burst ) return false;
  else if ( a.CPU_last == -1 && b.CPU_last != -1 ) return true;
  else if ( b.CPU_last == -1 && a.CPU_last != -1 ) return false;
  else if ( a.start < b.start ) return true;
  else if ( a.start > b.start ) return false;
  else if ( a.ID < b.ID ) return true;
  else return false;
} // PSJF()

bool PSJF_context( Data a, Data b ) {
  if ( a.CPU_burst < b.CPU_burst ) return true;
  else return false;
} // PSJF_context()

bool NSJF_init( Data a, Data b ) {
  if ( a.arrival < b.arrival ) return true;
  else if ( a.arrival > b.arrival ) return false;
  else if ( a.CPU_burst < b.CPU_burst ) return true;
  else if ( a.CPU_burst > b.CPU_burst ) return false;
  else if ( a.ID < b.ID ) return true;
  else return false;
} // NSJF_init()

bool NSJF( Data a, Data b ) {
  if ( a.CPU_burst < b.CPU_burst ) return true;
  else if ( a.CPU_burst > b.CPU_burst ) return false;
  else if ( a.arrival < b.arrival ) return true;
  else if ( a.arrival > b.arrival ) return false;
  else if ( a.ID < b.ID ) return true;
  else return false;
} // NSJF()

bool P_init( Data a, Data b ) {
  if ( a.arrival < b.arrival ) return true;
  else if ( a.arrival > b.arrival ) return false;
  else if ( a.priority < b.priority ) return true;
  else if ( a.priority > b.priority ) return false;
  else if ( a.ID < b.ID ) return true;
  else return false;
} // P_init()

bool P( Data a, Data b ) {
  if ( a.priority < b.priority ) return true;
  else if ( a.priority > b.priority ) return false;
  else if ( a.CPU_last == -1 && b.CPU_last != -1 ) return true;
  else if ( b.CPU_last == -1 && a.CPU_last != -1 ) return false;
  else if ( a.start < b.start ) return true;
  else if ( a.start > b.start ) return false;
  else if ( a.ID < b.ID ) return true;
  else return false;
} // P()

bool P_context( Data a, Data b ) {
  if ( a.priority < b.priority ) return true;
  else return false;
} // P_context()

class CPU {
  int timeslice;
public:
  CPU( int t_s ) {
    timeslice = t_s;
  } // CPU()

  void InsertionSort( deque<Data>& dataset, bool ( *cmp ) ( Data, Data ), int time, bool nonpreemptive ) {
    for ( int i = 1 + nonpreemptive; i < dataset.size() && dataset[i].arrival <= time; i++ ) {
      Data key = dataset[i];
      int j = i - 1;
      while ( j >= 0 + nonpreemptive && cmp( key, dataset[j] ) ) {
        dataset[j + 1] = dataset[j];
        j--;
      } // while
      dataset[j + 1] = key;
    } // for
  } // InsertionSort()

  bool Contextswitch( deque<Data>& const dataset, bool ( *cmp ) ( Data, Data ) ) {
    if ( dataset.size() < 2 ) return false;
    else return cmp( dataset[1], dataset[0] );
  } // Contextswitch

  deque<Data> simulate( deque<Data> dataset, int mode, string& Gantt ) {

    deque<Data> output;

    int time = 0;
    int timeslice_cur = 0;
    int time_offset = 0;

    bool ( *init_func_ptr )( Data, Data ) = FCFS;
    bool ( *func_ptr )( Data, Data ) = FCFS;
    bool ( *context_func_ptr )( Data, Data ) = FCFS;

    switch ( mode ) {
    case 1:
      init_func_ptr = FCFS;
      func_ptr = FCFS;
      timeslice_cur = -1;
      break;
    case 2:
      init_func_ptr = FCFS;
      func_ptr = RR;
      timeslice_cur = timeslice;
      break;
    case 3:
      init_func_ptr = PSJF_init;
      func_ptr = PSJF;
      context_func_ptr = PSJF_context;
      timeslice_cur = 0;
      break;
    case 4:
      init_func_ptr = NSJF_init;
      func_ptr = NSJF;
      timeslice_cur = -1;
      break;
    case 5:
      init_func_ptr = P_init;
      func_ptr = P;
      context_func_ptr = P_context;
      timeslice_cur = 0;
      break;
    } // switch

    InsertionSort( dataset, init_func_ptr, 2147483647, 0 );

    while ( !dataset.empty() ) {

      if ( TOP.arrival <= time ) {

        TOP.CPU_actual++;
        TOP.CPU_burst--;
        TOP.CPU_last = time;
        if ( TOP.ID < 10 ) Gantt += TOP.ID + '0';
        else if ( TOP.ID < 36 ) Gantt += TOP.ID + 'A' - 10;
        else Gantt += TOP.ID + 'a' - 36;

        
        if ( TOP.CPU_burst_o == TOP.CPU_actual ) {
          output.push_back( TOP ); // make_pair( time + 1 - TOP.start - TOP.CPU_burst, time + 1 - TOP.start );
          time_offset = time + 1;
          dataset.pop_front();
          InsertionSort( dataset, func_ptr, time + 1, 0 );
        } // if program end

        else if ( timeslice_cur == 0 ) {
          InsertionSort( dataset, func_ptr, time + 1, 1 );
          if ( Contextswitch( dataset, context_func_ptr ) ) {
            TOP.arrival = time + 1;
            InsertionSort( dataset, func_ptr, time + 1, 0 );
          } // if
        } // if preemptive

        else if ( timeslice_cur != -1 && ( time + 1 - time_offset ) % timeslice_cur == 0 ) {
          TOP.arrival = time + 1;
          InsertionSort( dataset, func_ptr, time + 1, 0 );
        } // if timeout RR

      } // cpu have program
      else {
        Gantt += '-';
        time_offset = time + 1;
      } // cpu stall

      time++;

    } // while still have program

    InsertionSort( output, ID, 2147483647, 0 );
    return output;
  } // simulate()
};

string GetModeName( int mode ) {
  switch ( mode ) {
  case 1:
    return "FCFS";
  case 2:
    return "RR";
  case 3:
    return "PSJF";
  case 4:
    return "NSJF";
  case 5:
    return "Priority";
  } // switch
  return "Error";
} // GetMode()

int main() {

  fstream fin;
  fstream fout;
  while ( true ) {

    // open file
    string filename;
    while ( true ) {
      cout << "Please input a filename, not include extension (XX.txt) or [Q]uit : ";
      cin >> filename;
      if ( filename == "Q" ) return 0;

      fin.open( filename + ".txt", ios::in );
      if ( !fin ) cout << "There is no such file in .../Project1/Project1" << endl;
      else {
        fout.open( filename + "_output.txt", ios::out | ios::trunc );
        if ( !fout ) cout << "Create file error" << endl;
        else break;
      } // check file success
    } // while
    // open file

    int mode = -1;
    int timeslice = -1;

    // read argument
    fin >> mode >> timeslice;
    string trash;
    getline( fin, trash );
    getline( fin, trash );
    // read argument

    deque<Data> dataset;
    // read dataset
    int t1, t2, t3, t4;
    while ( fin >> t1 >> t2 >> t3 >> t4 ) {
      Data temp( t1, t2, t3, t4 );
      dataset.push_back( temp );
    } // while not eof
    // read dataset

    deque<deque<Data>> output;
    deque<string> modename;
    CPU myCPU( timeslice );

    for ( int i = 1; i < 6; i++ ) {
      if ( i == mode || mode == 6 ) {
        string name = GetModeName( i );
        modename.push_back( name );
        fout << "==" << setw( 10 ) << name << "==" << endl;

        string Gantt = "";
        output.push_back( myCPU.simulate( dataset, i, Gantt ) );
        fout << Gantt << endl;
      } // if
    } // for

    fout << "===========================================================" << endl << endl;

    fout << "Waiting Time" << endl;
    fout << "ID" ;
    for ( string name : modename ) fout << setw( 10 ) << name;
    fout << endl << "===========================================================" << endl;
    for ( int i = 0; i < dataset.size(); i++ ) {
      for ( int j = 0; j < output.size(); j++ ) {
        Data temp = output[j][i];
        if ( j == 0 ) fout << temp.ID;
        fout << setw( 10 ) << temp.CPU_last + 1 - temp.start - temp.CPU_burst_o;
      } // for
      fout << endl;
    } // for

    fout << "===========================================================" << endl << endl;

    fout << "Turnaround Time" << endl;
    fout << "ID" ;
    for ( string name : modename ) fout << setw( 10 ) << name;
    fout << endl << "===========================================================" << endl;
    for ( int i = 0; i < dataset.size(); i++ ) {
      for ( int j = 0; j < output.size(); j++ ) {
        Data temp = output[j][i];
        if ( j == 0 ) fout << temp.ID;
        fout << setw( 10 ) << temp.CPU_last + 1 - temp.start;
      } // for
      fout << endl;
    } // for

    fout << "===========================================================" << endl << endl;

    // close file
    fin.close();
    fout.close();
    // close file

  } // while

} // main()