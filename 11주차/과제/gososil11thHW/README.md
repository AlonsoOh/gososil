#### [gososil11_2.cu](https://github.com/AlonsoOh/gososil11thHW/blob/master/gososil11thHW/gososil11_2.cu)

CPU에서 계산한 결과와 GPU에서 계산한 결과를 비교하고 두 연산 속도를 비교합니다.
69번 줄의 BLOCK_SIZE를 조절하여 CUDA 실행시 thread block 크기를 조절할 수 있습니다.
70번 줄의 DATA_SIZE를 조절하여 전체 데이터 크기를 조절할 수 있습니다.

CPU와 GPU는 다른 시간 측정 코드를 이용하며 GPU 시간 측정시 데이터 전송 시간을 제외한 
순수 커널 실행 시간만 측정합니다.

#### [gososil11_3.cu](https://github.com/AlonsoOh/gososil11thHW/blob/master/gososil11thHW/gososil11_3.cu)

주어진 데이터에서 각 원소의 앞뒤 nf만큼 원소들을 더합니다.
67번 줄부터 72번 줄까지 변수들을 조절하여 다른 조건에서 실험을 진행할 수 있습니다.

주석으로 Todo 라 되어있는 부분을 작성하면 됩니다.
