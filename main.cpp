#include <fstream>
#include <iostream>
#include <math.h>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;

queue<int> tsq;
mutex m;
bool writer_done = false;

void start_reader(const string &input_file) {
  ifstream file(input_file);
  int num;

  if (file.is_open()) {
    while (file >> num) {
      m.lock();
      tsq.push(num);
      m.unlock();
    }
    file.close();
  } else {
    cout << "Unable to open file: " << input_file << endl;
    cout << "Terminating..." << endl;
    exit(1);
  }
}

void start_writer() {
  ofstream even_file("even.out");
  ofstream odd_file("odd.out");
  ofstream positive_file("positive.out");
  ofstream negative_file("negative.out");
  ofstream squared_file("squared.out");
  ofstream cube_file("cube.out");

  while (true) {
    m.lock();
    if (tsq.empty() && writer_done) {
      m.unlock();
      break;
    } else if (tsq.empty()) {
      m.unlock();
      continue;
    }

    int value = tsq.front();
    tsq.pop();
    m.unlock();

    // odd/even
    if (value % 2 == 0){
      even_file << value << endl;
    }
    else{
      odd_file << value << endl;
    }
    // +/-
    if (value > 0){
      positive_file << value << endl;
    }
      int sqrt_num = sqrt(value);
      if (sqrt_num * sqrt_num == value) {
        squared_file << value << endl;
      }
    else{
      negative_file << value << endl;
    }
    // square

    
    

    // cube
    int cube_num = cbrt(value);
    if (cube_num * cube_num * cube_num == value) {
      cube_file << value << endl;
    }
  }

  even_file.close();
  odd_file.close();
  positive_file.close();
  negative_file.close();
  squared_file.close();
  cube_file.close();
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <input_file>" << endl;
    return 1;
  }

  string input_file = argv[1];
  cout << "Input file: " << input_file << endl;

  std::thread reader_thread(start_reader, input_file);
  std::thread writer_thread(start_writer);

  reader_thread.join();
  writer_done = true;
  writer_thread.join();

  return 0;
}
