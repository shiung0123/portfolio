#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
using namespace std;

fstream fin;
fstream fout;

class Page {
public:
  int Key;
  int timestamp; // LRU/LFU/MFU
  int timestamp_begin; // FIFO
  float timestamp_second; //SC
  int ref; // ARB
  int shift; // ARB
  int ref_second; // SC
  int counter; // LFU/MFU
  Page(int key, int time) {
    Key = key;
    timestamp_begin = time;
    timestamp = time;
    timestamp_second = time;
    ref_second = 1;
    ref = 1;
    shift = 0;
    counter = 1;
  } // Page()
};

// cmp( select, cur )
// true : select = cur
// false : select = select

bool FIFO( Page a, Page b ) {
  if ( b.timestamp_begin < a.timestamp_begin ) return true;
  else return false;
} // FIFO()

bool LRU( Page a, Page b ) {
  if ( b.timestamp < a.timestamp ) return true;
  else return false;
} // LRU()

bool Additional_Reference_Bits( Page a, Page b ) {
  if ( b.shift < a.shift ) return true;
  else return false;
} // Additional_Reference_Bits()

bool Second_Chance( Page a, Page b ) {
  if ( b.timestamp_second < a.timestamp_second ) return true;
  else return false;
} // Second_Chance()

bool Least_Frequently_Used( Page a, Page b ) {
  if ( b.counter < a.counter ) return true;
  else if ( b.counter > a.counter ) return false;
  else if ( b.timestamp < a.timestamp ) return true;
  else return false;
} // Least_Frequently_Used()

bool Most_Frequently_Used( Page a, Page b ) {
  if ( b.counter > a.counter ) return true;
  else if ( b.counter < a.counter ) return false;
  else if ( b.timestamp < a.timestamp ) return true;
  else return false;
} // Most_Frequently_Used()

class Page_Table {
  int size;
public:
  Page_Table( int p ) {
    size = p;
  } // CPU()

  int Select( const vector<Page>& dataset, bool ( *cmp ) ( Page, Page ) ) {
    int sel = 0;
    for ( int i = 0; i < dataset.size(); i++ ) {
      if ( cmp( dataset[sel], dataset[i] ) ) sel = i;
    } // for
    return sel;
  } // Select()

  int Select_Second_Chance( vector<Page>& dataset, bool ( *cmp ) ( Page, Page ), const int time ) {
    float offset = 0.01;
    while ( true ) {
      int sel = 0;
      for ( int i = 0; i < dataset.size(); i++ ) {
        if ( cmp( dataset[sel], dataset[i] ) ) sel = i;
      } // for

      if ( dataset[sel].ref_second == 1 ) {
        dataset[sel].ref_second = 0;
        dataset[sel].timestamp_second = time + offset;
      } // if sc
      else return sel;
      
      offset += 0.01;
    } // while
  } // Select()

  void simulate( queue<int> dataset, int mode ) {

    int time = 0;
    bool ( *func_ptr )( Page, Page ) = NULL;


    fout << "-----------------";
    switch ( mode ) {
    case 1:
      func_ptr = FIFO;
      fout << "FIFO";
      break;
    case 2:
      func_ptr = LRU;
      fout << "LRU";
      break;
    case 3:
      func_ptr = Additional_Reference_Bits;
      fout << "Additional_Reference_Bits";
      break;
    case 4:
      func_ptr = Second_Chance;
      fout << "Second_Chance";
      break;
    case 5:
      func_ptr = Least_Frequently_Used;
      fout << "Least_Frequently_Used";
      break;
    case 6:
      func_ptr = Most_Frequently_Used;
      fout << "Most_Frequently_Used";
      break;
    } // switch
    fout << "-----------------" << endl;


    int page_fault_time = 0;

    vector<Page> table;
    while ( !dataset.empty() ) {
      int cur = dataset.front();
      dataset.pop();

      // print cur
      if ( cur < 10 ) fout << char( cur + '0' );
      else if ( cur < 36 ) fout << char( cur + 'A' - 10 );
      else fout << char( cur + 'a' - 36 );
      fout << '\t' << '\t';
      // print cur end

      bool page_fault = true;

      // serash page
      for ( int i = 0; page_fault && i < table.size(); i++ ) {
        if ( table[i].Key == cur ) {
          table[i].timestamp = time;
          table[i].ref_second = 1;
          table[i].ref = 1;
          table[i].counter++;
          page_fault = false;
        } // if exist
      } // for all until find
      // serash page end

      // page_fault
      if ( page_fault ) {
        page_fault_time++;

        if ( table.size() < size ) {
          // Key = key; timestamp = time; ref = 1; shift = 0; counter = 1;
          Page temp = Page( cur, time );
          table.push_back( temp );
        } // free frame not full
        else {
          int replace_index;
          if ( mode != 4 ) replace_index = Select( table, func_ptr );
          else replace_index = Select_Second_Chance( table, func_ptr, time - 1 );

          Page temp = Page( cur, time );
          table[replace_index] = temp;
        } // page replacement

      } // if page_fault
      // page_fault end

      // shift
      for ( int i = 0; i < table.size(); i++ ) {
        table[i].shift /= 2;
        if ( table[i].ref == 1 ) {
          table[i].ref = 0;
          table[i].shift += 32768; // 1000 0000 0000 0000
        } // if ref
      } // for all
      // shift end

      // print table
      for ( auto i : table ) {
        if ( i.Key < 10 ) fout << char( i.Key + '0' );
        else if ( i.Key < 36 ) fout << char( i.Key + 'A' - 10 );
        else fout << char( i.Key + 'a' - 36 );
        // fout << '(' << i.ref_second << ',' << i.timestamp_second << ')';
      } // for all
      fout << '\t' << '\t' << '\t';
      // print table end
      
      // print page_fault
      if ( page_fault ) fout << 'F';
      fout << endl;
      // print page_fault end

      time++;
    } // while still have data

    fout << "Page Fault = " << page_fault_time 
      << " Page Replaces = " << page_fault_time - size 
      << " Page Frames = " << size << endl << endl;
  } // simulate()

};

int main() {

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

    int size = -1;
    // read argument
    fin >> size;
    string trash;
    getline( fin, trash );
    // read argument

    queue<int> dataset;
    // read dataset
    char ch;
    while ( fin >> ch ) {
      int temp = -1;
      if ( '0' <= ch && ch <= '9' ) temp = ch - '0';
      else if ( 'A' <= ch && ch <= 'Z' ) temp = ch - 'A' + 10;
      else if ( 'a' <= ch && ch <= 'z' ) temp = ch - 'a' + 36;
      
      if (temp != -1 ) dataset.push( temp );
    } // while not eof
    // read dataset

    Page_Table Table( size );
    for ( int i = 1; i <= 6; i++ ) {
      Table.simulate( dataset, i );
    } // for

    // close file
    fin.close();
    fout.close();
    // close file

  } // while

} // main()