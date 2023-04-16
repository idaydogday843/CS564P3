#include <iostream>
using namespace std;

void merge(int A[], int first, int last, int mid){
  int i, j, k, c[18];
  i = first;
  k = first;
  j = mid + 1;
  while (i <= mid && j <= last) {
    if (A[i] < A[j]) {
      c[k] = A[i];
        k++;
        i++;
    }
    else {
      c[k] = A[j];
      k++;
      j++;
    }
  }
  while (i <= mid) {
    c[k] = A[i];
    k++;
    i++;
  }
  while (j <= last) {
    c[k] = A[j];
    k++;
    j++;
  }
  for (i = first; i < k; i++) {
    A[i] = c[i];
  }
}

void mergeSort(int A[], int start, int end) {
  if (start < end) {
    int partition, blockIndex;
    partition = int((end - start) / 2);
    blockIndex = start;
    while (blockIndex < end) {
      if (partition + blockIndex > end) {
        mergeSort(A, blockIndex, end);
      }
      else {
        mergeSort(A, blockIndex, partition + blockIndex);
      }
      blockIndex += partition + 1;
    }
    blockIndex = (start + end) / 2;
    merge(A, start, end, blockIndex);
  }
  return;
}


int main() {
  int A[18] = {21,10,7,18,5,9,2,19,23,11,22,1,15,14,3,13,6,17};
  mergeSort(A, 0, 17);
  for (int i = 0; i < 18; i++) {
    cout << A[i] << endl;
  }
}