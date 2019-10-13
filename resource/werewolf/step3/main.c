#include <stdio.h>

//名前の最大文字数
#define NAME_MAX_NUM 32
#define NAME_MAX_NUM 32
#define PLAYERS_MAX_NUM 30

//役職を表すRole型を定義
typedef enum{
	VILLAGER, //実際は0
	WEREWOLF  //実際は1
}Role;

//プレイヤーを表すPlayer型を定義
typedef struct{
	Role role;               //役職番号
	char name[NAME_MAX_NUM]; //名前
	int is_live;             //生きているなら1, 死んでいるなら0
}Player;

int main(void){
	printf("人狼");
	Player players[PLAYERS_MAX_NUM];
//	役職を割り振る
//		各役職の人数を入力してもらう
	int players_num = 5;
	Role roles[] = {VILLAGER, VILLAGER, VILLAGER, VILLAGER, WEREWOLF};
//		各プレイヤーの名前を入力してもらう+プレイヤーに役職を教える
	for (int i=0;i<players_num;i++){
		players[i].is_live = 1; //生きてることにする
		players[i].role = roles[i]; //就職
	}

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
