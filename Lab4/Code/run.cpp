#include <iostream>
using namespace std;




int func1(int a[2]) {
  int idx = 0;
  int len = 2;
  int m = 0;
  while (idx < len) {
    if (a[idx] > m) {
      m = a[idx];
    }
    idx = idx + 1;
  }
  return m;
}

int func2(int p1, int p2) {
  int t1 = p1 / p2;
  int t2 = t1 * p2;
  int t3 = p1 - t2;
  return t3;
}

int main() {
  int arr_len = 2;
  int arr[2][2];
  int sum_arr[2];
  int idx1 = 0;
  int idx2 = 0;

  while (idx1 < arr_len) {
    idx2 = 0;
    while (idx2 < arr_len) {
      arr[idx1][idx2] = func2(idx1 + idx2, arr_len);
      idx2 = idx2 + 1;
    }
    sum_arr[idx1] = func1(arr[idx1]);
    idx1 = idx1 + 1;
  }
  cout<<arr[0][0]<<endl;
  cout<<arr[0][1]<<endl;
  cout<<arr[1][0]<<endl;
  cout<<arr[1][1]<<endl;
  cout<<sum_arr[0]<<endl;
  cout<<sum_arr[1]<<endl;
  return 0;
}