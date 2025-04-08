#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void alwaysTaken(FILE *input_file, FILE *output_file){
    char pc[11];
    char actual_outcome[3];
    int correct_predictions = 0;
    int total_predictions = 0;

    while(fscanf(input_file, "%10s %2s", pc, &actual_outcome) != EOF){
        if(actual_outcome[0] == 'T'){
            correct_predictions++;
        }
        total_predictions++;
    }
    fprintf(output_file, "%d, %d;\n",correct_predictions, total_predictions);
}

void alwaysNotTaken(FILE *input_file, FILE *output_file){
    char pc[11];
    char actual_outcome[3];
    int correct_predictions = 0;
    int total_predictions = 0;

    while(fscanf(input_file, "%10s %2c", pc, &actual_outcome) != EOF){
        if(actual_outcome[0] == 'N'){
            correct_predictions++;
        }
        total_predictions++;
    }
    fprintf(output_file, "%d, %d;\n",correct_predictions, total_predictions);
}       

void oneBimodal(FILE *input_file, FILE *output_file) {
    unsigned long long addr;  
    char behavior[10];  
    int total_predictions = 0;

    int correct_predictions[7] = {0};  

    int table4[4] = {0};
    int table8[8] = {0};
    int table32[32] = {0};
    int table64[64] = {0};
    int table256[256] = {0};
    int table1024[1024] = {0};
    int table4096[4096] = {0};

    while (fscanf(input_file, "%llx %10s\n", &addr, behavior) != EOF) {
        int actual_taken = (!strncmp(behavior, "T", 1)) ? 1 : 0; 

        if (table4[addr % 4] == actual_taken) correct_predictions[0]++;
        if (table8[addr % 8] == actual_taken) correct_predictions[1]++;
        if (table32[addr % 32] == actual_taken) correct_predictions[2]++;
        if (table64[addr % 64] == actual_taken) correct_predictions[3]++;
        if (table256[addr % 256] == actual_taken) correct_predictions[4]++;
        if (table1024[addr % 1024] == actual_taken) correct_predictions[5]++;
        if (table4096[addr % 4096] == actual_taken) correct_predictions[6]++;

        if (actual_taken) {
            table4[addr % 4] = 1;
            table8[addr % 8] = 1;
            table32[addr % 32] = 1;
            table64[addr % 64] = 1;
            table256[addr % 256] = 1;
            table1024[addr % 1024] = 1;
            table4096[addr % 4096] = 1;
        } else {
            table4[addr % 4] = 0;
            table8[addr % 8] = 0;
            table32[addr % 32] = 0;
            table64[addr % 64] = 0;
            table256[addr % 256] = 0;
            table1024[addr % 1024] = 0;
            table4096[addr % 4096] = 0;
        }

        total_predictions++;
    }

    fprintf(output_file, "%d, %d; ", correct_predictions[0], total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions[1], total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions[2], total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions[3], total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions[4], total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions[5], total_predictions);
    fprintf(output_file, "%d, %d;\n", correct_predictions[6], total_predictions);
}

void twoBimodal(FILE *input_file, FILE *output_file) {
    unsigned long long addr;  
    char behavior[10];  
    int total_predictions = 0;

    int correct_predictions4 = 0;
    int correct_predictions8 = 0;
    int correct_predictions32 = 0;
    int correct_predictions64 = 0;
    int correct_predictions256 = 0;
    int correct_predictions1024 = 0;
    int correct_predictions4096 = 0;

    int table4[4] = {0};
    int table8[8] = {0};
    int table32[32] = {0};
    int table64[64] = {0};
    int table256[256] = {0};
    int table1024[1024] = {0};
    int table4096[4096] = {0};

    while (fscanf(input_file, "%llx %10s\n", &addr, behavior) != EOF) {
        int actual_taken = (!strncmp(behavior, "T", 1)) ? 1 : 0; 

        if ((table4[addr % 4] >> 1) == actual_taken) correct_predictions4++;
        if ((table8[addr % 8] >> 1) == actual_taken) correct_predictions8++;
        if ((table32[addr % 32] >> 1) == actual_taken) correct_predictions32++;
        if ((table64[addr % 64] >> 1) == actual_taken) correct_predictions64++;
        if ((table256[addr % 256] >> 1) == actual_taken) correct_predictions256++;
        if ((table1024[addr % 1024] >> 1) == actual_taken) correct_predictions1024++;
        if ((table4096[addr % 4096] >> 1) == actual_taken) correct_predictions4096++;

        if (actual_taken) {
            if (table4[addr % 4] < 3) table4[addr % 4]++;
            if (table8[addr % 8] < 3) table8[addr % 8]++;
            if (table32[addr % 32] < 3) table32[addr % 32]++;
            if (table64[addr % 64] < 3) table64[addr % 64]++;
            if (table256[addr % 256] < 3) table256[addr % 256]++;
            if (table1024[addr % 1024] < 3) table1024[addr % 1024]++;
            if (table4096[addr % 4096] < 3) table4096[addr % 4096]++;
        } else {
            if (table4[addr % 4] > 0) table4[addr % 4]--;
            if (table8[addr % 8] > 0) table8[addr % 8]--;
            if (table32[addr % 32] > 0) table32[addr % 32]--;
            if (table64[addr % 64] > 0) table64[addr % 64]--;
            if (table256[addr % 256] > 0) table256[addr % 256]--;
            if (table1024[addr % 1024] > 0) table1024[addr % 1024]--;
            if (table4096[addr % 4096] > 0) table4096[addr % 4096]--;
        }

        total_predictions++;
    }

    fprintf(output_file, "%d, %d; ", correct_predictions4, total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions8, total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions32, total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions64, total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions256, total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions1024, total_predictions);
    fprintf(output_file, "%d, %d;\n", correct_predictions4096, total_predictions);
}

void threeBimodal(FILE *input_file, FILE *output_file) {
    unsigned long long addr;  
    char behavior[10];  
    int total_predictions = 0;

    int correct_predictions4 = 0;
    int correct_predictions8 = 0;
    int correct_predictions32 = 0;
    int correct_predictions64 = 0;
    int correct_predictions256 = 0;
    int correct_predictions1024 = 0;
    int correct_predictions4096 = 0;

    int table4[4] = {0, 0, 0, 0}; 
    int table8[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int table32[32] = {0}; 
    int table64[64] = {0}; 
    int table256[256] = {0}; 
    int table1024[1024] = {0}; 
    int table4096[4096] = {0}; 

    while (fscanf(input_file, "%llx %10s\n", &addr, behavior) != EOF) {
        int actual_taken = (!strncmp(behavior, "T", 1)) ? 1 : 0; 

        int prediction4 = (table4[addr % 4] <= 2) ? 1 : 0;  
        int prediction8 = (table8[addr % 8] <= 2) ? 1 : 0;
        int prediction32 = (table32[addr % 32] <= 2) ? 1 : 0;
        int prediction64 = (table64[addr % 64] <= 2) ? 1 : 0;
        int prediction256 = (table256[addr % 256] <= 2) ? 1 : 0;
        int prediction1024 = (table1024[addr % 1024] <= 2) ? 1 : 0;
        int prediction4096 = (table4096[addr % 4096] <= 2) ? 1 : 0;

        if (prediction4 == actual_taken) correct_predictions4++;
        if (prediction8 == actual_taken) correct_predictions8++;
        if (prediction32 == actual_taken) correct_predictions32++;
        if (prediction64 == actual_taken) correct_predictions64++;
        if (prediction256 == actual_taken) correct_predictions256++;
        if (prediction1024 == actual_taken) correct_predictions1024++;
        if (prediction4096 == actual_taken) correct_predictions4096++;

        if (actual_taken) {
            if (table4[addr % 4] > 0) table4[addr % 4]--;
            if (table8[addr % 8] > 0) table8[addr % 8]--;
            if (table32[addr % 32] > 0) table32[addr % 32]--;
            if (table64[addr % 64] > 0) table64[addr % 64]--;
            if (table256[addr % 256] > 0) table256[addr % 256]--;
            if (table1024[addr % 1024] > 0) table1024[addr % 1024]--;
            if (table4096[addr % 4096] > 0) table4096[addr % 4096]--;
        } else {
            if (table4[addr % 4] == 1) table4[addr % 4] = 3;
            else if (table4[addr % 4] < 5) table4[addr % 4]++; 

            if (table8[addr % 8] == 1) table8[addr % 8] = 3;
            else if (table8[addr % 8] < 5) table8[addr % 8]++;

            if (table32[addr % 32] == 1) table32[addr % 32] = 3;
            else if (table32[addr % 32] < 5) table32[addr % 32]++;

            if (table64[addr % 64] == 1) table64[addr % 64] = 3;
            else if (table64[addr % 64] < 5) table64[addr % 64]++;

            if (table256[addr % 256] == 1) table256[addr % 256] = 3;
            else if (table256[addr % 256] < 5) table256[addr % 256]++;

            if (table1024[addr % 1024] == 1) table1024[addr % 1024] = 3;
            else if (table1024[addr % 1024] < 5) table1024[addr % 1024]++;

            if (table4096[addr % 4096] == 1) table4096[addr % 4096] = 3;
            else if (table4096[addr % 4096] < 5) table4096[addr % 4096]++;
        }

        total_predictions++;
    }

    fprintf(output_file, "%d, %d; ", correct_predictions4, total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions8, total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions32, total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions64, total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions256, total_predictions);
    fprintf(output_file, "%d, %d; ", correct_predictions1024, total_predictions);
    fprintf(output_file, "%d, %d;\n", correct_predictions4096, total_predictions);
}

void gsharePredictor(FILE *input_file, FILE *output_file) {
    unsigned long long addr;  
    char behavior[10];  

    int correct_predictions[11] = {0};  
    int total_predictions_per_ghr[11] = {0};

    int table[4096];  
    for (int i = 0; i < 4096; i++) {
        table[i] = 0;
    }

    for (int history_len = 2; history_len < 13; history_len++) {
        int ghr = 1;  
        rewind(input_file);  
        while (fscanf(input_file, "%llx %10s\n", &addr, behavior) != EOF) {
            int actual_taken = (!strncmp(behavior, "T", 1)) ? 1 : 0;  
            int index = ((addr % 4096) ^ ghr);
            int prediction = (table[index] >= 2) ? 1 : 0;

            if (prediction == actual_taken) {
                correct_predictions[history_len - 2]++;
            }
            if (actual_taken) {
                if (table[index] < 3) {
                    table[index]++;  
                }
            } else {
                if (table[index] > 0) {
                    table[index]--; 
                }
            }

            ghr = ((ghr << 1) | actual_taken) & ((1 << history_len) - 1); 
            total_predictions_per_ghr[history_len - 2]++;
        }
    }

    for (int i = 0; i < 11; i++) {
        fprintf(output_file, "%d, %d; ", correct_predictions[i], total_predictions_per_ghr[i]);
    }
}




void tournamentPredictor(FILE *input_file, FILE *output_file) {
    // Temporary variables for branch trace parsing
    unsigned long long addr;
    char behavior[10];  

    int total_predictions = 0;
    int correct_gshare_predictions = 0;
    int correct_bimodal_predictions = 0;

    int gshare_table[4096];  // Gshare 4096-entry table
    int bimodal_table[4096]; // Bimodal 4096-entry table
    int selector_table[4096]; // Selector 4096-entry table
    int ghr = 0;  // Global History Register (12 bits)

    // Initialize tables
    for (int i = 0; i < 4096; i++) {
        gshare_table[i] = 0;   // Initialize to "Strongly Not Taken"
        bimodal_table[i] = 0;  // Initialize to "Strongly Not Taken"
        selector_table[i] = 3; // Initialize to "Strongly Prefer Bimodal" (11)
    }

    // The following loop will read a hexadecimal address and a branch behavior (T or NT)
    while (fscanf(input_file, "%llx %10s\n", &addr, behavior) != EOF) {
        int actual_taken = (!strncmp(behavior, "T", 1)) ? 1 : 0;  // Determine if branch was taken

        // Calculate indexes for both predictors
        int index = addr % 4096;   
        int gshare_index = (addr ^ ghr) % 4096;  // Gshare uses PC XOR with GHR

        // Make predictions
        int gshare_prediction = (gshare_table[gshare_index] >= 2) ? 1 : 0;
        int bimodal_prediction = (bimodal_table[index] >= 2) ? 1 : 0;

        // Tournament selector decides which predictor to use
        int selected_prediction = (selector_table[index] <= 1) ? gshare_prediction : bimodal_prediction;

        // Track correct predictions for Gshare and Bimodal
        if (gshare_prediction == actual_taken) {
            correct_gshare_predictions++;
        }
        if (bimodal_prediction == actual_taken) {
            correct_bimodal_predictions++;
        }

        // Update selector table based on which predictor was correct
        if (gshare_prediction != bimodal_prediction) {
            if (gshare_prediction == actual_taken) {
                if (selector_table[index] > 0) {  // Move towards Gshare
                    selector_table[index]--;
                }
            } else {
                if (selector_table[index] < 3) {  // Move towards Bimodal
                    selector_table[index]++;
                }
            }
        }

        // Update the Gshare predictor
        if (actual_taken) {
            if (gshare_table[gshare_index] < 3) {
                gshare_table[gshare_index]++;
            }
        } else {
            if (gshare_table[gshare_index] > 0) {
                gshare_table[gshare_index]--;
            }
        }

        // Update the Bimodal predictor
        if (actual_taken) {
            if (bimodal_table[index] < 3) {
                bimodal_table[index]++;
            }
        } else {
            if (bimodal_table[index] > 0) {
                bimodal_table[index]--;
            }
        }

        // Update Global History Register (GHR)
        ghr = ((ghr << 1) | actual_taken) & 0xFFF;  // Keep last 12 bits

        total_predictions++;
    }

    // Output the predictor with more correct predictions
    if (correct_gshare_predictions > correct_bimodal_predictions) {
        fprintf(output_file, "\n%d, %d;", correct_gshare_predictions, total_predictions);
    } else {
        fprintf(output_file, "\n%d, %d;", correct_bimodal_predictions, total_predictions);
    }
}


int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Incorrect input type \n");
        return 1;
    }  

    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w");

    if(input_file == NULL){
        printf("Cannot open file \n");
        return 1;
    }

    alwaysTaken(input_file, output_file);
    rewind(input_file);

    alwaysNotTaken(input_file, output_file);
    rewind(input_file);

    oneBimodal(input_file, output_file);
    rewind(input_file);

    twoBimodal(input_file, output_file);
    rewind(input_file);

    threeBimodal(input_file, output_file);
    rewind(input_file);

    gsharePredictor(input_file, output_file);
    rewind(input_file);

    tournamentPredictor(input_file, output_file);

    fclose(input_file);
    fclose(output_file);
    return 0;

}