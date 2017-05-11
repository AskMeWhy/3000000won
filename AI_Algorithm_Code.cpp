// Samsung Go Tournament Form C (g++-4.8.3)
//�̰� ��������
/*
[AI �ڵ� �ۼ� ���]

1. char info[]�� �迭 �ȿ�					"TeamName:�ڽ��� ����,Department:�ڽ��� �Ҽ�"					������ �ۼ��մϴ�.
( ���� ) Teamname:�� Department:�� �� ���� �մϴ�.
"�ڽ��� ����", "�ڽ��� �Ҽ�"�� �����ؾ� �մϴ�.

2. �Ʒ��� myturn() �Լ� �ȿ� �ڽŸ��� AI �ڵ带 �ۼ��մϴ�.

3. AI ������ �׽�Ʈ �Ͻ� ���� "���� �˰����ȸ ��"�� ����մϴ�.

4. ���� �˰��� ��ȸ ���� �����ϱ⿡�� �ٵϵ��� ���� ��, �ڽ��� "����" �� �� �˰����� �߰��Ͽ� �׽�Ʈ �մϴ�.



[���� �� �Լ�]
myturn(int cnt) : �ڽ��� AI �ڵ带 �ۼ��ϴ� ���� �Լ� �Դϴ�.
int cnt (myturn()�Լ��� �Ķ����) : ���� �� �� �־��ϴ��� ���ϴ� ����, cnt�� 1�̸� ���� ���� ��  �� ����  �δ� ��Ȳ(�� ��), cnt�� 2�̸� �� ���� ���� �δ� ��Ȳ(�� ��)
int  x[0], y[0] : �ڽ��� �� ù �� ° ���� x��ǥ , y��ǥ�� ����Ǿ�� �մϴ�.
int  x[1], y[1] : �ڽ��� �� �� �� ° ���� x��ǥ , y��ǥ�� ����Ǿ�� �մϴ�.
void domymove(int x[], int y[], cnt) : �� ������ ��ǥ�� �����ؼ� ���


//int board[BOARD_SIZE][BOARD_SIZE]; �ٵ��� �����Ȳ ��� �־� �ٷλ�� ������. ��, ���������ͷ� ���� �������
// ������ ���� ��ġ�� �ٵϵ��� ������ �ǰ��� ó��.

boolean ifFree(int x, int y) : ���� [x,y]��ǥ�� �ٵϵ��� �ִ��� Ȯ���ϴ� �Լ� (������ true, ������ false)
int showBoard(int x, int y) : [x, y] ��ǥ�� ���� ���� �����ϴ��� �����ִ� �Լ� (1 = �ڽ��� ��, 2 = ����� ��, 3 = ��ŷ)


<-------AI�� �ۼ��Ͻ� ��, ���� �̸��� �Լ� �� ���� ����� �������� �ʽ��ϴ�----->
*/

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "Connect6Algo.h"

// "�����ڵ�[C]"  -> �ڽ��� ���� (����)
// "AI�μ�[C]"  -> �ڽ��� �Ҽ� (����)
// ����� ���������� �ݵ�� �������� ����!
char info[] = { "TeamName:�����ڵ�[C],Department:AI�μ�[C]" };


void myturn(int cnt) {

	int x[2], y[2];
	extern int showBoard(int x, int y);
	int(*getBoard())[20];          //������ ���带 �����ϴ� �迭
	int weight[20][20];
	for (int i = 0; i < 20; i++){
		for (int j = 0; j < 20; j++){
			weight[i][j] = 0;
		}
	}

	

	// �� �κп��� �˰��� ���α׷�(AI)�� �ۼ��Ͻʽÿ�. �⺻ ������ �ڵ带 ���� �Ǵ� �����ϰ� ������ �ڵ带 ����Ͻø� �˴ϴ�.
	// ���� Sample code�� AI�� Random���� ���� ���� Algorithm�� �ۼ��Ǿ� �ֽ��ϴ�.
	if (cnt == 1){
		x[0] = 9;
		y[0] = 9;
	}
	else{
		 int (*board)[20]= getBoard();            //���带 �ҷ���...

		/*�ӽ� ��������*/
		srand((unsigned)time(NULL));
		
			for (int i = 0; i < cnt; i++) {
			do {
				x[i] = rand() % width;
				y[i] = rand() % height;
				if (terminateAI) return;
				
			} while (!isFree(x[i], y[i]));
				
					if (x[1] == x[0] && y[1] == y[0]) i--;
				
		}

		/*�ӽ� ������*/
		//�ϴ� �׷� �꺣�̽����� ¥����.
		for (int i = 0; i < 20; i++) {			///��ü ���� ���� ���� ���� Ȯ��
			int flag = 0;							// �ӽ÷� ���� Ż���÷���
			for (int j = 0; j < 20; j++) {
				if (showBoard(i,j)==1) {        /// �Ʊ��� Ȯ��

				}
				else if (board[i][j] == 2) {
					if (board[i+1][j] == 2 && board[i+2][j] == 2 && board[i+3][j] == 2) {  ///���� 4�� Ȯ��
						if ((board[i - 1][j] == 3 || board[i - 1][j] == 1) && (board[i + 4][j] == 3 || board[i + 4][j] == 1)){ //������ �̹� �� ���������� ���
							continue;
						}
						else if (board[i + 4][j] == 2){							//5�� ������ 1 : �������� �涧
							if ((board[i + 5][j] == 1 || board[i + 5][j] == 3) && (board[i - 1][j] == 1 || board[i - 1][j] == 3)) //���� �ٸ��� ���
								continue;
							else if ((board[i + 5][j] == 1 || board[i + 5][j] == 3)){  //���ʸ� �������1 : ������
								x[0] = i - 1;
								y[0] = j;
								//eight[i - 1][j] = 999999;
							}
							else if ((board[i - 1][j] == 1 || board[i - 1][j] == 3)){ //���ʸ� �������2 : ����
								x[0] = i + 5;
								y[0] = j;
								//weight[i + 5][j] = 999999;
							}
							else{
								x[0] = i - 1;
								x[1] = i + 5;
								y[0] = j;
								y[1] = j;
								//weight[i + 5][j] = 999999;
								//weight[i - 1][j] = 999999;
							}

						}
						else if (board[i - 1][j] == 2){							//5�� ������ 2 : �������� �涧
							if ((board[i - 2][j] == 1 || board[i - 2][j] == 3) && (board[i + 4][j] == 1 || board[i + 4][j] == 3)) //���� �ٸ��� ���
								continue;
							else if ((board[i - 2][j] == 1 || board[i - 2][j] == 3)){  //���ʸ� �������1 : ������
								x[0] = i + 4;
								y[0] = j;
								//weight[i + 4][j] = 999999;

							}
							else if ((board[i + 4][j] == 1 || board[i + 4][j] == 3)){  //���ʸ� �������2 : ����
								x[0] = i - 2;
								y[0] = j;
								//weight[i - 2][j] = 999999;
							}
							else{
								x[0] = i - 2;
								x[1] = i + 4;
								y[0] = j;
								y[1] = j;
								//weight[i + 4][j] = 999999;
								//weight[i - 2][j] = 999999;
							}

						}
						else{										//�ƴϸ� �ϴ� ������ ��ġ. �ӽ���ġ��!
							x[0] = i - 1;
							x[1] = i + 4;
							y[0] = j;
							y[1] = j;
							//weight[i + 4][j] = 999999;
							//weight[i - 1][j] = 999999;
							break;
						}
					}
					if (board[i][j+1] == 2 && board[i][j+2] == 2 && board[i][j+3] == 2) {  ///���� 4�� Ȯ��
						if ((board[i][j - 1] == 3 || board[i][j - 1] == 1) && (board[i][j + 4] == 3 || board[i][j + 4] == 1)){ //������ �̹� �� ���������� ���
							continue;
						}
						else if (board[i][j - 1] == 2){							//5�� ������ 1 : �������� �涧
							if ((board[i][j - 2] == 3 || board[i][j - 2] == 1) && (board[i][j + 4] == 3 || board[i][j + 4] == 1)){ //������ �̹� �� ���������� ���
								continue;
							}
							else if ((board[i][j - 2] == 3 || board[i][j - 2] == 1)){   //���ʸ� �������1 : ����
								x[0] = i;
								y[0] = j + 4;
								weight[i][j + 4] = 999999;
							}
							else if ((board[i][j + 4] == 3 || board[i][j + 4] == 1)){   //���ʸ� �������2 : �Ʒ���
								x[0] = i;
								y[0] = j - 2;
							}
							else{
								x[0] = i;
								x[1] = i;
								y[0] = j - 2;
								y[1] = j + 4;
								//weight[i][j - 2] = 999999;
								//weight[i][j + 4] = 999999;
							}

						}
						else if (board[i][j + 4] == 2){							//5�� ������ 2 : �Ʒ������� �涧
							if ((board[i][j - 1] == 3 || board[i][j - 1] == 1) && (board[i][j + 5] == 3 || board[i][j + 5] == 1)){ //������ �̹� �� ���������� ���
								continue;
							}
							else if ((board[i][j - 1] == 3 || board[i][j - 1] == 1)){   //���ʸ� �������1 : ����
								x[0] = i;
								y[0] = j + 5;
								//weight[i][j + 5] = 999999;
							}
							else if ((board[i][j + 5] == 3 || board[i][j + 5] == 1)){   //���ʸ� �������2 : �Ʒ���
								x[0] = i;
								y[0] = j - 1;
								//weight[i][j - 1] = 999999;

							}
							else{
								x[0] = i;
								x[1] = i;
								y[0] = j - 1;
								y[1] = j + 5;
								//weight[i][j - 1] = 999999;
								//weight[i][j + 5] = 999999;
							}

						}


						else{  //�ƴϸ� �ϴ� ������ ��ġ. �ӽ���ġ��!
							x[0] = i;
							x[1] = i;
							y[0] = j - 1;
							y[1] = j + 4;
							//weight[i][j - 1] = 999999;
							//weight[i][j + 4] = 999999;
							break;
						}

					}
					if (board[i - 1 ][j + 1] == 2 && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 2) {  /// /���� 4�� Ȯ��
						if ((board[i + 1][j - 1] == 3 || board[i + 1][j - 1] == 1) && (board[i - 4][j + 4] == 3 || board[i - 4][j + 4] == 1)){//������ �̹� �� ���������� ���
							continue;
						}

						else if (board[i + 1][j - 1] == 2){						//5�������� 1 : �������� �拚
							if ((board[i + 2][j - 2] == 3 || board[i + 2][j - 2] == 1) && (board[i - 4][j + 4] == 3 || board[i - 4][j + 4] == 1)){//������ �̹� �� ���������� ���
								continue;
							}
							else if ((board[i + 2][j - 2] == 3 || board[i + 2][j - 2] == 1)){   //���ʸ� �������1 : ����
								x[0] = i - 4;
								y[0] = j + 4;
								//weight[i - 4][j + 4] = 999999;

							}
							else if ((board[i - 4][j + 4] == 3 || board[i - 4][j + 4] == 1)){   //���ʸ� �������2 : �Ʒ���
								x[0] = i + 2;
								y[0] = j - 2;
								//weight[i + 2][j - 2] = 999999;
							}
							else{
								x[0] = i + 2;
								x[1] = i - 4;
								y[0] = j - 2;
								y[1] = j + 4;
								//weight[i + 2][j - 2] = 999999;
								//weight[i - 4][j + 4] = 999999;
								break;
							}

						}
						else if (board[i - 4][j + 4] == 2){						//5�������� 2 : �Ʒ������� �拚
							if ((board[i + 1][j - 1] == 3 || board[i + 1][j - 1] == 1) && (board[i - 5][j + 5] == 3 || board[i - 5][j + 5] == 1)){//������ �̹� �� ���������� ���
								continue;
							}
							else if ((board[i + 1][j - 1] == 3 || board[i + 1][j - 1] == 1)){  //���ʸ� �������1 : ����
								x[0] = i - 5;
								y[0] = j + 5;
								//weight[i - 5][j + 5] = 999999;
							}
							else if ((board[i - 5][j + 5] == 3 || board[i - 5][j + 5] == 1)){  //���ʸ� �������2 : �Ʒ���
								x[0] = i + 1;
								y[0] = j - 1;
								//weight[i + 1][j - 1] = 999999;
							}
							else{
								x[0] = i + 1;
								x[1] = i - 5;
								y[0] = j - 1;
								y[1] = j + 5;
								//weight[i + 1][j - 1] = 999999;
								//weight[i - 5][j + 5] = 999999;
								break;
							}

						}

						else{//�ƴϸ� �ϴ� ������ ��ġ. �ӽ���ġ��!
							x[0] = i + 1;
							x[1] = i - 4;
							y[0] = j - 1;
							y[1] = j + 4;
							//weight[i + 1][j - 1] = 999999;
							//weight[i - 4][j + 4] = 999999;
							break;
						}

					}
					if (board[i+1][j + 1] == 2 && board[i+2][j + 2] == 2 && board[i+3][j + 3] == 2) {  /// \���� 4�� Ȯ��
						if ((board[i - 1][j - 1] == 3 || board[i - 1][j - 1] == 1) && (board[i + 4][j + 4] == 3 || board[i + 4][j + 4] == 1)){   //������ �̹� �� ���������� ���
							continue;
						}
						else if (board[i - 1][j - 1] == 2){					//5�� ������ 1 : �������� �涧
							if ((board[i - 2][j - 2] == 3 || board[i - 2][j - 2] == 1) && (board[i + 4][j + 4] == 3 || board[i + 4][j + 4] == 1)){   //������ �̹� �� ���������� ���
								continue;
							}
							else if ((board[i - 2][j - 2] == 3 || board[i - 2][j - 2] == 1)){
								x[0] = i + 4;
								y[0] = j + 4;
								//weight[i + 4][j + 4] = 999999;
							}

							else if ((board[i + 4][j + 4] == 3 || board[i + 4][j + 4] == 1)){
								x[0] = i - 2;
								y[0] = j - 2;
								//weight[i - 2][j - 2] = 999999;
							}
							else{
								x[0] = i - 2;
								x[1] = i + 4;
								y[0] = j - 2;
								y[1] = j + 4;
								//weight[i - 2][j - 2] = 999999;
								//weight[i + 4][j + 4] = 999999;
								break;
							}

						}
						else if (board[i + 4][j + 4] == 2){
							if ((board[i - 1][j - 1] == 3 || board[i - 1][j - 1] == 1) && (board[i + 5][j + 5] == 3 || board[i + 5][j + 5] == 1)){   //������ �̹� �� ���������� ���
								continue;
							}
							else if ((board[i - 1][j - 1] == 3 || board[i - 1][j - 1] == 1)){
								x[0] = i + 5;
								y[0] = j + 5;
								//weight[i + 5][j + 5] = 999999;
							}

							else if ((board[i + 5][j + 5] == 3 || board[i + 5][j + 5] == 1)){
								x[0] = i - 1;
								y[0] = j - 1;
								//weight[i - 1][j - 1] = 999999;
							}
							else{
								x[0] = i - 1;
								x[1] = i + 5;
								y[0] = j - 1;
								y[1] = j + 5;
								//weight[i - 1][j - 1] = 999999;
								//weight[i + 5][j + 5] = 999999;
								break;
							}
						}

						else{//�ƴϸ� �ϴ� ������ ��ġ. �ӽ���ġ��!
							x[0] = i - 1;
							x[1] = i + 4;
							y[0] = j - 1;
							y[1] = j + 4;
							//weight[i - 1][j - 1] = 999999;
							//weight[i + 4][j + 4] = 999999;
							break;
						}

					}





				}


			}
				
		}  

		/*int flags = 0;
		for (int i = 1; i < 20; i++){
			for (int j = 1; j < 20; j++){
				if (weight[i][j] > 0){
					if (flags == 0){
						x[0] = i;
						y[0] = j;
						weight[i][j] = -1;
						flags++;
					}
					else{
						x[1] = i;
						y[1] = j;
						weight[i][j] = -1;
					}

				}
			}
		}
		flags = 0;
		*/

		/*
		1. �ϴ� �� ��� ���� ��ġ�غ���.
		2. ���� ������ �� �� �ִ� ������ ���� ������ �ִ밪
		3. �׿� ���� �������� �� �ִ� ���� ������
		4. ���ǹ��� ���� �н�.
		01 function alphabeta(node, depth, ��, ��, maximizingPlayer)
		02      if depth = 0 or node is a terminal node
		03          return the heuristic value of node
		04      if maximizingPlayer
		05          v := -��
		06          for each child of node
		07              v := max(v, alphabeta(child, depth - 1, ��, ��, FALSE))
		08              �� := max(��, v)
		09              if �� �� ��
		10                  break (* �� cut-off *)
		11          return v
		12      else
		13          v := ��
		14          for each child of node
		15              v := min(v, alphabeta(child, depth - 1, ��, ��, TRUE))
		16              �� := min(��, v)
		17              if �� �� ��
		18                  break (* �� cut-off *)
		19          return v
		*/
	}

	// �� �κп��� �ڽ��� ���� ���� ����Ͻʽÿ�.
	// �ʼ� �Լ� : domymove(x�迭,y�迭,�迭ũ��)
	// ���⼭ �迭ũ��(cnt)�� myturn()�� �Ķ���� cnt�� �״�� �־���մϴ�.
	


	domymove(x, y, cnt); 





}

int (*getBoard(void))[20] {
	static int board[20][20];
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			board[i][j] = showBoard(i, j);
		}
	}
	return board;
}



////////////�Ű� �Ƚᵵ �� �ӽ���
int *findbestmove(int arr[20][20]) {
	static int move[2];
	//int *edge = findEdgemoves();
	return move;
}


/*int *findEdgemoves() {				//������ �����ִ� ��ġ ���� +3������ �簢���� �˻�
	int edge[4];
	int flag1 = 0;
	int flag2 = 0;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {				//���� ���� �� �� �������ִ� ���� �������� +3�� �簢���� ���ʵκ� ��
			if (showBoard(i, j) == 1 || showBoard(i, j) == 2) {
				if (i < 3 && j < 3) {
					edge[0] = 0;
					edge[1] = 0;
					flag1 = 1;
					break;
				}
				else if (i < 3) {
					edge[0] = 0;
					edge[1] = j - 3;
					flag1 = 1;
					break;
				}
				else if (j < 3) {
					edge[0] = i - 3;
					edge[1] = 0;
					flag1 = 1;
					break;
				}
				else {
					edge[0] = i - 3;
					edge[1] = j - 3;
					flag1 = 1;
					break;
				}
			}
			if (flag1 == 1) {
				break;
			}

		}
	}

		for (int i = 19; i >= 0; i++) {			 //���� ���� �� �� �������ִ� ���� �������� +3�� �簢���� �Ʒ��ʵκ� ��
			for (int j = 19; j >= 0; j++) {
				if (showBoard(i, j) == 1 || showBoard(i, j) == 2) {
					if (i > 16 && j > 16) {
						edge[2] = 19;
						edge[3] = 19;
						flag2 = 1;
						break;
					}
					else if (i > 16) {
						edge[2] = 19;
						edge[3] = j + 3;
						flag2 = 1;
						break;
					}
					else if (j > 16) {
						edge[2] = i + 3;
						edge[3] = 19;
						flag2 = 1;
						break;
					}
					else {
						edge[2] = i + 3;
						edge[3] = j + 3;
						flag2 = 1;
						break;
					}
				}
				if (flag2 == 1) {
					break;
				}

			}
		}
		return edge;
}*/