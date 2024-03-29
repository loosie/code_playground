#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef float sample[3];


// 로지스틱 회귀분석에서는 종속변수 y값이 0 또는 1을 갖는다. 
// 그래서 우리는 주어진 데이터를 분류할 때 0인지 1인지 예측하는 모델을 만들어야 한다.
float sigmoidf(float x){
    return  1.f / (1.f + expf(-x));
}

// OR-gate
float or_train[][3] = {
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 1},
};

// AND-gate
float and_train[][3] = {
    {0, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {1, 1, 1},
};

// NAND-gate
float nand_train[][3] = {
    {0, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 0},
};

sample *train = nand_train;
size_t train_count = 4;

float rand_float(void){
    return (float)rand() / (float)RAND_MAX;
}

float cost(float w1, float w2, float b){
    // cost 평균 제곱 오차(MSE) - result 크면 나쁨, 0으로 수렴할수록 좋음 
    float result = 0.0f;
    for(size_t i=0; i<train_count; ++i){
        float x1 = train[i][0];
        float x2 = train[i][1];
        float y = sigmoidf(x1*w1 + x2*w2 + b); // O or 1?

        float d = y - train[i][2];
        result += d*d;
    }

    return result/train_count;
}

int main(){
    srand(time(NULL));
    rand();
    float w1 = rand_float();
    float w2 = rand_float();
    float b = rand_float();

    printf("w1 = %f, w2 = %f, b = %f, c = %f\n", w1, w2, b, cost(w1, w2, b));
    float eps = 1e-1;
    float rate = 1e-1;
    for(size_t i=0; i<100000; ++i){
        float c = cost(w1, w2, b);
        // printf("%f\n", cost(w1, w2, b));
        float dw1 = (cost(w1+eps, w2, b) - c)/eps;
        float dw2 = (cost(w1, w2+eps, b) - c)/eps;
        float db = (cost(w1, w2, b+eps) - c)/eps;

        w1 -= rate*dw1;
        w2 -= rate*dw2;
        b -= rate*db;
    }

    printf("w1 = %f, w2 = %f, b = %f, c = %f\n", w1, w2, b, cost(w1, w2, b));

    for(size_t i=0; i<2; ++i){
        for(size_t j=0; j<2; ++j){
            printf("%zu | %zu = %f \n", i, j, sigmoidf(i*w1 + j*w2 + b));
        }
    }

    printf("sig: %f", sigmoidf(w1 + w2 + b));

    return 0;
}