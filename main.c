#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define DO4 (261.626)
#define KEY(n) (pow(pow(2, 1.0f / 12.0f), n))
#define RE4 (KEY(2) * DO4)
#define MI4 (KEY(4) * DO4)
#define FA4 (KEY(5) * DO4)
#define SOL4 (KEY(7) * DO4)
#define RA4 (KEY(9) * DO4)
#define SI4 (KEY(11) * DO4)
#define UNIT_BEAT 200

#define DEBUG 0

enum status{
    INIT_PAGE = 0
};

enum beats{
    WHOLE_NOTE = 16,
    HALF_NOTE = 8,
    QUARTER_NOTE = 4,
    EIGHTH_NOTE = 2,
    SIXTEENTH_NOTE = 1
};


static const int beat_table[5] = {WHOLE_NOTE, HALF_NOTE, QUARTER_NOTE, EIGHTH_NOTE, SIXTEENTH_NOTE};

enum major_scale{
    c_major = 0,
    g_major = 1,
    d_major = 2,
    a_major = 3,
    e_major = 4,
    b_major = 5,
    fs_major = 6,
    cs_major = 7,
    gs_major = 8,

    f_major = 9,
    bf_major = 10,
    ef_major = 11,
    af_major = 12,
    df_major = 13,
    gf_major = 14,
    cf_major = 15,
    ff_major = 16
};

struct options{
    int major;
    int scale;
    float tonic;
    int beat;
};

void display(int status);
int get_key(int status);
void init_option(struct options *global_option);
void make_melody(struct options *global_option, float *global_notes, int *global_beats);
void play_melody(struct options *global_option, float *global_notes, int *global_beats);

int main(){
    int global_status = INIT_PAGE;
    struct options *global_option = (struct options *)malloc(sizeof(struct options));
    float global_notes[16 * 4];
    int global_beats[16 * 4];

    init_option(global_option);
    //display(INIT_PAGE);
    //get_key(INIT_PAGE);
    make_melody(global_option, global_notes, global_beats);
    play_melody(global_option, global_notes, global_beats);

    free(global_option);
    printf("END OF PROGRAM\n");
}

void init_option(struct options *global_option){
    global_option->scale = c_major;
    global_option->beat = 4;
    global_option->major = 1;
    global_option->tonic = DO4;
}

void display(int status){
    system("cls");
    if(status == INIT_PAGE){
        printf("==============================================\n");
        printf("============= Random Melody Maker ============\n");
        printf("==============================================\n");
        printf("== 1. Make Melody                           ==\n");
        printf("== 2. Option                                ==\n");
        printf("== 3. Exit                                  ==\n");
        printf("==============================================\n");
        printf("Enter the number: ");
        return;
    }
}

int get_key(int status){
    if(status == INIT_PAGE){
        char s[2]={'\0'};
        while(!sscanf("%s", s, sizeof(s))){
            ;
        }
        switch(s[0]){
            case '1':
                break;
            case '2':
                break;
            case '3':
                exit(0);
            default:
                printf("Unvalid number; please try again.\n");
                return -1;
        }
    }
}

void make_melody(struct options *global_option, float *global_notes, int *global_beats){
    if(DEBUG) printf("make_melody called\n");
    srand(time(NULL));
    float notes[16][4] = {0.0f};
    int beats[16][4] = {0};
    int remain_beats = global_option->beat * 4;
    float tonic = global_option->tonic;
    int index_i = 0;
    int index_j = 0;
	if(DEBUG) printf("remain_beats: %d\n", remain_beats);
    while(index_j != 4){
        //Get random note
        int random_note = 0;
        int random_beat = 0;
        if(index_i == 0 && index_j == 0){
            //First note: choose 1, 3, 5th note
            random_note = rand() % 3;
            if(DEBUG) printf("random_note: %d\n", random_note);
            switch(random_note){
                case 0:
                    notes[index_i][index_j] = tonic;
                    break;
                case 1:
                    notes[index_i][index_j] = KEY(4) * tonic;
                    break;
                case 2:
                    notes[index_i][index_j] = KEY(7) * tonic;
            }
        } else {
            random_note = rand() % 7;
            if(DEBUG) printf("random_note: %d\n", random_note);
            switch(random_note){
                case 0:
                    notes[index_i][index_j] = tonic;
                    break;
                case 1:
                    notes[index_i][index_j] = KEY(2) * tonic;
                    break;
                case 2:
                    notes[index_i][index_j] = KEY(4) * tonic;
                    break;
                case 3:
                    notes[index_i][index_j] = KEY(5) * tonic;
                    break;
                case 4:
                    notes[index_i][index_j] = KEY(7) * tonic;
                    break;
                case 5:
                    notes[index_i][index_j] = KEY(9) * tonic;
                    break;
                case 6:
                    notes[index_i][index_j] = KEY(11) * tonic;
                    break;   
            }
        }
        //Get random beats
        random_beat = rand() % 5;
        while(beat_table[random_beat] > remain_beats && remain_beats - beat_table[random_beat] < 0){
        	//if(DEBUG) printf("remain_beats: %d, random_beat: %d, beat: %d\n", remain_beats, random_beat, beat_table[random_beat]);
            random_beat = rand() % 5;
        }
        if(DEBUG) printf("i: %d, j: %d, random_beat: %d, beat: %d\n", index_i, index_j, random_beat, beat_table[random_beat]);
        beats[index_i][index_j] = beat_table[random_beat];
        remain_beats -= beat_table[random_beat];
        
        if(remain_beats == 0){
            index_i = 0;
            index_j++;
            remain_beats = global_option->beat * 4;
        } else {
            index_i =+ beat_table[random_beat];
        }
    }
    if(DEBUG) printf("make_melody: memory copying\n");
    memcpy(global_notes, notes, sizeof(float) * 16 * 4);
    memcpy(global_beats, beats, sizeof(int) * 16 * 4);
    if(DEBUG) printf("make_melody: end\n");
    return;
}

void play_melody(struct options *global_option, float *global_notes, int *global_beats){
    if(DEBUG) printf("play_melody called\n");
    float notes[16][4];
    int beats[16][4];
    int remain_beats = global_option->beat * 4;
    int index_i = 0;
    int index_j = 0;

    memcpy(notes, global_notes, sizeof(float) * 16 * 4);
    memcpy(beats, global_beats, sizeof(int) * 16 * 4);

	system("pause");
	
    while(index_j != 4){
        while(index_i < remain_beats){
        	printf("index_i: %d, index_j: %d, beat: %d\n", index_i, index_j, beats[index_i][index_j]);
            if(beats[index_i][index_j] == 0){
                index_i++;
                continue;
            }
            printf("note: %f, duration: %f\n", notes[index_i][index_j], UNIT_BEAT * beats[index_i][index_j]);
            Beep(notes[index_i][index_j], UNIT_BEAT * beats[index_i][index_j]);
            
            index_i++;
        }
        index_j++;
        index_i = 0;
    }
    if(DEBUG) printf("play_melody: end\n");
}
