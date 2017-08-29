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

#define BOARD_SIZE 19

// "�����ڵ�[C]"  -> �ڽ��� ���� (����)
// "AI�μ�[C]"  -> �ڽ��� �Ҽ� (����)
// ����� ���������� �ݵ�� �������� ����!
char info[] = { "TeamName:300����,Department:������б�" };
int depth = 5;
int childNum = 7;
estimate_type result;//State Node
Node* root;

void createChildNode(Node* node, int size, int* x, int* y)
{
	node->childNum = size;
	node->child_list = (Node**)malloc(sizeof(Node*) * size);

	for (int i = 0; i < size; ++i)
	{
		node->child_list[i] = (Node*)malloc(sizeof(Node));

		node->child_list[i]->value = 0;
		node->child_list[i]->x = x[i];
		node->child_list[i]->y = y[i];

		for (int j = 0; j < BOARD_SIZE; ++j) {
			node->child_list[i]->weight[j] = (int*)malloc(sizeof(int) * BOARD_SIZE);
			node->child_list[i]->board[j] = (int*)malloc(sizeof(int) * BOARD_SIZE);
		}

		for (int j = 0; j < BOARD_SIZE; ++j) {
			for (int k = 0; k < BOARD_SIZE; ++k) {
				if (x[i] == j && y[i] == k)
					node->child_list[i]->board[j][k] = 1;
				else
					node->child_list[i]->board[j][k] = node->board[j][k];

				node->child_list[i]->weight[j][k] = 0;
			}
		}

		node->child_list[i]->childNum = 0;
		node->child_list[i]->child_list = NULL;
	}
}

Node* initTree()
{
	Node *root = (Node*)malloc(sizeof(Node));

	root->value = 0;
	root->x = 0;
	root->y = 0;

	for (int i = 0; i < BOARD_SIZE; ++i) {
		root->weight[i] = (int*)malloc(sizeof(int) * BOARD_SIZE);
		root->board[i] = (int*)malloc(sizeof(int) * BOARD_SIZE);

		for (int j = 0; j < BOARD_SIZE; ++j) {
			root->board[i][j] = showBoard(i, j);
			root->weight[i][j] = 0;
		}
	}

	root->childNum = 0;
	root->child_list = NULL;

	return root;
}

void deleteNode(Node* node)
{
	int size = node->childNum;

	for (int i = 0; i < BOARD_SIZE; ++i) {
		free(node->board[i]);
		free(node->weight[i]);
	}

	for (int i = 0; i < size; ++i)
		deleteNode(node->child_list[i]);

	free(node);
}

Node* firstLeafNode(Node* root)			///�� ���� ��� ã�°�
{
	if (root->child_list != NULL)
		return firstLeafNode(root->child_list[0]);
	else
		return root;

}

void setBoard(int* board[BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = showBoard(i, j);
		}
	}
}

void getNextXY(int* weight[BOARD_SIZE], int* x, int* y) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			for (int k = 6; k >= 0; --k) {
				if (weight[x[k]][y[k]] < weight[i][j]) {
					if (k == 6) {
						x[k] = i;
						y[k] = j;
					}
					else {
						int temp = x[k];
						x[k] = x[k + 1];
						x[k + 1] = temp;

						temp = y[k];
						y[k] = y[k + 1];
						y[k + 1] = temp;
					}
				}
			}
		}
	}
}

//���⼭���� ���� ���� �Լ�

bool chkMix(int arr[6]) {
	bool f1, f2;
	f1 = f2 = false;
	for (int i = 0; i < 6; i++) {
		if (f1 && f2) return true;	//��� ���� �� ���� �� �� ������ �˸�
		if (arr[i] == 3) return true;	//����� ������ �˸�
		if (arr[i] == 1) f1 = true;			//�� ���� ������ �÷��� ��
		if (arr[i] == 2) f2 = true;			//��� ���� ������ �÷��� ��
	}
	return false;
}

int chkState(int* arr) {
	for (int i = 0; i < 6; i++)
		if (arr[i] % 3) return arr[i];
	return 0;
}

int cntStone(int* arr) {

	int cnt = 0;

	for (int i = 0; i < 6; i++)
		if (arr[i] % 3) cnt++;

	return cnt;
}

void createAllTree(Node* node, int level) {
	int k;
	if (level == depth)
		return;

	//1. �� ���� ���� weight�� ����Ѵ�.
	
	//���� 8���⿡ ���� ������ ����ġ �ο�
	for (int i = 1; i < BOARD_SIZE - 1; i++) {
		for (int j = 1; j < BOARD_SIZE - 1; j++) {
			
			for (int a = i - 1; a <= i + 1; a++) {
				for (int b = j - 1; b <= j + 1; b++) {
					if (a == i && b == j) continue;
					if (node->board[a][b] % 3) node->weight[i][j]++;	//�ֺ� 8���⿡ �� ������ ����ġ �ο� -> �ʹ� ��ó��
					if (node->board[a][b] == 1) node->weight[i][j]++;	//�� ���� �� ���̸� �� �� �� �ο�
				}
			}

			if (node->board[i][j]) node->weight[i][j] = 0;	//�� ��, ��� ��, ����̶�� ����ġ ����
		}
	}

	int space[6];

	//����
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE - 6; j++) {
			//now State : ���� ���� ������(1 or 2)
			//cnt : 6ĭ �ȿ��� �� ����
			int nowState, cnt = 0;

			//�迭�� 6�� �ֱ�
			for (k = 0; k < 6; k++)
				space[k] = node->board[i][j + k];

			//chkMix : �����ְų� ����� ������ true ����
			if (!chkMix(space)) {
				nowState = chkState(space);		//chkState : �迭 �ȿ� �ִ� ���� ���� ������(1 or 2) ����
				if (!nowState) continue;		//��������� �Ѿ��
				cnt = cntStone(space);			//�� �� ����

												//�߰��� �κ�
				if (cnt == 1 && nowState == 1) {		//�츮 �� �ϳ��� ���� ��
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i][j + k] += 5;
				}
				if (cnt >= 2) {
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i][j + k] += 5;
					if (nowState == 1)		//�� ���� �� �� ����ġ �ο�
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i][j + k] += 3;

					//3�� �̻�
					if (cnt >= 3) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i][j + k] += 10;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i][j + k] += 5;
					}
					//4�� �̻�
					if (cnt >= 4) {		//���� �����ϰų� �ٷ� ���� �� ���� ��
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i][j + k] += 999;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i][j + k] += 999;
					}
				}
			}

		}
	}

	//����
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE - 6; j++) {
			//now State : ���� ���� ������(1 or 2)
			//cnt : 6ĭ �ȿ��� �� ����
			int nowState, cnt = 0;

			//�迭�� 6�� �ֱ�
			for (k = 0; k < 6; k++)
				space[k] = node->board[j + k][i];

			//chkMix : �����ְų� ����� ������ true ����
			if (!chkMix(space)) {
				nowState = chkState(space);		//chkState : �迭 �ȿ� �ִ� ���� ���� ������(1 or 2) ����
				if (!nowState) continue;		//��������� �Ѿ��
				cnt = cntStone(space);			//�� �� ����

												//�߰��� �κ�
				if (cnt == 1 && nowState == 1) {		//�츮 �� �ϳ��� ���� ��
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[j + k][i] += 5;
				}
				if (cnt >= 2) {
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[j + k][i] += 5;
					if (nowState == 1)		//�� ���� �� �� ����ġ �ο�
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[j + k][i] += 3;

					//3�� �̻�
					if (cnt >= 3) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[j + k][i] += 10;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[j + k][i] += 5;
					}
					//4�� �̻�
					if (cnt >= 4) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[j + k][i] += 999;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[j + k][i] += 999;
					}
				}
			}

		}
	}

	//�밢�� ������ ����
	for (int i = 0; i < BOARD_SIZE - 6; i++) {
		for (int j = 0; j < BOARD_SIZE - 6; j++) {
			//now State : ���� ���� ������(1 or 2)
			//cnt : 6ĭ �ȿ��� �� ����
			int nowState, cnt = 0;

			//�迭�� 6�� �ֱ�
			for (k = 0; k < 6; k++)
				space[k] = node->board[i + k][j + k];

			//chkMix : �����ְų� ����� ������ true ����
			if (!chkMix(space)) {
				nowState = chkState(space);		//chkState : �迭 �ȿ� �ִ� ���� ���� ������(1 or 2) ����
				if (!nowState) continue;		//��������� �Ѿ��
				cnt = cntStone(space);			//�� �� ����

												//�߰��� �κ�
				if (cnt == 1 && nowState == 1) {		//�츮 �� �ϳ��� ���� ��
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i + k][j + k] += 5;
				}
				if (cnt >= 2) {
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i + k][j + k] += 5;
					if (nowState == 1)		//�� ���� �� �� ����ġ �ο�
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j + k] += 3;

					//3�� �̻�
					if (cnt >= 3) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j + k] += 10;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i + k][j + k] += 5;
					}
					//4�� �̻�
					if (cnt >= 4) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j + k] += 999;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i + k][j + k] += 999;
					}
				}
			}

		}
	}

	//�밢�� ���� ����
	for (int i = 0; i < BOARD_SIZE - 6; i++) {
		for (int j = 5; j < BOARD_SIZE; j++) {
			//now State : ���� ���� ������(1 or 2)
			//cnt : 6ĭ �ȿ��� �� ����
			int nowState, cnt = 0;

			//�迭�� 6�� �ֱ�
			for (k = 0; k < 6; k++)
				space[k] = node->board[i + k][j - k];

			//chkMix : �����ְų� ����� ������ true ����
			if (!chkMix(space)) {
				nowState = chkState(space);		//chkState : �迭 �ȿ� �ִ� ���� ���� ������(1 or 2) ����
				if (!nowState) continue;		//��������� �Ѿ��
				cnt = cntStone(space);			//�� �� ����

												//�߰��� �κ�
				if (cnt == 1 && nowState == 1) {		//�츮 �� �ϳ��� ���� ��
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i + k][j - k] += 5;
				}
				if (cnt >= 2) {
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i + k][j - k] += 5;
					if (nowState == 1)
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j - k] += 3;

					//3�� �̻�
					if (cnt >= 3) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j - k] += 10;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i + k][j - k] += 5;
					}
					//4�� �̻�
					if (cnt >= 4) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j - k] += 999;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i + k][j - k] += 999;
					}
				}
			}

		}
	}

	//2. weight�� �������� �ְ��� �� n���� ����
	int next_x[7] = { 0, };
	int next_y[7] = { 0, };

	getNextXY(node->weight, next_x, next_y);

	//3. ������ n�� ���� ������ child node ����.
	createChildNode(node, childNum, next_x, next_y);

	for (int i = 0; i < node->childNum; ++i)
		createAllTree(node->child_list[i], level + 1);

	return;
}

void myturn(int cnt) {

	int x[2], y[2];
	estimate_type result;

	if (cnt == 1) {
		// ù ���� ���.
		// �߰����� ��� �ʿ� ����.
		x[0] = 9;
		y[0] = 9;

	}
	else {
		for (int i = 0; i < cnt; i++) {
			do {
				Node* root = initTree();

				if (i == 1)
					root->board[x[0]][y[0]] = 1;

				//4. depth��ŭ Ʈ���� ����.
				createAllTree(root, 0);

				//5. �ϼ��� Ʈ���� ���� ���� �ڽĳ�带 ������.
				Node* mostLeftChildNode = firstLeafNode(root);

				//6. alpha beta pruning���� ���� ���� ���� ��.
				result = alphabeta_pruning(root, depth, -2147483647, -2147483647, true);
				//result = custom_alphabeta_pruning(root, depth, -2147483647, 2147483647, true, 2);

				for (int childIter = 0; childIter < root->childNum; ++childIter) {
					if (root->child_list[childIter]->value == result) {
						x[i] = root->child_list[childIter]->x;
						y[i] = root->child_list[childIter]->y;
					}
				}

				deleteNode(root);

				if (terminateAI) return;
			} while (!isFree(x[i], y[i]));

			if (x[1] == x[0] && y[1] == y[0]) i--;
		}
	}


	//7. ���� ���� ��.
	domymove(x, y, cnt);
}

// O(b**3d / 4)
estimate_type alphabeta_pruning(Node* state, int depth, estimate_type alpha, estimate_type beta, bool player)
{
	state->value = 0;

	if (depth == 0 || state->child_list == NULL) {
		return estimate_function(state);
	}

	if (player) {
		state->value = -2147483647;	//Negative infinite

		for (int i = 0; i < state->childNum; ++i) {
			state->value = max(state->value, alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, FALSE));
			alpha = max(alpha, state->value);

			if (beta <= alpha)
				break;
		}

		return state->value;
	}
	else {
		state->value = 2147483647;	//positive infinite

		for (int i = 0; i < state->childNum; ++i) {
			state->value = min(state->value, alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, TRUE));
			beta = min(beta, state->value);

			if (beta <= alpha)
				break;
		}

		return state->value;
	}
}

estimate_type custom_alphabeta_pruning(Node* state, int depth, estimate_type alpha, estimate_type beta, bool player, int count)
{
	state->value = 0;

	if (depth == 0 || state->child_list == NULL) {
		return estimate_function(state);
	}

	if (player) {
		state->value = -2147483647;	//Negative infinite

		for (int i = 0; i < state->childNum; ++i) {
			if (count == 1)
				state->value = max(state->value, custom_alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, TRUE, count - 1));
			else if (count == 0)
				state->value = max(state->value, custom_alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, FALSE, count - 1));

			alpha = max(alpha, state->value);

			if (beta <= alpha)
				break;
		}

		return state->value;
	}
	else {
		state->value = 2147483647;	//positive infinite

		for (int i = 0; i < state->childNum; ++i) {
			if (count == 1)
				state->value = min(state->value, custom_alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, FALSE, count - 1));
			else if (count == 0)
				state->value = min(state->value, custom_alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, TRUE, count - 1));

			beta = min(beta, state->value);

			if (beta <= alpha)
				break;
		}

		return state->value;
	}
}

estimate_type estimate_function(Node* state)
{
	estimate_type estimate_value = 0;

	for (int i = 0; i < BOARD_SIZE; i++) {			///��ü ���� ���� ���� ���� Ȯ��
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (state->board[i][j] == 1) {        /// �Ʊ��� Ȯ��-�ݵ�� �̱�� ����.
												  //����
				if (i < 14) { ///���� �����. ���� 6�� ������ �ִ� state�� �ݵ�� �̱�� ��Ȳ

					if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1 && state->board[i + 4][j] == 1 && state->board[i + 5][j] == 1)
						estimate_value += 100000;    ///������ �̱�� �ְ��� ���̱⿡ �ݵ�� �ְ�. ���� ���ϰ� �ڽð� �Ұ� ���� ���� ��.

				}
				if (i == 0) { ///���� �����. ���� 6�� ������ �ִ� state�� �ݵ�� �̱�� ��Ȳ

					if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1 && state->board[i + 4][j] == 1 && state->board[i + 5][j] == 1)
						estimate_value += 100000;    ///������ �̱�� �ְ��� ���̱⿡ �ݵ�� �ְ�. ���� ���ϰ� �ڽð� �Ұ� ���� ���� ��.
					else if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1 && state->board[i + 4][j] == 1) {
						if ((state->board[i + 5][j] == 2 || state->board[i + 5][j] == 3)) { //5���� �ѷ����� �ִ°�쿡��
							estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

						}
						else
							estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...

					}
					else if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1) {
						if ((state->board[i + 4][j] == 2 || state->board[i + 4][j] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
							estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

						}
						else
							estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...

					}

				}
				if (i > 0 && i < 14) {  ///5���� ������ �ִ� ���. ������ �ʱ� ������ ��������� �ٸ���.
					if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1 && state->board[i + 4][j] == 1) {
						if ((state->board[i - 1][j] == 2 || state->board[i - 1][j] == 3) && (state->board[i + 5][j] == 2 || state->board[i + 5][j] == 3)) { //5���� �ѷ����� �ִ°�쿡��
							estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

						}
						else
							estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
					}
				}
				if (i > 0 && i < 15) { // 4���� ���̴� ���. �̶����ʹ� ��밡 �Ű��� ����ϴ� ��Ȳ.
					if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1) {
						if ((state->board[i - 1][j] == 2 || state->board[i - 1][j] == 3) && (state->board[i + 4][j] == 2 || state->board[i + 4][j] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
							estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

						}
						else
							estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
					}
				}
				///
				///����
				if (state->board[i][j] == 1) {        /// �Ʊ��� Ȯ��-�ݵ�� �̱�� ����.

					if (j < 14) { ///���� ->����. ���� 6�� ������ �ִ� state�� �ݵ�� �̱�� ��Ȳ
						if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1 && state->board[i][j + 4] == 1 && state->board[i][j + 5] == 1) {
							estimate_value += 100000;    ///������ �̱�� �ְ��� ���̱⿡ �ݵ�� �ְ�. ���� ���ϰ� �ڽð� �Ұ� ���� ���� ��.
						}

					}
					if (j == 0) {
						if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1 && state->board[i][j + 4] == 1 && state->board[i][j + 5] == 1) {
							estimate_value += 100000;    ///������ �̱�� �ְ��� ���̱⿡ �ݵ�� �ְ�. ���� ���ϰ� �ڽð� �Ұ� ���� ���� ��.
						}
						else if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1 && state->board[i][j + 4] == 1) {
							if ((state->board[i][j + 5] == 2 || state->board[i][j + 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
						else if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1) {
							if ((state->board[i][j + 5] == 2 || state->board[i][j + 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
						else if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1) {
							if ((state->board[i][j - 1] == 2 || state->board[i][j - 1] == 3) && (state->board[i][j + 4] == 2 || state->board[i][j + 4] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}

					}



					if (j > 0 && j < 14) {  ///5���� ������ �ִ� ���. ������ �ʱ� ������ ��������� �ٸ���.
						if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1 && state->board[i][j + 4] == 1) {
							if ((state->board[i][j - 1] == 2 || state->board[i][j - 1] == 3) && (state->board[i][j + 5] == 2 || state->board[i][j + 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
					}
					if (j > 0 && j < 15) { // 4���� ���̴� ���. �̶����ʹ� ��밡 �Ű��� ����ϴ� ��Ȳ.
						if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1) {
							if ((state->board[i][j - 1] == 2 || state->board[i][j - 1] == 3) && (state->board[i][j + 4] == 2 || state->board[i][j + 4] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
					}
				}




				//////�밢�� ///����

				if (state->board[i][j] == 1) {        /// �Ʊ��� Ȯ��-�ݵ�� �̱�� ����.

					if (i < 14 && j >4) { ///���� ->����. ���� 6�� ������ �ִ� state�� �ݵ�� �̱�� ��Ȳ
						if (state->board[i + 1][j - 1] == 1 && state->board[i + 2][j - 2] == 1 && state->board[i + 3][j - 3] == 1 && state->board[i + 4][j - 4] == 1 && state->board[i + 5][j - 5] == 1) {
							estimate_value += 100000;    ///������ �̱�� �ְ��� ���̱⿡ �ݵ�� �ְ�. ���� ���ϰ� �ڽð� �Ұ� ���� ���� ��.
						}

					}
					if (i < 14 && j >4 && i > 0) {  ///5���� ������ �ִ� ���. ������ �ʱ� ������ ��������� �ٸ���.
						if (state->board[i + 1][j - 1] == 1 && state->board[i + 2][j - 2] == 1 && state->board[i + 3][j - 3] == 1 && state->board[i + 4][j - 4] == 1) {
							if ((state->board[i - 1][j + 1] == 2 || state->board[i - 1][j + 1] == 3) && (state->board[i + 5][j - 5] == 2 || state->board[i + 5][j - 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
					}
					if (i < 14 && j >4 && i == 0) {  ///5���� ������ �ִ� ���. ������ �ʱ� ������ ��������� �ٸ���.
						if (state->board[i + 1][j - 1] == 1 && state->board[i + 2][j - 2] == 1 && state->board[i + 3][j - 3] == 1 && state->board[i + 4][j - 4] == 1) {
							if ((state->board[i + 5][j - 5] == 2 || state->board[i + 5][j - 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
					}
					if (i < 15 && j >3 && i == 0) { // 4���� ���̴� ���. �̶����ʹ� ��밡 �Ű��� ����ϴ� ��Ȳ.
						if (state->board[i + 1][j - 1] == 1 && state->board[i + 2][j - 2] == 1 && state->board[i + 3][j - 3] == 1) {
							if ((state->board[i + 4][j - 4] == 2 || state->board[i + 4][j - 4] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
					}
					if (i < 15 && j >3 && i > 0) { // 4���� ���̴� ���. �̶����ʹ� ��밡 �Ű��� ����ϴ� ��Ȳ.
						if (state->board[i + 1][j - 1] == 1 && state->board[i + 2][j - 2] == 1 && state->board[i + 3][j - 3] == 1) {
							if ((state->board[i + 4][j - 4] == 2 || state->board[i + 4][j - 4] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
					}
				}


				//////�밢�� \\\����.

				if (state->board[i][j] == 1) {        /// �Ʊ��� Ȯ��-�ݵ�� �̱�� ����.

					if (i < 14 && j < 14) { ///���� ->����. ���� 6�� ������ �ִ� state�� �ݵ�� �̱�� ��Ȳ
						if (state->board[i + 1][j + 1] == 1 && state->board[i + 2][j + 2] == 1 && state->board[i + 3][j + 3] == 1 && state->board[i + 4][j + 4] == 1 && state->board[i + 5][j + 5] == 1) {
							estimate_value += 100000;    ///������ �̱�� �ְ��� ���̱⿡ �ݵ�� �ְ�. ���� ���ϰ� �ڽð� �Ұ� ���� ���� ��.
						}
					}
					if (i > 0 && i < 14 && j > 0 && j < 14) {  ///5���� ������ �ִ� ���. ������ �ʱ� ������ ��������� �ٸ���.
						if (state->board[i + 1][j + 1] == 1 && state->board[i + 2][j + 2] == 1 && state->board[i + 3][j + 3] == 1 && state->board[i + 4][j + 4] == 1) {
							if ((state->board[i - 1][j - 1] == 2 || state->board[i - 1][j - 1] == 3) && (state->board[i + 5][j + 5] == 2 || state->board[i + 5][j + 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
					}
					if (i == 0 && i < 14 && j > 0 && j < 14) {  ///5���� ������ �ִ� ���. �� ���ڶ� �����. ������ �ʱ� ������ ��������� �ٸ���.
						if (state->board[i + 1][j + 1] == 1 && state->board[i + 2][j + 2] == 1 && state->board[i + 3][j + 3] == 1 && state->board[i + 4][j + 4] == 1) {
							if ((state->board[i + 5][j + 5] == 2 || state->board[i + 5][j + 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
					}
					if (i > 0 && i < 15 && j > 0 && j < 15) { // 4���� ���̴� ���. �̶����ʹ� ��밡 �Ű��� ����ϴ� ��Ȳ.
						if (state->board[i + 1][j + 1] == 1 && state->board[i + 2][j + 2] == 1 && state->board[i + 3][j + 3] == 1) {
							if ((state->board[i - 1][j - 1] == 2 || state->board[i - 1][j - 1] == 3) && (state->board[i + 4][j + 4] == 2 || state->board[i + 4][j + 4] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
					}
					if (i == 0 && i < 15 && j > 0 && j < 15) { // 4���� ���̴� ���. �̶����ʹ� ��밡 �Ű��� ����ϴ� ��Ȳ.
						if (state->board[i + 1][j + 1] == 1 && state->board[i + 2][j + 2] == 1 && state->board[i + 3][j + 3] == 1) {
							if ((state->board[i + 4][j + 4] == 2 || state->board[i + 4][j + 4] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
					}
				}




				int availablemovecount = 0;					///���� ���� �������� �󸶳� ������ �� �ִ��� üũ�ϴ� ��. ������ �������� ���� ���ϱ�.
				for (int a = 0; a < 3; a++) {
					for (int b = 0; b < 3; b++) {
						if (a == 0 && b == 0)				//�ڱ��ڽ��� ���� �����Ƿ� ���� �� 8�� ���.
							continue;
						else {
							switch (state->board[i + a][j + b]) {
							case 0: availablemovecount++; break;	//0�̳� 1�� �츮���̳� �ƴϸ� ��������� ����. 2�� 3�� ���δµ��� ���� ���̴ϱ� �������� ������ ����.
							case 1: availablemovecount = availablemovecount + 2; break;  //����ִ°ź��� �츮�� �ֺ��� �̹� �ִ°� �� �����ϱ� +1 ����.
							case 2: break;
							case 3: break;
							}
						}
						if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 2 && b == 0) {				//�缱������ ���ؼ� ����ְų� �� ���� ���� ��쿡 ���ؼ� ����.
							if (state->board[i + a][j + b] == 1)
								availablemovecount = availablemovecount + 2;													// �����̸� �� �� ��������
							else
								availablemovecount++;
						}
						else if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 0 && b == 2) {
							if (state->board[i + a][j + b] == 1)
								availablemovecount = availablemovecount + 2;													// �����̸� �� �� ��������
							else
								availablemovecount++;
						}
						else if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 2 && b == 2) {
							if (state->board[i + a][j + b] == 1)
								availablemovecount = availablemovecount + 2;													// �����̸� �� �� ��������
							else
								availablemovecount++;
						}
						else {
							;
						}
					}
				}
				estimate_value += availablemovecount;  ///�ִ� 9�̱� ������ ���� �������ٴ� ����. �� ����ִ� ���� ��Ȳ�� ����ġ�� �߰���!

			}
			else if (state->board[i][j] == 2) {			////��븻�� ���ؼ�...
														//����
				if (i < 14) { ///���� �����. ���� 6�� ������ �ִ� state�� �ݵ�� �̱�� ��Ȳ
					if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2 && state->board[i + 4][j] == 2 && state->board[i + 5][j] == 2) {
						estimate_value -= 100000;    ///������ ���� ����
					}

				}
				if (i > 0 && i < 14) {  ///5���� ������ �ִ� ���. ������ �ʱ� ������ ��������� �ٸ���.
					if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2 && state->board[i + 4][j] == 2) {
						if ((state->board[i - 1][j] == 1 || state->board[i - 1][j] == 3) && (state->board[i + 5][j] == 1 || state->board[i + 5][j] == 3)) { //5���� �ѷ����� �ִ°�쿡��
							estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

						}
						else
							estimate_value -= 100000;  //����ġ -15 �� ���f��. �̰� �θ� ���� �ݵ�� ���ƾ� �ϴ� ��Ȳ�� �Ǳ� ������ �̷� ��Ȳ�� ���ؾ���.
					}
				}
				if (i == 0) { ///���� �����. ���� 6�� ������ �ִ� state�� �ݵ�� �̱�� ��Ȳ

					if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2 && state->board[i + 4][j] == 2 && state->board[i + 5][j] == 2)
						estimate_value += 100000;    ///������ �̱�� �ְ��� ���̱⿡ �ݵ�� �ְ�. ���� ���ϰ� �ڽð� �Ұ� ���� ���� ��.
					else if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2 && state->board[i + 4][j] == 2) {
						if ((state->board[i + 5][j] == 1 || state->board[i + 5][j] == 3)) { //5���� �ѷ����� �ִ°�쿡��
							estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

						}
						else
							estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...

					}
					else if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2) {
						if ((state->board[i + 4][j] == 1 || state->board[i + 4][j] == 1)) { //4���� �̹� �ѷ����� �ִ°�쿡��
							estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

						}
						else
							estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...

					}

				}


				if (i > 0 && i < 15) { // 4���� ���̴� ���. �̶����ʹ� ��밡 �Ű��� ����ϴ� ��Ȳ.
					if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2) {
						if ((state->board[i - 1][j] == 1 || state->board[i - 1][j] == 3) && (state->board[i + 4][j] == 1 || state->board[i + 4][j] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
							estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

						}
						else
							estimate_value -= 100000;  //����ġ -15 �� ���f��. �̰� �θ� ���� �ݵ�� ���ƾ� �ϴ� ��Ȳ�� �Ǳ� ������ �̷� ��Ȳ�� ���ؾ���.
					}
				}

				///
				///����
				if (state->board[i][j] == 2) {        /// �Ʊ��� Ȯ��-�ݵ�� �̱�� ����.

					if (j < 14) { ///���� ->����. ���� 6�� ������ �ִ� state�� �ݵ�� �̱�� ��Ȳ
						if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2 && state->board[i][j + 4] == 2 && state->board[i][j + 5] == 2) {
							estimate_value -= 100000;    ///������ ���� ����
						}

					}
					if (j == 0) {
						if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2 && state->board[i][j + 4] == 2 && state->board[i][j + 5] == 2) {
							estimate_value += 100000;    ///������ �̱�� �ְ��� ���̱⿡ �ݵ�� �ְ�. ���� ���ϰ� �ڽð� �Ұ� ���� ���� ��.
						}
						else if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2 && state->board[i][j + 4] == 2) {
							if ((state->board[i][j + 5] == 1 || state->board[i][j + 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
						else if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2) {
							if ((state->board[i][j + 5] == 1 || state->board[i][j + 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}
						else if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 2) {
							if ((state->board[i][j - 1] == 1 || state->board[i][j - 1] == 3) && (state->board[i][j + 4] == 2 || state->board[i][j + 4] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

							}
							else
								estimate_value += 1000;  //����ġ +15 �� ���f��. �̰� �θ� ������ �ݵ�� ���ƾ� �ϱ� ������...
						}

					}



					if (j > 0 && j < 14) {  ///5���� ������ �ִ� ���. ������ �ʱ� ������ ��������� �ٸ���.
						if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2 && state->board[i][j + 4] == 2) {
							if ((state->board[i][j - 1] == 1 || state->board[i][j - 1] == 3) && (state->board[i][j + 5] == 1 || state->board[i][j + 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value -= 100000;  //����ġ -15 �� ���f��. �̰� �θ� ���� �ݵ�� ���ƾ� �ϴ� ��Ȳ�� �Ǳ� ������ �̷� ��Ȳ�� ���ؾ���.
						}
					}
					if (j > 0 && j < 15) { // 4���� ���̴� ���. �̶����ʹ� ��밡 �Ű��� ����ϴ� ��Ȳ.
						if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2) {
							if ((state->board[i][j - 1] == 1 || state->board[i][j - 1] == 3) && (state->board[i][j + 4] == 1 || state->board[i][j + 4] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

							}
							else
								estimate_value -= 100000;  //����ġ -15 �� ���f��. �̰� �θ� ���� �ݵ�� ���ƾ� �ϴ� ��Ȳ�� �Ǳ� ������ �̷� ��Ȳ�� ���ؾ���.
						}
					}
				}

				//////�밢�� ///����

				if (state->board[i][j] == 2) {        /// �Ʊ��� Ȯ��-�ݵ�� �̱�� ����.

					if (i < 14 && j >4) { ///���� ->����. ���� 6�� ������ �ִ� state�� �ݵ�� �̱�� ��Ȳ
						if (state->board[i + 1][j - 1] == 2 && state->board[i + 2][j - 2] == 2 && state->board[i + 3][j - 3] == 2 && state->board[i + 4][j - 4] == 2 && state->board[i + 5][j - 5] == 2) {
							estimate_value += 100000;    ///������ �̱�� �ְ��� ���̱⿡ �ݵ�� �ְ�. ���� ���ϰ� �ڽð� �Ұ� ���� ���� ��.
						}

					}
					if (i < 14 && j >4 && i > 0) {  ///5���� ������ �ִ� ���. ������ �ʱ� ������ ��������� �ٸ���.
						if (state->board[i + 1][j - 1] == 2 && state->board[i + 2][j - 2] == 2 && state->board[i + 3][j - 3] == 2 && state->board[i + 4][j - 4] == 2) {
							if ((state->board[i - 1][j + 1] == 1 || state->board[i - 1][j + 1] == 3) && (state->board[i + 5][j - 5] == 1 || state->board[i + 5][j - 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value -= 100000;    ///������ ���� ����

							}
							else
								estimate_value -= 100000;  //����ġ -15 �� ���f��. �̰� �θ� ���� �ݵ�� ���ƾ� �ϴ� ��Ȳ�� �Ǳ� ������ �̷� ��Ȳ�� ���ؾ���.
						}
					}


					if (i < 15 && j >3 && i > 0) { // 4���� ���̴� ���. �̶����ʹ� ��밡 �Ű��� ����ϴ� ��Ȳ.
						if (state->board[i + 1][j - 1] == 2 && state->board[i + 2][j - 2] == 2 && state->board[i + 3][j - 3] == 2) {
							if ((state->board[i - 1][j + 1] == 1 || state->board[i - 1][j + 1] == 3) && (state->board[i + 4][j - 4] == 1 || state->board[i + 4][j - 4] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

							}
							else
								estimate_value -= 100000;  //����ġ -15 �� ���f��. �̰� �θ� ���� �ݵ�� ���ƾ� �ϴ� ��Ȳ�� �Ǳ� ������ �̷� ��Ȳ�� ���ؾ���.
						}
					}
				}


				//////�밢�� \\\����.

				if (state->board[i][j] == 2) {        /// �Ʊ��� Ȯ��-�ݵ�� �̱�� ����.

					if (i < 14 && j < 14) { ///���� ->����. ���� 6�� ������ �ִ� state�� �ݵ�� �̱�� ��Ȳ
						if (state->board[i + 1][j + 1] == 2 && state->board[i + 2][j + 2] == 2 && state->board[i + 3][j + 3] == 2 && state->board[i + 4][j + 4] == 2 && state->board[i + 5][j + 5] == 2) {
							estimate_value -= 100000;    ///������ ���� ����
						}
					}
					if (i > 0 && i < 14 && j > 0 && j < 15) {  ///5���� ������ �ִ� ���. ������ �ʱ� ������ ��������� �ٸ���.
						if (state->board[i + 1][j + 1] == 2 && state->board[i + 2][j + 2] == 2 && state->board[i + 3][j + 3] == 2 && state->board[i + 4][j + 4] == 2) {
							if ((state->board[i - 1][j - 1] == 1 || state->board[i - 1][j - 1] == 3) && (state->board[i + 5][j + 5] == 1 || state->board[i + 5][j + 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value -= 100000;  //����ġ -15 �� ���f��. �̰� �θ� ���� �ݵ�� ���ƾ� �ϴ� ��Ȳ�� �Ǳ� ������ �̷� ��Ȳ�� ���ؾ���.
						}
					}
					if (i == 0 && i < 14 && j > 0 && j < 15) {  ///5���� ������ �ִ� ���. ������ �ʱ� ������ ��������� �ٸ���.
						if (state->board[i + 1][j + 1] == 2 && state->board[i + 2][j + 2] == 2 && state->board[i + 3][j + 3] == 2 && state->board[i + 4][j + 4] == 2) {
							if ((state->board[i + 5][j + 5] == 1 || state->board[i + 5][j + 5] == 3)) { //5���� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�.

							}
							else
								estimate_value -= 100000;  //����ġ -15 �� ���f��. �̰� �θ� ���� �ݵ�� ���ƾ� �ϴ� ��Ȳ�� �Ǳ� ������ �̷� ��Ȳ�� ���ؾ���
						}
					}
					if (i > 0 && i < 15 && j > 0 && j < 16) { // 4���� ���̴� ���. �̶����ʹ� ��밡 �Ű��� ����ϴ� ��Ȳ.
						if (state->board[i + 1][j + 1] == 2 && state->board[i + 2][j + 2] == 2 && state->board[i + 3][j + 3] == 2) {
							if ((state->board[i - 1][j - 1] == 1 || state->board[i - 1][j - 1] == 3) && (state->board[i + 4][j + 4] == 1 || state->board[i + 4][j + 4] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

							}
							else
								estimate_value -= 100000;  //����ġ -15 �� ���f��. �̰� �θ� ���� �ݵ�� ���ƾ� �ϴ� ��Ȳ�� �Ǳ� ������ �̷� ��Ȳ�� ���ؾ���.
						}
					}
					if (i == 0 && i < 15 && j > 0 && j < 16) { // 4���� ���̴� ���. �̶����ʹ� ��밡 �Ű��� ����ϴ� ��Ȳ.
						if (state->board[i + 1][j + 1] == 2 && state->board[i + 2][j + 2] == 2 && state->board[i + 3][j + 3] == 2) {
							if ((state->board[i + 4][j + 4] == 1 || state->board[i + 4][j + 4] == 3)) { //4���� �̹� �ѷ����� �ִ°�쿡��
								estimate_value += 0; //���� ���� �������� �ƴϰ����� ���� �־��ϴ� ���� �ƴ�. 

							}
							else
								estimate_value -= 100000;  //����ġ -15 �� ���f��. �̰� �θ� ���� �ݵ�� ���ƾ� �ϴ� ��Ȳ�� �Ǳ� ������ �̷� ��Ȳ�� ���ؾ���.
						}
					}
				}

				int availablemovecount = 0;					///���� ���� �������� �󸶳� ������ �� �ִ��� üũ�ϴ� ��. ����� �������� �������ϼ��� �����ϱ�.

				for (int a = 0; a < 3; a++) {
					for (int b = 0; b < 3; b++) {
						if (a == 0 && b == 0)				//�ڱ��ڽ��� ���� �����Ƿ� ���� �� 8�� ���.
							continue;
						else {
							switch (state->board[i + a][j + b]) {
							case 0: availablemovecount--; break;	//0�̳� 1�� �츮���̳� �ƴϸ� ��������� ����. 2�� 3�� ���δµ��� ���� ���̴ϱ� �������� �������̴� ����.
							case 1: break;  //����ִ°ź��� �츮�� �ֺ��� �̹� �ִ°� �� ���� ��Ȳ��
							case 2: availablemovecount = availablemovecount - 2; break;
							case 3: break;
							}
						}
						if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 2 && b == 0) {				//�缱������ ���ؼ� ����ְų� �� ���� ���� ��쿡 ���ؼ� ����.
							if (state->board[i + a][j + b] == 2)
								availablemovecount = availablemovecount - 2;													// ���� ���ϰ�� �� �� ����
							else
								availablemovecount--;
						}
						else if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 0 && b == 2) {
							if (state->board[i + a][j + b] == 2)
								availablemovecount = availablemovecount - 2;													//  ���� ���ϰ�� �� �� ����
							else
								availablemovecount--;
						}
						else if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 2 && b == 2) {
							if (state->board[i + a][j + b] == 2)
								availablemovecount = availablemovecount - 2;													//  ���� ���ϰ�� �� �� ����
							else
								availablemovecount--;
						}
					}
				}
				estimate_value += availablemovecount;  ///availablemovecount�� �̰� ������ �Ǳ� ������ �׳� ���ϴ°ɷ�.
			}
		}
	}

	return estimate_value;
}