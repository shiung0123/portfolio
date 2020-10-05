
# include <iostream>
# include <vector>
# include <random>
# include <time.h> 
# include <thread>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/shm.h>
# include <fstream>
# include <string>
using namespace std;


void swap( int& a, int& b ) {
  int c( a ); a = b; b = c;
}

void Bubble_P( vector<int> data, vector<int>& data_out )
{
  int n = data.size();
  bool flag = true;
  for ( int i = 0 ; i < n - 1 && flag ; i++ ) {
    flag = false;
    for ( int j = 0 ; j < n - 1 - i ; j++ ) {
      if ( data[j + 1] < data[j] ) {
        swap( data[j + 1], data[j] );
        flag = true;
      }
    }
  }

  for ( auto i : data ) data_out.push_back( i );
}

void Bubble_MT( vector<int>& data, int start, int end )
{
  bool flag = true;
  for ( int i = 0 ; i < end - start - 1 && flag ; i++ ) {
    flag = false;
    for ( int j = start ; j < end - 1 - i ; j++ ) {
      if ( data[j + 1] < data[j] ) {
        swap( data[j + 1], data[j] );
        flag = true;
      }
    }
  }
  // cout << start << endl;
}

void Merge_MT( vector<int>& data, int start, int mid, int end )
{
  vector<int> temp;
  temp.reserve( end - start );

  int i = start;
  int j = mid;
  while ( i < mid && j < end ) {
    if ( data[i] <= data[j] ) temp.push_back( data[i++] );
    else temp.push_back( data[j++] );
  }
  while ( i < mid ) temp.push_back( data[i++] );
  while ( j < end ) temp.push_back( data[j++] );
 
  for ( int k = 0; k < temp.size(); k++ ) data[k + start] = temp[k];

  // cout << "sort:" << start << "to" << end << endl;
}

void Bubble_MT_Merge_MT( vector<int> data, int k, vector<int>& data_out )
{
  int n = data.size();
  int block_size = n / k ;

  vector<thread> threads;
  threads.reserve( k );
  for ( int i = 0; i < k; i++ ) {
    int start = i * block_size;
    int end = (i + 1) * block_size;
    if ( i == k - 1 ) end = n;
    threads.push_back( thread( Bubble_MT, ref( data ), start, end ) );
  }
  for ( auto& t : threads ) t.join();

  int block_num = 1;
  while ( k != 1 ) {
    threads.clear();
    int i;
    for ( i = 0; i < k / 2; i++ ) {
      int start = i * 2 * block_num * block_size;
      int mid = start + block_num * block_size;
      int end = start + 2 * block_num * block_size;
      if ( ( i + 1 ) * 2 == k ) end = n;
      threads.push_back( thread( Merge_MT, ref( data ), start, mid, end ) );
    }
    k -= i;
    block_num *= 2;
    for ( auto& t : threads ) t.join();
  }

  for ( auto i : data ) data_out.push_back( i );
}

void Bubble_MP( int* data, int start, int end )
{
  bool flag = true;
  for ( int i = 0 ; i < end - start - 1 && flag ; i++ ) {
    flag = false;
    for ( int j = start ; j < end - 1 - i ; j++ ) {
      if ( data[j + 1] < data[j] ) {
        swap( data[j + 1], data[j] );
        flag = true;
      }
    }
  }
}

void Merge_MP( int* data, int start, int mid, int end )
{
  vector<int> temp;
  temp.reserve( end - start );

  int i = start;
  int j = mid;
  while ( i < mid && j < end ) {
    if ( data[i] <= data[j] ) temp.push_back( data[i++] );
    else temp.push_back( data[j++] );
  }
  while ( i < mid ) temp.push_back( data[i++] );
  while ( j < end ) temp.push_back( data[j++] );

  for ( int k = 0; k < temp.size(); k++ ) data[k + start] = temp[k];

  // cout << "sort:" << start << "to" << end << endl;
}

void Bubble_MP_Merge_MP( vector<int> data, int k, vector<int>& data_out )
{
  int n = data.size();
  int block_size = n / k ;

  int key = 66666;
  int shmid = shmget( key, sizeof( int ) * n, IPC_CREAT | 0666 ); // != -1
  int* shm = ( int* ) shmat( shmid, NULL, 0 ); // != -1
  for ( int i = 0; i < n; i++ ) *( shm + i ) = data[i];

  pid_t pid = 0;
  for ( int i = 0; i < k; i++ )
  {
    pid = fork();
    switch ( pid )
    {
    case 0:  // this is the child process
    {
      int shmid = shmget( key, sizeof( int ) * n, IPC_CREAT | 0666 ); // != -1
      int* shm = ( int* ) shmat( shmid, NULL, 0 ); // != -1
      int start = i * block_size;
      int end = ( i + 1 ) * block_size;
      if ( i == k - 1 ) end = n;
      Bubble_MP( shm, start, end );
      shmdt( shm ); // != -1
      exit( 0 );   // do not get out of this block of code in the child
      break;
    }
    case -1:  // error
      perror( "failed to fork a new process" );
      exit( 1 );
      break;
    }
  }
  for ( int i = 0; i < k; i++ ) wait( NULL );

  int block_num = 1;
  while ( k != 1 ) {
    pid = 0;
    int i;
    for ( i = 0; i < k / 2; i++ )
    {
      pid = fork();
      switch ( pid )
      {
      case 0:  // this is the child process
      {
        int shmid = shmget( key, sizeof( int ) * n, IPC_CREAT | 0666 ); // != -1
        int* shm = ( int* ) shmat( shmid, NULL, 0 ); // != -1
        int start = i * 2 * block_num * block_size;
        int mid = start + block_num * block_size;
        int end = start + 2 * block_num * block_size;
        if ( ( i + 1 ) * 2 == k ) end = n;
        Merge_MP( shm, start, mid, end );
        shmdt( shm ); // != -1
        exit( 0 );   // do not get out of this block of code in the child
        break;
      }
      case -1:  // error
        perror( "failed to fork a new process" );
        exit( 1 );
        break;
      }
    }
    k -= i;
    block_num *= 2;
    for ( int j = 0; j < i; j++ ) wait( NULL );
  } // while k != 1

  struct shmid_ds buf;
  shmctl( shmid, IPC_RMID, &buf ); // != -1

  for ( int i = 0 ; i < n ; i++ ) data_out.push_back( *( shm + i ) );
}

void Bubble_KP_Merge_P( vector<int> data, int k, vector<int>& data_out )
{
  int n = data.size();
  int block_size = n / k ;

  int key = 66666;
  int shmid = shmget( key, sizeof( int ) * n, IPC_CREAT | 0666 ); // != -1
  int* shm = ( int* ) shmat( shmid, NULL, 0 ); // != -1
  for ( int i = 0; i < n; i++ ) *( shm + i ) = data[i];

  pid_t pid = 0;
  for ( int i = 0; i < k; i++ )
  {
    pid = fork();
    switch ( pid )
    {
    case 0:  // this is the child process
    {
      int shmid = shmget( key, sizeof( int ) * n, IPC_CREAT | 0666 ); // != -1
      int* shm = ( int* ) shmat( shmid, NULL, 0 ); // != -1
      int start = i * block_size;
      int end = ( i + 1 ) * block_size;
      if ( i == k - 1 ) end = n;
      Bubble_MP( shm, start, end );
      shmdt( shm ); // != -1
      exit( 0 );   // do not get out of this block of code in the child
      break;
    }
    case -1:  // error
      perror( "failed to fork a new process" );
      exit( 1 );
      break;
    }
  }
  for ( int i = 0; i < k; i++ ) wait( NULL );

  vector<int> start_i;
  vector<int> end_i;
  for ( int i = 0; i < k; i++ ) {
    start_i.push_back( i * block_size );
    if (i != k - 1) end_i.push_back( (i + 1) * block_size );
    else end_i.push_back( n );
  }

  bool isdone = false;
  vector<int> temp;
  temp.reserve( n );

  while ( !isdone ) {
    isdone = true;
    int min_i = -1;
    for ( int i = 0; i < k; i++ ) {
      if ( start_i[i] < end_i[i] ) {
        isdone = false;
        if ( min_i == -1 ) min_i = i;
        if( shm[start_i[i]] < shm[start_i[min_i]] ) min_i = i;
      }
    }
    if ( !isdone ) temp.push_back( shm[start_i[min_i]++] );
  }

  struct shmid_ds buf;
  shmctl( shmid, IPC_RMID, &buf ); // != -1

  for ( auto i : temp ) data_out.push_back( i );
}

# define MENU_QUIT 0
# define BUBBLE_P 1
# define BUBBLE_MT_MERGE_MT 2
# define BUBBLE_MP_MERGE_MP 3
# define BUBBLE_KP_MERGE_P 4
# define DO_ALL 5

bool test(vector<int>& dataset_out) {
  bool check = true;
  for ( int i = 1; i < dataset_out.size(); i++ )
    if ( dataset_out[i - 1] > dataset_out[i] ) {
      check = false; break;
    }
  dataset_out.clear();
  return check;
}

int main() {

  int mode;   // 選單選項
  while ( true ) {
    fstream fin;
    fstream fout;
    vector<int> dataset;
    vector<int> dataset_out;
    dataset.reserve( 1000000 );
    dataset_out.reserve( 1000000 );
    int n = 0;
    int k = 0;

    // get filename start
    string filename;
    while ( true ) {
      mode = -1;
      cout << "Please input a filename, not include extension (XX.txt) or [Q]uit : ";
      cin >> filename;

      if ( filename == "000" ) {
        mode = 5;
        fout.open( "/root/projects/Project1/" + filename + "_output.txt", ios::out | ios::trunc );
        if ( !fout ) cout << "Create file error" << endl;
        else break;
      } // filename : 000 is testmode

      if ( filename == "Q" ) {
        mode = 0; break;
      } // filename : 000 is testmode

      fin.open( "/root/projects/Project1/" + filename + ".txt", ios::in );
      if ( !fin ) cout << "There is no such file in /root/projects/Project1/" << endl;
      else {
        fout.open( "/root/projects/Project1/" + filename + "_output.txt", ios::out | ios::trunc );
        if ( !fout ) cout << "Create file error" << endl;
        else break;
      } // check file success

    } // while
    // get filename end

    // read filedata start
    if ( mode != 0 && mode != 5 ) {
      fin >> mode;
      if ( mode <= 0 || mode >= 5 ) {
        cout << "Input mode error, Repair the file and try again" << endl;
        continue;
      } // check mode

      int temp;
      while ( fin >> temp ) dataset.push_back( temp );
    } // if mode need read dataset
    // read filedata end

    cout << "Read file success, dataset.size() = " << dataset.size() << endl;

    // set separate number start
    if ( mode == 2 || mode == 3 || mode == 4 ) {
      while ( k <= 0 ) {
        cout << "How much do you want to separate, give a positive number : ";
        cin >> k;
      } // while cheak k
    } // if mode need separate number start
    // set separate number end

    cout << "Start sorting..." << endl;

    switch ( mode ) {
    case MENU_QUIT:
      return 0; // quit

    case BUBBLE_P:
    {
      clock_t start_CPU = clock();
      time_t start_roundtrip = time( NULL );

      thread mThread( Bubble_P, dataset, ref( dataset_out ) );
      mThread.join();

      clock_t end_CPU = clock();
      time_t end_roundtrip = time( NULL );
      double cpu_time = ( ( double ) ( end_CPU - start_CPU ) ) / CLOCKS_PER_SEC;
      double turnaround_time = difftime( end_roundtrip, start_roundtrip );

      fout << "Bubble_P (cpu_time): " << cpu_time << endl;
      fout << "Bubble_P (turnaround_time): " << turnaround_time << endl;
      for ( auto i : dataset_out ) fout << i << ' ';
      if ( test( dataset_out ) ) fout << "Success" << endl;
      else fout << "Fail" << endl;

      break;
    } // Bubble_P

    case BUBBLE_MT_MERGE_MT:
    {
      clock_t start_CPU = clock();
      time_t start_roundtrip = time( NULL );

      thread mThread( Bubble_MT_Merge_MT, dataset, k, ref( dataset_out ) );
      mThread.join();

      clock_t end_CPU = clock();
      time_t end_roundtrip = time( NULL );
      double cpu_time = ( ( double ) ( end_CPU - start_CPU ) ) / CLOCKS_PER_SEC;
      double turnaround_time = difftime( end_roundtrip, start_roundtrip );

      fout << "Bubble_MT_Merge_MT (cpu_time): " << cpu_time << endl;
      fout << "Bubble_MT_Merge_MT (turnaround_time): " << turnaround_time << endl;
      for ( auto i : dataset_out ) fout << i << ' ';
      if ( test( dataset_out ) ) fout << "Success" << endl;
      else fout << "Fail" << endl;

      break;
    } // BUBBLE_MT_MERGE_MT

    case BUBBLE_MP_MERGE_MP:
    {
      clock_t start_CPU = clock();
      time_t start_roundtrip = time( NULL );

      thread mThread( Bubble_MP_Merge_MP, dataset, k, ref( dataset_out ) );
      mThread.join();

      clock_t end_CPU = clock();
      time_t end_roundtrip = time( NULL );
      double cpu_time = ( ( double ) ( end_CPU - start_CPU ) ) / CLOCKS_PER_SEC;
      double turnaround_time = difftime( end_roundtrip, start_roundtrip );

      fout << "Bubble_MP_Merge_MP (cpu_time): " << cpu_time << endl;
      fout << "Bubble_MP_Merge_MP (turnaround_time): " << turnaround_time << endl;
      for ( auto i : dataset_out ) fout << i << ' ';
      if ( test( dataset_out ) ) fout << "Success" << endl;
      else fout << "Fail" << endl;

      break;
    } // BUBBLE_MP_MERGE_MP

    case BUBBLE_KP_MERGE_P:
    {
      clock_t start_CPU = clock();
      time_t start_roundtrip = time( NULL );

      thread mThread( Bubble_KP_Merge_P, dataset, k, ref( dataset_out ) );
      mThread.join();

      clock_t end_CPU = clock();
      time_t end_roundtrip = time( NULL );
      double cpu_time = ( ( double ) ( end_CPU - start_CPU ) ) / CLOCKS_PER_SEC;
      double turnaround_time = difftime( end_roundtrip, start_roundtrip );

      fout << "Bubble_KP_Merge_P (cpu_time): " << cpu_time << endl;
      fout << "Bubble_KP_Merge_P (turnaround_time): " << turnaround_time << endl;
      for ( auto i : dataset_out ) fout << i << ' ';
      if ( test( dataset_out ) ) fout << "Success" << endl;
      else fout << "Fail" << endl;

      break;
    } // BUBBLE_KP_MERGE_P
    
    case DO_ALL:
    {
      clock_t start_CPU;
      time_t start_roundtrip;
      clock_t end_CPU;
      time_t end_roundtrip;
      double cpu_time;
      double turnaround_time;

      // n = 1000, 10000, 100000, 500000, 1000000
      for ( n = 1000 ; n <= 1000000; n *= 10 ) {
        // generate data
        random_device rd;
        default_random_engine gen = std::default_random_engine( rd() );
        uniform_int_distribution<int> dis( -2147483648, 2147483647 );
        for ( int i = dataset.size() ; i < n ; i++ ) dataset.push_back( dis( gen ) );
        for ( int i = 0 ; i < n ; i++ ) fout << dataset[i] << endl;

        // print n 
        fout << "n = " << n << endl;

        // Bubble_P
        start_CPU = clock();
        start_roundtrip = time( NULL );

        thread mThread1( Bubble_P, dataset, ref( dataset_out ) );
        mThread1.join();

        end_CPU = clock();
        end_roundtrip = time( NULL );
        cpu_time = ( ( double ) ( end_CPU - start_CPU ) ) / CLOCKS_PER_SEC;
        turnaround_time = difftime( end_roundtrip, start_roundtrip );

        // for ( int i = 0 ; i < n ; i++ ) fout << dataset_out[i] << endl;
        if ( test( dataset_out ) ) fout << "Success" << endl;
        else fout << "Fail" << endl;
        fout << "Bubble_P (cpu_time): " << cpu_time << endl;
        fout << "Bubble_P (turnaround_time): " << turnaround_time << endl;
        fout << endl;

        // n = 1000, k = 1, 10, 100
        // n = 10000, k = 1, 10, 100, 1000
        // n = 100000, k = 1, 10, 100, 1000, 10000
        // n = 500000, k = 1, 10, 100, 1000, 10000
        // n = 1000000, k = 1, 10, 100, 1000, 10000
        for ( k = 1 ; k < dataset.size(); k *= 10 ) {

          // print k 
          fout << "k = " << k << endl;

          // Bubble_MT_Merge_MT
          start_CPU = clock();
          start_roundtrip = time( NULL );

          thread mThread2( Bubble_MT_Merge_MT, dataset, k, ref( dataset_out ) );
          mThread2.join();

          end_CPU = clock();
          end_roundtrip = time( NULL );
          cpu_time = ( ( double ) ( end_CPU - start_CPU ) ) / CLOCKS_PER_SEC;
          turnaround_time = difftime( end_roundtrip, start_roundtrip );

          // for ( int i = 0 ; i < n ; i++ ) fout << dataset_out[i] << endl;
          if ( test( dataset_out ) ) fout << "Success" << endl;
          else fout << "Fail" << endl;
          fout << "Bubble_MT_Merge_MT (cpu_time): " << cpu_time << endl;
          fout << "Bubble_MT_Merge_MT (turnaround_time): " << turnaround_time << endl;
          fout << endl;

          // Bubble_MP_Merge_MP
          start_CPU = clock();
          start_roundtrip = time( NULL );

          thread mThread3( Bubble_MP_Merge_MP, dataset, k, ref( dataset_out ) );
          mThread3.join();

          end_CPU = clock();
          end_roundtrip = time( NULL );
          cpu_time = ( ( double ) ( end_CPU - start_CPU ) ) / CLOCKS_PER_SEC;
          turnaround_time = difftime( end_roundtrip, start_roundtrip );

          // for ( int i = 0 ; i < n ; i++ ) fout << dataset_out[i] << endl;
          if ( test( dataset_out ) ) fout << "Success" << endl;
          else fout << "Fail" << endl;
          fout << "Bubble_MP_Merge_MP (cpu_time): " << cpu_time << endl;
          fout << "Bubble_MP_Merge_MP (turnaround_time): " << turnaround_time << endl;
          fout << endl;

          // Bubble_KP_Merge_P
          start_CPU = clock();
          start_roundtrip = time( NULL );

          thread mThread4( Bubble_KP_Merge_P, dataset, k, ref( dataset_out ) );
          mThread4.join();

          end_CPU = clock();
          end_roundtrip = time( NULL );
          cpu_time = ( ( double ) ( end_CPU - start_CPU ) ) / CLOCKS_PER_SEC;
          turnaround_time = difftime( end_roundtrip, start_roundtrip );

          // for ( int i = 0 ; i < n ; i++ ) fout << dataset_out[i] << endl;
          if ( test( dataset_out ) ) fout << "Success" << endl;
          else fout << "Fail" << endl;
          fout << "Bubble_KP_Merge_P (cpu_time): " << cpu_time << endl;
          fout << "Bubble_KP_Merge_P (turnaround_time): " << turnaround_time << endl;
          fout << endl;

        } // for k = 1 ~ n/10

      } // for n = 1,000 ~ 1,000,000

      break;
    } // DO_ALL

    } // switch

    cout << "Finish." << endl;

    fin.close();
    fout.close();
  } // while

} // main


/*
int main() {
  int a = 0;
  thread mThread( test_func, ref(a) );
  mThread.join();
  cout << "main thread" << endl;
  cout << a << endl;
  return 0;
}
*/

/*
int main() {

  int key = 666;
  struct shmid_ds buf;
  pid_t pid = 0;
  for ( int i = 0; i < 10; i++ )
  {
    pid = fork();
    switch ( pid )
    {
    case 0:  // this is the child process
    {
      int shmid = shmget( key, sizeof( int ) * 10, IPC_CREAT | 0666 ); // != -1
      int* shm = ( int* ) shmat( shmid, NULL, 0 ); // != -1
      *( shm + i ) = i*2;
      cout << "C" << i << endl;
      shmdt( shm ); // != -1
      exit( 0 );   // do not get out of this block of code in the child
      break;
    }
    case -1:  // error
      perror( "failed to fork a new process" );
      exit( 1 );
      break;
    default:  // this is the parent process
      wait( NULL );
      cout << "EACH P WAIT" << endl;
      break;
    }
  }
  cout << "P NOT WAIT" << endl;
  for ( int i = 0; i < 10; i++ )
    wait( NULL );
    
  cout << "P WAIT" << endl;

  int shmid = shmget( key, sizeof( int ) * 10, IPC_CREAT | 0666 ); // != -1
  int* shm = ( int* ) shmat( shmid, NULL, 0 ); // != -1
  for ( int i = 0; i < 10; i++ ) cout << *( shm + i ) << endl;
  shmctl( shmid, IPC_RMID, &buf ); // != -1

}

*/

/*
int main() {
  string filename;
  cin >> filename;

  fstream fin;
  fin.open( "/root/projects/Project1/" + filename + ".txt", ios::in );
  if ( !fin ) cout << "There is no such file in /root/projects/Project1/" << endl;

  fstream fout;
  fout.open( "/root/projects/Project1/" + filename + "_output.txt", ios::out | ios::trunc );

  int num;
  while ( fin >> num ) fout << num << endl;
  fin.close();
  fout.close();
}
*/