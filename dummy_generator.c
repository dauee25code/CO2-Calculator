#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 랜덤 정수 생성
int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

// 랜덤 실수 생성
double random_double(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

int main() {
    int year, month;
    printf("생성할 연도와 월을 입력하세요 (예: 2025 05): ");
    scanf("%d %d", &year, &month);

    FILE* file = fopen("data.csv", "w");
    if (!file) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    for (int day = 1; day <= 28; ++day) {
        double km = random_double(0.0, 100.0);      // 차량 이동 거리
        double kwh = random_double(0.0, 50.0);      // 전기 사용량
        int plastic = random_int(0, 10);            // 일회용품 개수
        double gas = random_double(0.0, 30.0);      // 가스 사용량
        int delivery = random_int(0, 5);            // 택배 횟수

        // CSV 형식 (헤더 없이)
        fprintf(file, "%d,%d,%d,%.2f,%.2f,%d,%.2f,%d\n",
            year, month, day,
            km, kwh, plastic,
            gas, delivery);
    }

    fclose(file);
    printf("data.csv 파일에 28일치 더미 데이터를 저장했습니다.\n");
    return 0;
}
