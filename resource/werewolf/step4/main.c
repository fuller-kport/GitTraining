#include <stdio.h>
#include <stdlib.h>

//名前の最大文字数
#define NAME_MAX_NUM 32
#define ROLE_STR_MAX_NUM 32
#define PLAYERS_MAX_NUM 30

//役職を表すRole型を定義
typedef enum{
	VILLAGER, //実際は0
	WEREWOLF  //実際は1
}Role;

char ROLE_STRINGS[][ROLE_STR_MAX_NUM] = {"村人", "狼"};

//プレイヤーを表すPlayer型を定義
typedef struct{
	Role role;               //役職番号
	char name[NAME_MAX_NUM]; //名前
	int is_live;             //生きているなら1, 死んでいるなら0
}Player;

//プレイヤーの情報を表示 今は開発しやすいように役職も表示
void show_players(Player p[], int p_num){
	printf("\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~ << players! >> ~~~~~~~~~~~~~~~~~~~~~~~\n");
	for(int i=0;i<p_num;i++){
		if (p[i].is_live) printf("No.%d\t name: %s role:%s\n", i, p[i].name, ROLE_STRINGS[p[i].role]);
	}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

//入力を全部読み捨てる
void clean_stdin(void){
	char c;
	for(;;){
		c = getchar();
		if (c == '\n' || c == EOF) break;
	}
}

//上の関数を待機用として用いる
void wait_key(void) {
	printf("<ENTER>\n");
	clean_stdin();
}

//自分以外の生きているプレイヤーNo.を読み込む
int read_living_other_than_myself_player_num(Player players[], int players_num, int cur_player_num){
	int n;
	for (;;){
		printf("No.");
		scanf("%d", &n);
		if (n<0 || n>=players_num){
			printf("player No.%d は村にいません\n", n);
			continue;
		}
		if (n == cur_player_num){
			printf("自分自身は選択出来ません\n");
			continue;
		}
		if (!players[n].is_live){
			printf("player No.%d は既に死体です\n", n);
			continue;
		}
		wait_key();
		return n;
	}
}

//投票を集計して一人選ぶ
int ballot_winner(int ballot_box[], int size){
	int max = 0;//最大の得票数
	for(int i=0; i<size; i++) {
		if (max < ballot_box[i]) max = ballot_box[i]; 
	}
	int j = 0;//最大の投票数を得た人は何人いるか
	int arr[PLAYERS_MAX_NUM];//最大の投票数の人達(j人)
	for(int i=0; i<size; i++) {
		if (ballot_box[max] == ballot_box[i]){
			arr[j] = i;
			j++;
		}
	}
	return arr[rand()%j];
}

int main(void){
	printf("人狼");
	Player players[PLAYERS_MAX_NUM];
//	役職を割り振る
//		各役職の人数を入力してもらう
	int players_num = 5;
	Role roles[] = {VILLAGER, VILLAGER, VILLAGER, VILLAGER, WEREWOLF};
//		各プレイヤーの名前を入力してもらう+プレイヤーに役職を教える
	for (int i=0;i<players_num;i++){
		players[i].role = roles[i]; //就職
		sprintf(players[i].name, "hoge");  //名前は適当に
		players[i].is_live = 1; //生きてることにする
	}

	show_players(players, players_num);
//	昼のターン
//		一定時間待つ
//		処刑したい人を選んでもらう
//		誰かが火あぶりにされる
//		終了判定 (人狼の数が村人の人数以上なら人狼の，村人しかいないなら村人の勝利)
//	夜のターン
//		各プレイヤーの役職に応じて行動してもらう．
//		各プレイヤーの状態を決める
//		終了判定 (人狼の数が村人の人数以上なら人狼の，村人しかいないなら村人の勝利)
//	各プレイヤーに夜のターンの結果を知らせる
//	1に戻る
	return 0;
}
