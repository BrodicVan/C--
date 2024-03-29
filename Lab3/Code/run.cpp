#include <iostream>
using namespace std;


struct S1{
  int a;
  int b;
};

struct S2{
  int c;
  struct S1 arr[2];
};

int func1(struct S1 p1, struct S1 p2) {
  int a1 = p1.a * p2.a;
  int b1 = p1.b * p2.b;
  cout<<p1.b<<endl;
  cout<<p2.b<<endl;
  cout<<b1<<endl;
  return a1 + b1;
}

int func2(int p) {
  int idx = 0;
  while (idx <= p) {
    int i = idx * idx;
    int j = (idx + 1) * (idx + 1);
    if ((p >= i) && (p < j)) {
      return idx;
    }
    idx = idx + 1;
  }
  return -1;
}

int func3(struct S1 pp1, struct S1 pp2) {
  int t1 = func1(pp1, pp2);
  return func2(t1);
}

int main() {
  int arr_len = 2;
  struct S2 arrS[2];
  int idx1 = 0;
  int idx2 = 0;
  while (idx1 < 2) {
    idx2 = 0;
    while (idx2 < 2) {
      arrS[idx1].arr[idx2].a = idx1 * idx2 + idx2;
      arrS[idx1].arr[idx2].b = idx1 * idx1 * idx2 + idx2;
      idx2 = idx2 + 1;
    }
    arrS[idx1].c = func3(arrS[idx1].arr[0], arrS[idx1].arr[1]);
    idx1 = idx1 + 1;
  }
  cout<<arrS[0].c + arrS[1].c<<endl;
  return 0;
}
